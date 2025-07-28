<script setup>
import { Document, Edit } from '@element-plus/icons-vue'
import { useRoute } from 'vue-router'
const route = useRoute()
</script>

<template>
    <div class="common-layout">
        <el-container>
            <el-aside>

                <h1 class="logo">
                    <img src="../../assets/logo.webp" alt="logo" width="60" height="60">
                </h1>
                <el-menu :default-active="route.path" router active-text-color="#ffd04b" background-color="#232323"
                    text-color="#fff">
                    <el-menu-item index="/dashboard">
                        <el-icon>
                            <Document />
                        </el-icon>
                        <span>数据看报</span>
                    </el-menu-item>
                    <el-menu-item index="/article">
                        <el-icon>
                            <Edit />
                        </el-icon>
                        <span>文章管理</span>
                    </el-menu-item>
                </el-menu>

            </el-aside>
            <el-container>
                <el-header>Header</el-header>
                <el-main>
                    <!-- 二级路由出口 -->
                    <router-view />
                </el-main>
            </el-container>
        </el-container>
    </div>
</template>

<style scoped lang="scss">
.common-layout {
    height: 100vh;

    .el-aside {
        width: 200px;
        height: 100vh;
        background-color: #232323;
        border-right: 1px solid #e6e6e6;
        overflow: hidden; // 防止出现滚动条

        .el-menu {
            width: 200px;
        }

        .logo {
            width: 200px;
            display: flex;
            justify-content: center;
            align-items: center;
            border-right: 1px solid #e6e6e6;
            height: 80px; // 稍微减小logo高度
            background-color: #232323;
            color: #fff;
            text-align: center;
            line-height: 80px; // 对应调整行高
            font-size: 20px;
        }

        // 修改菜单 hover 颜色和居中
        :deep(.el-menu) {
            .el-menu-item {
                margin: 0 0 0 -16px;
                display: flex;
                justify-content: center;
                align-items: center;
                text-align: center;

                .el-icon {
                    margin-right: 8px;
                }
            }

            .el-menu-item:hover {
                background-color: #ffd04b !important;
                color: #232323 !important;
            }
        }
    }
}
</style>