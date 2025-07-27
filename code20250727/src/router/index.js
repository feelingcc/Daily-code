import { createRouter, createWebHistory } from 'vue-router'
import useUserStore from '@/stores/user'

const router = createRouter({
	history: createWebHistory(import.meta.env.BASE_URL),
	routes: [
		{
			path: '/',
			component: () => import('@/views/Home/index.vue'),
			redirect: '/article',
			children: [
				{
					path: 'article',
					component: () => import('@/views/Article/index.vue')
				},
				{
					path: 'dashboard',
					component: () => import('@/views/Dashboard/index.vue')
				}
			]
		},
		{
			path: '/login',
			component: () => import('@/views/Login/index.vue')
		}
	]
})

const whiteList = ['/login']
router.beforeEach((to, from) => {
	const userStore = useUserStore()
	if (!userStore.token) {
		// 未登录
		if (whiteList.includes(to.path)) {
			return true
		} else {
			// 登录完跳回刚才用户想去的页面
			return `/login?redirect=${to.path}`
		}
	} 
	
	// 登录后，如果用户想去的页面是登录页，则跳转到首页
	if (userStore.token && to.path === '/login') {
		return '/'
	}
	
	return true
})

export default router
