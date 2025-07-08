<script setup>

import { ref, computed, reactive, watch } from 'vue'

const LS_SCORE_LIST_KEY = "score-list-key"

const totalScore = computed(() => {
	return scoreList.value.reduce((prev, item) => {
		return prev + item.score
	}, 0)
})
// 巧妙使用逻辑或，前面为真则短路，前面为假则执行后面的语句
const scoreList = ref(JSON.parse(localStorage.getItem(LS_SCORE_LIST_KEY)) || [])

// 数据持久化，使用watch来监视需要持久化的数据，当发生改变了，把最新的值存储到本地
// 本质上是要监视数组中的每个元素，而不是普通的基本类型，所以要使用深度监视器
watch(scoreList, (newval) => {
	localStorage.setItem(LS_SCORE_LIST_KEY, JSON.stringify(scoreList.value))	// 记住使用ref数据.value要不然很容易bug
}, { deep: true })

const avgScore = computed(() => {
	// 计算属性需要.value
	return scoreList.value.length !== 0 ? Math.round(totalScore.value / scoreList.value.length) : 0
})

const scoreInfo = reactive({
	course: "",
	score: ""
})

const addScoreInfo = () => {
	// 非空校验
	if (!scoreInfo.course || !scoreInfo.score) {
		alert("科目或分数不能为空！")
		return
	}

	scoreList.value.push({
		id: Date.now(),
		subject: scoreInfo.course,
		score: scoreInfo.score
	})
	scoreInfo.course = scoreInfo.score = ""
}

const deleteScoreInfo = (pos) => {
	if (confirm("确认要删除吗？"))
		scoreList.value.splice(pos, 1)
}

</script>
<template>
	<div class="score-case">
		<div class="table">
			<table>
				<thead>
					<tr>
						<th>编号</th>
						<th>科⽬</th>
						<th>成绩</th>
						<th>操作</th>
					</tr>
				</thead>
				<tbody v-if="scoreList.length">
					<tr v-for="item, index in scoreList" :key="item.id">
						<td>{{ index + 1 }}</td>
						<td>{{ item.subject }}</td>
						<td :class="{ red: item.score < 60 }">{{ item.score }}</td>
						<td><a href="#" @click="deleteScoreInfo(index)">删除</a></td>
					</tr>
				</tbody>
				<tbody v-if="!scoreList.length">
					<tr>
						<td colspan="5">
							<span class="none">暂⽆数据</span>
						</td>
					</tr>
				</tbody>
				<tfoot>
					<tr>
						<td colspan="5">
							<span>总分: {{ totalScore }}</span>
							<span style="margin-left: 50px">平均分: {{ avgScore }}</span>
						</td>
					</tr>
				</tfoot>
			</table>
		</div>
		<form class="form">
			<div class="form-item">
				<div class="label">科⽬：</div>
				<div class="input">
					<input type="text" placeholder="请输⼊科⽬" v-model.trim="scoreInfo.course" />
				</div>
			</div>
			<div class="form-item">
				<div class="label">分数：</div>
				<div class="input">
					<input type="number" placeholder="请输⼊分数" v-model.number="scoreInfo.score" />
				</div>
			</div>
			<div class="form-item">
				<div class="label"></div>
				<div class="input">
					<button class="submit" @click.prevent="addScoreInfo">添加</button>
				</div>
			</div>
		</form>
	</div>
</template>

<style scoped>
.score-case {
	width: 1000px;
	margin: 50px auto;
	display: flex;
}

.score-case .table {
	flex: 4;
}

.score-case .table table {
	width: 100%;
	border-spacing: 0;
	border-top: 1px solid #ccc;
	border-left: 1px solid #ccc;
}

.score-case .table table th {
	background: #f5f5f5;
}

.score-case .table table tr:hover td {
	background: #f5f5f5;
}

.score-case .table table td,
.score-case .table table th {
	border-bottom: 1px solid #ccc;
	border-right: 1px solid #ccc;
	text-align: center;
	padding: 10px;
}

.score-case .table table td.red,
.score-case .table table th.red {
	color: red;
}

.score-case .table .none {
	height: 100px;
	line-height: 100px;
	color: #999;
}

.score-case .form {
	flex: 1;
	padding: 20px;
}

.score-case .form .form-item {
	display: flex;
	margin-bottom: 20px;
	align-items: center;
}

.score-case .form .form-item .label {
	width: 60px;
	text-align: right;
	font-size: 14px;
}

.score-case .form .form-item .input {
	flex: 1;
}

.score-case .form .form-item input,
.score-case .form .form-item select {
	appearance: none;
	outline: none;
	border: 1px solid #ccc;
	width: 200px;
	height: 40px;
	box-sizing: border-box;
	padding: 10px;
	color: #666;
}

.score-case .form .form-item input::placeholder {
	color: #666;
}

.score-case .form .form-item .cancel,
.score-case .form .form-item .submit {
	appearance: none;
	outline: none;
	border: 1px solid #ccc;
	border-radius: 4px;
	padding: 4px 10px;
	margin-right: 10px;
	font-size: 12px;
	background: #ccc;
}

.score-case .form .form-item .submit {
	border-color: #069;
	background: #069;
	color: #fff;
}
</style>

<!-- <script setup>
import { ref, computed } from 'vue'
// 计划列表
const planList = ref([
	{ id: 12, name: '跑步', done: false },
	{ id: 76, name: '看书', done: false },
	{ id: 31, name: '撸码', done: false },
	{ id: 49, name: '追剧', done: false }
])

let isAllChecked = computed({
	get() {
		return planList.value.every(item => item.done)
	},
	set(val) {
		planList.value.forEach(item => item.done = val)
	}
})

const reverseChecked = () => {
	planList.value.forEach(item => item.done = !item.done)
}

</script>
<template>
	<p>
		<span>
			<input type="checkbox" id="all" v-model="isAllChecked" />
			<label for="all">全选</label>
		</span>
		<button @click="reverseChecked">反选</button>
	</p>
	<ul>
		<li v-for="item, index in planList" :key="item.id">
			<input type="checkbox" v-model="item.done" @click="item.done = !item.done" />
			<span :class="item.done ? 'completed' : ''">{{ item.name }}</span>
		</li>
	</ul>
</template>

<style lang="scss">
* {
	margin: 0;
	padding: 0;
}

div#app {
	width: 400px;
	margin: 100px auto;
	padding: 15px;
	font-size: 18px;
	background: plum;

	p {
		display: flex;
		justify-content: space-between;
		align-items: center;
		height: 40px;

		button {
			padding: 3px 6px;
		}
	}
}

ul {
	list-style: none;

	li {
		display: flex;
		align-items: center;
		justify-content: space-between;
		height: 40px;
		border-top: 1px solid #ccc;

		span.completed {
			color: #ddd;
			text-decoration: line-through;
		}
	}
}

input {
	margin-right: 8px;
}
</style> -->