<script setup>
const { todoList } = defineProps({
    todoList: {
        type: Array,
        default: () => []
        // required: true
    }
})

const emit = defineEmits(['handleMainClickDelete'])

const handleClickDel = (index) => {
    if (confirm("确认要删除吗？")) {
        emit('handleMainClickDelete', index)
    }
}
</script>

<template>
    <section class="main">
        <input id="toggle-all" class="toggle-all" type="checkbox" />
        <label for="toggle-all">Mark all as complete</label>
        <ul class="todo-list">
            <li v-for="item, index in todoList" :key="item.id" :class="{ completed: item.isChecked }">
                <div class="view">
                    <input class="toggle" type="checkbox" v-model="item.isChecked" />
                    <label>{{ item.name }}</label>
                    <button class="destroy" @click="handleClickDel(index)"></button>
                </div>
            </li>
        </ul>
    </section>
</template>