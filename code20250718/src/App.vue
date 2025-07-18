<script setup>
</script>

<template>
	<nav>
		<router-link :to="{
			path: '/login',
			query: {
				id: 2,
				name: 'a'
			}
		}">登录</router-link>
		<br>
		<router-link to="/home">home</router-link>
		<!-- <router-link :to="{
			name: 'home',
			params: {
				id: 1,
				name: 'c'
			}
		}">Home</router-link> -->
	</nav>
	<div>
		<router-view></router-view>
	</div>
</template>

<style scoped></style>