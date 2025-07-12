<script setup>
import { ref } from 'vue';
const emit = defineEmits(['handleHeaderInputAdd'])

let inputText = ref("")

const handleEnter = () => {
    if (!inputText) {
        alert("输入内容不能为空！")
        return;
    }
    const todo = {
        id: Date.now(),
        name: inputText.value,
        isChecked: false
    }
    emit('handleHeaderInputAdd', todo)
    inputText.value = ""
}
</script>
<template>
    <header class="header">
        <h1>Todo记事本</h1>
        <input class="new-todo" @keydown.enter="handleEnter" v-model.trim="inputText"
            placeholder="What needs to be finished?" autofocus />
    </header>
</template>