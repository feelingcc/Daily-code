import { createRouter , createWebHashHistory , createWebHistory} from 'vue-router'
import Login from '@/views/Login.vue'
import Home from '@/views/Home.vue'
import NotFound from '@/views/NotFound.vue'
import Recommend from '@/views/Recommend.vue'
import Setting from '@/views/Setting.vue'

const router = createRouter({
    history: createWebHistory(),
    routes: [
        {
            path: '/',
            redirect: '/login'
        },
        {
            path: '/login',
            component: Login
        },
        {
            name: 'home',
            path: '/home',
            component: Home,
            redirect: '/home/recommend',
            children: [
                {
                    path: 'recommend',
                    component: Recommend,
                    beforeEnter: () => {
                        return { name: 'set' }
                    }
                },
                {   
                    name: 'set',
                    path: 'setting',
                    component: Setting,
                    // beforeEnter: (to ,from , next) => {
                    //     next()
                    //     // next({
                    //     //     path: to.path
                    //     // })
                    //     // return true
                    // }
                }
            ]
        },
        {
            path: '/:pathMatch(.*)*',
            component: NotFound
        }
    ]
})

const isLogin = false

router.beforeEach((to , from ) => {
    // next()
    // if (!isLogin) {
    //     next()
    //     return
    // }
    // next({
    //     path: to.path
    // })

    // return true
    // return { name: 'set' }
})

export default router