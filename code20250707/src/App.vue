<script setup>
import { ref } from 'vue'

let x = ref(0)
</script>

<template>
	<div class="progress">
		<div class="inner" :style="{ width: x / 4 * 100 + '%' }">
			<span>{{ x / 4 * 100 }}%</span>
		</div>
	</div>
	<button @click="x = 1">设置25%</button>
	<button @click="x = 2">设置50%</button>
	<button @click="x = 3">设置75%</button>
	<button @click="x = 4">设置100%</button>
</template>

<style>
.progress {
	height: 25px;
	width: 400px;
	border-radius: 15px;
	background-color: #272425;
	border: 3px solid #272425;
	box-sizing: border-box;
	margin-bottom: 30px;
}

.inner {
	height: 20px;
	border-radius: 10px;
	text-align: right;
	position: relative;
	background-color: #409eff;
	background-size: 20px 20px;
	box-sizing: border-box;
	transition: all 1s;
}

.inner span {
	position: absolute;
	right: -25px;
	bottom: -25px;
}
</style>

<!-- <script setup>

import { ref } from 'vue'
// tabs 列表
const tabs = ref([
	{ id: 1, name: '京东秒杀' },
	{ id: 2, name: '每⽇特价' },
	{ id: 3, name: '品类秒杀' }
])

let activeIndex = ref(0)

</script>
<template>
	<div>
		<ul>
			<li v-for="item, index in tabs" :key="item.id"><a :class="activeIndex === index ? 'active' : ''" href="#"
					@click="activeIndex = index">{{ item.name }}</a></li>
		</ul>
	</div>
</template>
<style>
* {
	margin: 0;
	padding: 0;
}

ul {
	display: flex;
	border-bottom: 2px solid #e01222;
	padding: 0 10px;
}

li {
	width: 100px;
	height: 50px;
	line-height: 50px;
	list-style: none;
	text-align: center;
}

li a {
	display: block;
	text-decoration: none;
	font-weight: bold;
	color: #333333;
}

li a.active {
	background-color: #e01222;
	color: #fff;
}
</style> -->