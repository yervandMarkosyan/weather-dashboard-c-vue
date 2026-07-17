<script setup>

</script>

<template>
	<div>
		<nav>
			<router-link to="/" class="nav-link">Home</router-link>
			<router-link to="/weather" class="nav-link">Weather</router-link>
		</nav>

		<router-view />

	</div>
</template>

<style scoped>
nav {
	width: 70vw;
	display: flex;
	flex-direction: row;
	justify-content: space-between;
	padding: 30px;
	height: 30px;
	font-size: 1.2em;
	background-color: #f3f3f3;
	border-bottom: 1px solid #d3d3d3;
}
.nav-link {
	padding: 5px;
}
</style>
