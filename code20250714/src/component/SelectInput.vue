<script setup>
const currentSelect = defineModel()
</script>

<template>
    <div class="select-input-wrap">
        <select v-model="currentSelect">
            <option value="BJ">北京</option>
            <option value="SH">上海</option>
            <option value="GZ">广州</option>
            <option value="SZ">深圳</option>
        </select>
    </div>
</template>

<style scoped></style>