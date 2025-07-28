import { defineStore } from 'pinia'
import { setToken , getToken } from '@/utils/token'

const useUserStore = defineStore('user', {
    state: () => ({
        token: getToken() || ''
    }),
    actions: {
        setToken(token) {
            this.token = token
            setToken(token)
        }
    }
})

export default useUserStore