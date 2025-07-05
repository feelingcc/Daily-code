<script setup>
import { ref } from 'vue'

let isDisplay = ref(true)

</script>

<template>
	<div class="wrap">
		<h3>可折叠⾯板</h3>
		<div class="panel">
			<div class="title">
				<h4>⾃由与爱情</h4>
				<span class="btn" @click="isDisplay = !isDisplay"> {{ isDisplay ? "收起" : "展开" }} </span>
			</div>
			<div class="container" v-show="isDisplay">
				<p>⽣命诚可贵,</p>
				<p>爱情价更⾼。</p>
				<p>若为⾃由故，</p>
				<p>两者皆可抛。</p>
			</div>
		</div>
	</div>
</template>

<style scoped lang="scss">
.wrap {
	width: 400px;
	margin: 20px auto;
	padding: 1em 2em 2em;
	border: 4px solid green;
	border-radius: 1em;
	box-shadow: 3px 3px 3px rgba(0, 0, 0, 0.5);
	background-color: #fff;

	h3 {
		text-align: center;
	}

	.panel {
		.title {
			display: flex;
			justify-content: space-between;
			align-items: center;
			padding: 0 1em;
			border: 1px solid #ccc;

			h4 {
				line-height: 2;
				margin: 0;
			}
		}

		.container {
			border: 1px solid #ccc;
			border-top: 1px solid transparent;
			padding: 0 1em;
		}

		.btn {
			/* ⿏标改成⼿的形状 */
			cursor: pointer;
		}
	}
}
</style>

<!-- <script setup>
import { ref } from 'vue'
const imgList = [
	'https://cxk-1305128831.cos.ap-beijing.myqcloud.com/11-00.gif',
	'https://cxk-1305128831.cos.ap-beijing.myqcloud.com/11-01.gif',
	'https://cxk-1305128831.cos.ap-beijing.myqcloud.com/11-02.gif',
	'https://cxk-1305128831.cos.ap-beijing.myqcloud.com/11-03.gif',
	'https://cxk-1305128831.cos.ap-beijing.myqcloud.com/11-04.png',
	'https://cxk-1305128831.cos.ap-beijing.myqcloud.com/11-05.png'
]
let index = ref(0)

const next = () => {
	index.value++
}

const prev = () => {
	index.value--
}

</script>

<template>
	<div class="container">
		<div v-show="index !== 0">
			<button @click="prev">上一页</button>
		</div>
		<img :src="imgList[index]" alt="">
		<div v-show="index !== imgList.length - 1">
			<button @click="next">下一页</button>
		</div>
	</div>
</template>

<style scoped>
.container {
	display: flex;
	width: 540px;
}

img {
	width: 240px;
	height: 240px;
}

.container div {
	flex: 1;
	display: flex;
	justify-content: center;
	align-items: center;
}
</style> -->
