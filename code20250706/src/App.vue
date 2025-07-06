<script setup>
// 导⼊ todo 样式
import './app.css'

import { ref, reactive } from 'vue'

let input_value = ref("")

const add = () => {
	if (input_value.value === "") {
		window.alert("输入框内容不能为空！")
		return
	}
	const new_todo = {
		id: Date.now(),
		name: input_value.value.trim(),
		finished: false
	}
	todoList.value.push(new_todo)
	input_value.value = ""
}

// 代办任务列表
const todoList = ref([
	{ id: 321, name: '吃饭', finished: false },
	{ id: 666, name: '睡觉', finished: true },
	{ id: 195, name: '打⾖⾖', finished: false }
])

const handleDelete = (pos) => {
	if (window.confirm('确认删除吗？'))
		todoList.value.splice(pos, 1)
}

const handleClear = () => {
	// todoList.value.splice(0, todoList.value.length)
	todoList.value = []
}
</script>

<template>
	<section clsss="todoapp">
		<header class="header">
			<h1>TODOLIST</h1>
			<input placeholder="请输⼊任务" class="new-todo" v-model="input_value" />
			<button class="add" @click="add">添加任务</button>
		</header>
		<section class="main">
			<ul class="todo-list">
				<li class="todo" v-for="item, index in todoList" :key="item.id">
					<div class="view">
						<span class="index">{{ index + 1 }}.</span> <label>{{ item.name }}</label>
						<button class="destroy" @click="handleDelete(index)"></button>
					</div>
				</li>
			</ul>
		</section>
		<footer class="footer">
			<span class="todo-count">合 计: <strong> {{ todoList.length }} </strong></span>
			<button class="clear-completed" @click="handleClear">清空任务</button>
		</footer>
	</section>
</template>


<style scoped></style>