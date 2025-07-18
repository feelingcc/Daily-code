<script setup>
import { useRoute } from 'vue-router';
const route = useRoute()
console.log(route.query)
</script>

<template>
    <div class="home">
        <router-link to="/home/recommend">推荐</router-link>
        <br>
        <router-link to="/home/setting">设置</router-link>

        <p>这是登录成功的页面</p>
        <p>
            <router-view></router-view>
        </p>
    </div>
</template>

<style scoped></style>