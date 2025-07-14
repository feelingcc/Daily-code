<script setup>
import { nextTick, ref } from 'vue'

const isEidt = ref(false)
const inputRef = ref(null)
const showEidt = async () => {
	isEidt.value = true
	await nextTick()
	inputRef.value.focus()
}
</script>

<template>
	<div class="box">
		<h3>标题</h3>
		<button @click="showEidt">编辑</button>
	</div>

	<div v-if="isEidt">
		<input type="text" ref="inputRef">
	</div>
</template>

<style scoped>
.box {
	display: flex;
	justify-content: space-around;
	align-items: center;
	width: 200px;
	height: 40px;
}
</style>

<!-- <script setup>
import { ref, onMounted } from 'vue';

const pRef = ref(null)

onMounted(() => {
	console.log(`output->pRef`, pRef.value)
})
</script>

<template>
	<div>
		<p ref="pRef">Hello World</p>
	</div>
</template>

<style scoped></style> -->

<!-- <script setup>
import { ref } from 'vue';
import SelectInput from './component/SelectInput.vue';


const currentSelect = ref("BJ")
</script>

<template>
	<select-input v-model="currentSelect"></select-input>
</template>

<style scoped></style> -->

<!-- <script setup>
import { ref } from 'vue';

const msg = ref("")
</script>

<template>
	<div>
		<p>{{ msg }}</p>
		<input type="text" :value="msg" @input="msg = $event.target.value">
	</div>
</template>

<style scoped></style> -->

<!-- <script setup>
import { ref } from 'vue';

const msg = ref("")
</script>

<template>
	<div>
		<p>{{ msg }}</p>
		<input type="text" v-model="msg">
	</div>
</template>

<style scoped></style> -->