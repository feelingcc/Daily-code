import request from '@/utils/request'

const login = (data) => {
    return request({
        url: '/login',
        method: 'post',
        data
    })
}

export { login }