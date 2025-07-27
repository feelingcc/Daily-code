import axios from 'axios'

const instance = axios.create({
    baseURL: 'http://localhost:4000/api',
    timeout: 5000
});

// 添加请求拦截器
instance.interceptors.request.use(function (config) {
    // 在发送请求之前做些什么
    return config;
}, function (error) {
    // 对请求错误做些什么
    return Promise.reject(error);
});

const RESPONSE_CODE = {
    SUCCESS: 10000,
    ERROR: 10001
}

// 添加响应拦截器
instance.interceptors.response.use(function (response) {
    // 2xx 范围内的状态码都会触发该函数。
    // 对响应数据做点什么
    const { data } = response
    if (data.code === RESPONSE_CODE.SUCCESS) {
        return data.data
    } else {
        return Promise.reject(data.message)
    }
}, function (error) {
    // 超出 2xx 范围的状态码都会触发该函数。
    // 对响应错误做点什么
    return Promise.reject(error);
});

export default instance