<script setup>
import { nextTick, onUpdated, ref, watch } from 'vue';
const isEdit = ref(false)
const inputRef = ref(null)
const model = defineModel();

const handleDbClick = async () => {
    isEdit.value = !isEdit.value
    await nextTick()    // 等待DOM更新
    inputRef.value.focus()
}

const handleInputBlur = () => {
    if (inputRef.value.value.trim()) {
        model.value = inputRef.value.value.trim()
    }
    isEdit.value = !isEdit.value
}

// watch(isEdit, async (newVal) => {
    
// })

</script>
<template>
    <div class="my-tag">
        <input v-if="isEdit" :value="model" ref="inputRef" @blur="handleInputBlur" class="input" type="text"
            placeholder="输⼊标签" />
        <div v-else @dblclick="handleDbClick" class="text">
            {{ model }}
        </div>
    </div>
</template>
<style lang="scss" scoped>
.my-tag {
    cursor: pointer;

    .input {
        appearance: none;
        outline: none;
        border: 1px solid #ccc;
        width: 100px;
        height: 40px;
        box-sizing: border-box;
        padding: 10px;
        color: #666;

        &::placeholder {
            color: #666;
        }
    }
}
</style>
