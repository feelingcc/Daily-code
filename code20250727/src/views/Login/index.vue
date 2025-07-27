<script setup>
import { ref, reactive } from 'vue'
import { login } from '@/api/login'
import { ElMessage } from 'element-plus'
import useUserStore from '@/stores/user'
import { useRouter , useRoute } from 'vue-router'

const router = useRouter()
const route = useRoute()

const userStore = useUserStore()

const formRef = ref(null)

const form = reactive({
    username: 'admin',
    password: 'admin'
})

const rules = {
    username: [
        { required: true, message: '请输入用户名', trigger: 'blur' },
        { min: 3, max: 10, message: '用户名长度在3-10个字符之间', trigger: 'blur' }
    ],
    password: [
        { required: true, message: '请输入密码', trigger: 'blur' },
        { min: 5, max: 16, message: '密码长度在5-16个字符之间', trigger: 'blur' }
    ]
}

const handleLogin = () => {
    formRef.value.validate(async (valid) => {
        if (!valid) {
            return
        }
        try {
            const res = await login(form)
            userStore.setToken(res.token)
            router.push(route.query.redirect || '/')
            ElMessage.success('登录成功')
        } catch (error) {
            ElMessage.error(error)
        }
    })
}

const handleReset = () => {
    formRef.value.resetFields()
}
</script>


<template>
    <div class="login-container">
        <el-card>
            <template #header>
                <div class="card-header">
                    <span>登录</span>
                </div>
            </template>
            <el-form label-width="auto" ref="formRef" hide-required-asterisk status-icon :model="form" :rules="rules">
                <el-form-item label="用户名" prop="username">
                    <el-input size="large" v-model="form.username" placeholder="请输入用户名">
                        <template #prefix>
                            <el-icon>
                                <User />
                            </el-icon>
                        </template>
                    </el-input>
                </el-form-item>
                <el-form-item label="密码" prop="password">
                    <el-input size="large" type="password" v-model="form.password" placeholder="请输入密码">
                        <template #prefix>
                            <el-icon>
                                <Lock />
                            </el-icon>
                        </template>
                    </el-input>
                </el-form-item>
                <el-form-item>
                    <el-button size="large" type="primary" @click="handleLogin">登录</el-button>
                    <el-button size="large" @click="handleReset">取消</el-button>
                </el-form-item>
            </el-form>
        </el-card>
    </div>
</template>

<style scoped lang="scss">
.login-container {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;

    .el-card {
        width: 500px;
        height: 340px;
        margin: 0 auto;

        :deep(.el-card__header) {
            text-align: center;
            font-size: 28px;
            font-weight: bold;
            color: #303133;
        }

        .el-form {
            margin-top: 20px;
            padding: 0 20px;

            .el-form-item:nth-child(2) {
                margin-top: 28px;
                margin-bottom: 28px;
            }

            .el-form-item:nth-child(3) {
                :deep(.el-form-item__content) {
                    justify-content: flex-end;
                    gap: 12px;
                }
            }

        }
    }
}
</style>