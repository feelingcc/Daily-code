<script setup>
// vbase
import { ref } from 'vue'
const str = ref('<span style="color:red;">Hello Vue</span>')
</script>

<template>
	<div>
		<p v-html="str"></p>
		<p v-text="str"></p>
	</div>
</template>

<style scoped></style>
