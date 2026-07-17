<template>
	<div>
		<h1>Weather Dashboard</h1>
		<div id="wrapper">
			<weather-card :cityList="cities" />
		</div>
	</div>
</template>

<script setup>

import { ref } from 'vue'
import brasilia from '../assets/brasilia.svg'
import yerevan from '../assets/yerevan.svg'
import moscow from '../assets/moscow.svg'
import beijing from '../assets/beijing.svg'
import paris from '../assets/paris.svg'
import tokyo from '../assets/tokyo.svg'
import WeatherCard from '../components/WeatherCard.vue'

const cities = ref([
	{ name: "Brasilia", temperature: 25, desc: "rainy", img: brasilia },
	{ name: "Yerevan", temperature: 3, desc: "clear sky", img: yerevan },
	{ name: "Moscow", temperature: 0, desc: "snowy", img: moscow },
	{ name: "Beijing", temperature: 10, desc: "clear sky", img: beijing },
	{ name: "Paris", temperature: 20, desc: "overcast clouds", img: paris },
	{ name: "Tokyo", temperature: 15, desc: "rainy", img: tokyo },
	])
</script>

<style scoped>
div {
	display: flex;
	flex-direction: column;
	align-items: center;
}
</style>
