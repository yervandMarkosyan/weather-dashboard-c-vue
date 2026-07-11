#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/types.h>
#include "dotenv.h"
#include <microhttpd.h>


#define PORT 3000


struct MemoryBuffer
{
    char *response_data;
    size_t size;
};

// Callback used by libcurl to aggregate network packets into a clean memory string
static size_t curl_write_data(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryBuffer *mem = (struct MemoryBuffer *)userp;

    char *ptr = realloc(mem->response_data, mem->size + realsize + 1);
    if (ptr == NULL) return 0;

    mem->response_data = ptr;
    memcpy(&(mem->response_data[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response_data[mem->size] = '\0';

    return realsize;
}

// Utility helper to respond with JSON data payload errors or success messages
enum MHD_Result send_json_response(struct MHD_Connection *connection, unsigned int status_code, const char *json_str)
{
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(json_str), (void *)json_str, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(response, "Content-Type", "application/json");
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*"); // Simulate CORS
    enum MHD_Result ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    return ret;
}

// Main HTTP request lifecycle router entry point
enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                               const char *url, const char *method,
                               const char *version, const char *upload_data,
                               size_t *upload_data_size, void **con_cls)
{
   const char *prefix = "/api/weather/";
   if (strcmp(method, "GET") == 0 && strncmp(url, prefix, strlen(prefix)) == 0) {
    const char *city = url + strlen(prefix);
    if (strlen(city) == 0) {
        return send_json_response(connection, MHD_HTTP_BAD_REQUEST, "{\"error\":\"City parameter not provided\"}");
    }

    env_load(".env", false);

    char *api_key = getenv("WEATHER_API_KEY");
    if (api_key == NULL) {
        return send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, "{\"error\":\"API key missing\"}");
    }

    // Initialize libcurl request handle (http client) to forward call to OpenWeather
    CURL *curl_handle = curl_easy_init();
    if (!curl_handle) {
        return send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, "{\"error\":\"Failed to initialize HTTP client\"}");
    }

    // URL encode the city string safely to prevent route breaks
    char *escaped_city = curl_easy_escape(curl_handle, city, 0);

    // Build the query endpoint string target
    char target_url[512];
    snprintf(target_url, sizeof(target_url),
             "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric",
             escaped_city, api_key);
    curl_free(escaped_city);

    struct MemoryBuffer weather_buffer = { .response_data = malloc(1), .size = 0 };

    curl_easy_setopt(curl_handle, CURLOPT_URL, target_url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_write_data);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&weather_buffer);

    CURLcode res = curl_easy_perform(curl_handle);
    long http_status_code = 0;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_status_code);
    curl_easy_cleanup(curl_handle);

    if (res != CURLE_OK || http_status_code != 200) {
        free(weather_buffer.response_data);
        return send_json_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, "{\"error\":\"Failed to fetch weather data\"}");
    }

    // Stream the collected OpenWeatherMap JSON directly to our client
    struct MHD_Response *response = MHD_create_response_from_buffer(weather_buffer.size, (void *)weather_buffer.response_data, MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(response, "Content-Type", "application/json");
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*"); // Enable CORS

    enum MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
   }

   // Default Fallback: 404 Route Not Found
   return send_json_response(connection, MHD_HTTP_NOT_FOUND, "{\"error\":\"Not Found\"}");
}
int main()
{
    struct MHD_Daemon *daemon;

    curl_global_init(CURL_GLOBAL_ALL);

    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &handle_request, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        printf("CRITICAL: Failed to start web server daemon\n");
        return 1;
    }

    printf("Server running on port %d. Press Enter to stop...\n", PORT);
    getchar();

    MHD_stop_daemon(daemon);
    curl_global_cleanup();
    return 0;
}
