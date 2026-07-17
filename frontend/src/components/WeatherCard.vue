
<template>
  <div class="container">
    <!-- Enhanced Select Dropdown Container -->
    <div class="select-wrapper">
      <select v-model="selectedCity" @change="chooseCity" class="custom-select">
        <option disabled value="">Choose a city...</option>
        <option v-for="city in cityList" :key="city.name" :value="city.name">
          {{ city.name }}
        </option>
      </select>
    </div>

    <!-- UI State Feedback (Loading & Error States) -->
    <div v-if="isLoading" class="status-msg loading">Fetching forecast...</div>
    <div v-if="errorMessage" class="status-msg error">{{ errorMessage }}</div>

    <!-- Premium Card Design (Transitions smoothly into view) -->
    <Transition name="fade">
      <div 
        v-if="currentCity && !isLoading" 
        @click="weatherWebsite" 
        class="weather-card"
      >
        <div class="card-header">
          <h2>{{ currentCity.name }}</h2>
          <span class="pulse-dot"></span>
        </div>

        <div class="card-body">
          <div class="temp-display">
            <span class="temp-num">{{ Math.round(currentCity.temperature) }}</span>
            <span class="temp-unit">&deg;C</span>
          </div>
          
          <img :src="currentCity.img" :alt="currentCity.name" class="weather-icon"/>
        </div>

        <div class="card-footer">
          <p class="weather-desc"><strong>{{ currentCity.desc }}</strong></p>
          <p class="info-text">Click card for extended forecast details on AccuWeather.</p>
        </div>
      </div>
    </Transition>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import axios from 'axios'

const props = defineProps({
  cityList: {
    type: Array,
    required: true
  }
})

const selectedCity = ref('')
const currentCity = ref(null)
const isLoading = ref(false)
const errorMessage = ref('')

const chooseCity = async () => {
  if (!selectedCity.value) return
  
  const cityMeta = props.cityList.find(city => city.name === selectedCity.value)
  isLoading.value = true
  errorMessage.value = ''
  
  try {
    // Uses frontend proxy routing to prevent browser security blocks
    const response = await axios.get(`http://localhost:3000/api/weather/${selectedCity.value}`)
    
    currentCity.value = {
      name: selectedCity.value,
      temperature: response.data?.main?.temp ?? response.data?.temp ?? 0,
      desc: response.data?.weather?.[0]?.description ?? response.data?.description ?? 'Clear',
      img: cityMeta?.img
    }
  } catch (error) {
    console.error(error)
    errorMessage.value = 'Failed to load weather data. Please try again.'
    currentCity.value = null
  } finally {
    isLoading.value = false
  }
}

const weatherWebsite = () => {
  window.open('https://www.accuweather.com', '_blank')
}
</script>

<style scoped>
/* Container Layout */
.container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 25px;
  font-family: system-ui, -apple-system, sans-serif;
}

/* Fancier Dropdown Menu */
.select-wrapper {
  position: relative;
  width: 280px;
}

.custom-select {
  width: 100%;
  padding: 12px 16px;
  font-size: 1rem;
  font-weight: 500;
  color: #333;
  background-color: #fff;
  border: 2px solid #e0e0e0;
  border-radius: 12px;
  appearance: none;
  cursor: pointer;
  transition: all 0.3s ease;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.05);
}

.custom-select:focus {
  outline: none;
  border-color: #4a90e2;
  box-shadow: 0 0 0 4px rgba(74, 144, 226, 0.15);
}

/* Glassmorphism Weather Card Layout */
.weather-card {
  background: rgba(255, 255, 255, 0.85);
  backdrop-filter: blur(10px);
  border: 1px solid rgba(255, 255, 255, 0.4);
  border-radius: 24px;
  padding: 30px;
  width: 340px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.08);
  cursor: pointer;
  transition: transform 0.4s cubic-bezier(0.175, 0.885, 0.32, 1.275), box-shadow 0.3s ease;
}

.weather-card:hover {
  transform: translateY(-8px);
  box-shadow: 0 20px 40px rgba(0, 0, 0, 0.15);
}

/* Card Header elements */
.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  border-bottom: 1px solid rgba(0, 0, 0, 0.06);
  padding-bottom: 12px;
  margin-bottom: 20px;
}

.card-header h2 {
  margin: 0;
  font-size: 1.4rem;
  color: #1a1a1a;
  font-weight: 700;
}

.pulse-dot {
  width: 8px;
  height: 8px;
  background-color: #2ecc71;
  border-radius: 50%;
  box-shadow: 0 0 0 0 rgba(46, 204, 113, 0.7);
  animation: pulse 2s infinite;
}

/* Content Elements */
.card-body {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.temp-display {
  display: flex;
  align-items: flex-start;
}

.temp-num {
  font-size: 4rem;
  font-weight: 800;
  line-height: 1;
  color: #111;
  letter-spacing: -2px;
}

.temp-unit {
  font-size: 1.5rem;
  font-weight: 600;
  color: #4a90e2;
  margin-top: 4px;
}

.weather-icon {
  height: 75px;
  width: auto;
  filter: drop-shadow(0 4px 8px rgba(0, 0, 0, 0.1));
}

/* Card Bottom Text */
.weather-desc {
  margin: 0 0 8px 0;
  font-size: 1.1rem;
  text-transform: capitalize;
  color: #2c3e50;
}

.info-text {
  margin: 0;
  font-size: 0.8rem;
  color: #888;
}

/* Status Notifications */
.status-msg {
  font-size: 0.95rem;
  font-weight: 500;
  padding: 10px 20px;
  border-radius: 8px;
}

.loading { color: #666; }
.error { color: #e74c3c; background-color: #fde8e7; }

/* Entry/Exit Animations */
.fade-enter-active,
.fade-leave-active {
  transition: opacity 0.3s ease, transform 0.3s ease;
}

.fade-enter-from,
.fade-leave-to {
  opacity: 0;
  transform: scale(0.95);
}

@keyframes pulse {
  0% { transform: scale(0.95); box-shadow: 0 0 0 0 rgba(46, 204, 113, 0.7); }
  70% { transform: scale(1); box-shadow: 0 0 0 10px rgba(46, 204, 113, 0); }
  100% { transform: scale(0.95); box-shadow: 0 0 0 0 rgba(46, 204, 113, 0); }
}
</style>
