import { createRouter, createWebHistory } from 'vue-router'
import Weather from  '../pages/Weather.vue'

const routes = [
	{ path: '/', component: Weather },
	{ path: '/weather', redirect: '/'}
]

const router = createRouter({
	history: createWebHistory(),
	routes
})

export default router
