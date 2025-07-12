<script setup>
import { computed } from 'vue';
const { todoList } = defineProps({
    todoList: {
        type: Array,
        default: () => []
        // required: true
    }
})

const emit = defineEmits(['handleFooterClickClear'])

const unfinished = computed(() => {
    return todoList.reduce((prev, item) => {
        return prev + (item.isChecked ? 0 : 1)
    }, 0)
})
// console.log(`output->unfinished`, unfinished.value)

const handleClickClear = () => {
    emit('handleFooterClickClear')
}
</script>
<template>
    <footer class="footer">
        <span class="todo-count"><strong>{{ todoList.length ? unfinished : 0 }}</strong> item left</span>
        <ul class="filters">
            <li>
                <a href="#/" class="selected">All</a>
            </li>
            <li>
                <a href="#/active">Active</a>
            </li>
            <li>
                <a href="#/completed">Completed</a>
            </li>
        </ul>
        <button class="clear-completed" @click="handleClickClear">
            Clear completed
        </button>
    </footer>
</template>