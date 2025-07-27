import './style/index.css'
import { createApp } from 'vue'
import { createPinia } from 'pinia'
import ElementPlus from 'element-plus'  
import 'element-plus/dist/index.css'
import { User , Lock } from '@element-plus/icons-vue'

import App from './App.vue'
import router from './router'

const app = createApp(App)

app.use(createPinia())
app.use(router)
app.use(ElementPlus)

app.component(User.name , User)
app.component(Lock.name , Lock)

app.mount('#app')
