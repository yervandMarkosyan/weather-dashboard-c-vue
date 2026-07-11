# 🌦️ Weather Dashboard

A full-stack weather dashboard built using **Vue.js** for the frontend and **C with GNU microhttpd and libcurl** for the backend. It fetches real-time weather data from external APIs and displays it in a clean, responsive interface. This project was developed to practice systems-level web development with a focus on manual memory safety, low-overhead HTTP daemon polling, and third-party C API integrations.

## 🚀 Technologies Used

- Vue.js (Composition API)
- Tailwind CSS
- C (Standard C11 / GCC)
- GNU microhttpd (`libmicrohttpd`)
- libcurl (`libcurl`)

## 📦 How to Run Locally

### 1. Clone the repository

```bash
git clone https://github.com/yervandMarkosyan/weather-dashboard-vue-js.git
cd weather-dashboard-vue-js
```

### 2. Install system dependencies for backend

```bash
# On Ubuntu / Debian Linux
sudo apt update && sudo apt install libmicrohttpd-dev libcurl4-openssl-dev gcc make

# On macOS (via Homebrew)
brew install libmicrohttpd curl
```

### 3. Set up the environment API key

```bash
export WEATHER_API_KEY="your_actual_openweathermap_api_key"
```

### 4. Compile the C backend service

```bash
gcc main.c -Iinclude -o weather_server -lmicrohttpd -lcurl
```

### 5. Start the full-stack server application

```bash
./weather_server
```
