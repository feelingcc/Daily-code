
import { createApp } from 'vue'
import App from './App.vue'
// ./router/index.js -> index.js可省略
import router from './router'

const app = createApp(App)
app.use(router)
app.mount('#app')
