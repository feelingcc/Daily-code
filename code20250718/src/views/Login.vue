<script setup>
import { onMounted } from 'vue';
import { useRoute, useRouter } from 'vue-router';
const route = useRoute()    // 接收参数的
const router = useRouter()  // 编程式导航
console.log(route.query)

onMounted(() => {
    setTimeout(() => {
        // router.push('/home')
        router.push({
            path: '/home',
            query: {
                id: 1,
                name: 'abc'
            }
        })
    }, 1000)
})
</script>

<template>
    <div class="login">
        这是登录页面

    </div>
</template>

<style scoped></style>