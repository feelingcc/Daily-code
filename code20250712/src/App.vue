<script setup>
import './assets/style.css'
import { ref, watch } from 'vue'
import TodoHeader from './component/TodoHeader.vue';
import TodoMain from './component/TodoMain.vue';
import TodoFooter from './component/TodoFooter.vue';


// [
// 	{ id: 1, name: "学Vue组件通信", isChecked: false },
// 	{ id: 2, name: "打王者", isChecked: false },
// ]

const LS_TODOLIST_KEY = "ls-todolist-key"

const todoList = ref(JSON.parse(localStorage.getItem(LS_TODOLIST_KEY)) || [])

watch(todoList, (newval) => {
	localStorage.setItem(LS_TODOLIST_KEY, JSON.stringify(newval))
}, { deep: true })

const handleHeaderInputAdd = (todo) => {
	todoList.value.push(todo)
}

const handleMainClickDelete = (index) => {
	todoList.value.splice(index, 1)
}

const handleFooterClickClear = () => {
	// ref返回的是一个对象 .value是其的属性，const修饰的是ref对象，而不是对象中的属性
	todoList.value = todoList.value.filter(item => !item.isChecked)
}

</script>

<template>
	<div class="todoapp">
		<!-- vue组件可以使用v-if控制 -->
		<todo-header @handleHeaderInputAdd="handleHeaderInputAdd"></todo-header>
		<todo-main :todoList="todoList" @handleMainClickDelete="handleMainClickDelete"></todo-main>
		<todo-footer :todoList="todoList" @handleFooterClickClear="handleFooterClickClear"></todo-footer>
	</div>
</template>

<style scoped></style>






<!-- <script setup>
import { ref } from 'vue'
import Item from './component/Item.vue';

let x = ref(0)

const handleClickAdd = (num) => {
	x.value += num
}
</script>

<template>
	<item :x="x" @handleClickAdd="handleClickAdd"></item>
</template>

<style lang="scss"></style> -->