<script setup>
import { ref } from 'vue'
import Table from './component/Table.vue';
import Tag from './component/Tag.vue';

// 商品列表
const goodsList = ref([
	{
		id: 101,
		picture:
			'https://ts1.tc.mm.bing.net/th/id/R-C.8bbf769b39bb26eefb9b6de51c23851d?rik=crTnc5i8A%2b8p7A&riu=http%3a%2f%2fpicview.iituku.com%2fcontentm%2fzhuanji%2fimg%2f202207%2f09%2fe7196ac159f7cf2b.jpg%2fnu&ehk=DYPLVpoNAXLj5qzwgR5vHf9DladFh%2b34s4UcuP3Kn6E%3d&risl=&pid=ImgRaw&r=0',
		name: '梨⽪朱泥三绝清代⼩品壶经典款紫砂壶',
		tag: '茶具'
	},
	{
		id: 102,
		picture:
			'https://ts1.tc.mm.bing.net/th/id/OIP-C.E6m4s0dlHQhce_kPus5WJAHaEo?w=258&h=211&c=8&rs=1&qlt=90&o=6&pid=3.1&rm=2',
		name: '全防⽔HABU旋钮⽜⽪⼾外徒步鞋⼭宁泰抗菌',
		tag: '男鞋'
	},
	{
		id: 103,
		picture:
			'https://ts1.tc.mm.bing.net/th/id/OIP-C.IggEulKaQSlG5AIZGd16BQHaDt?w=243&h=211&c=8&rs=1&qlt=90&o=6&pid=3.1&rm=2',
		name: '⽑茸茸⼩熊出没，⼉童⽺羔绒背⼼73-90cm',
		tag: '⼉童服饰'
	},
	{
		id: 104,
		picture:
			'https://ts1.tc.mm.bing.net/th/id/OIP-C.8KlnNUYOJudsgoReJE6KsAHaQD?w=160&h=211&c=8&rs=1&qlt=90&o=6&pid=3.1&rm=2',
		name: '基础百搭，⼉童套头针织⽑⾐1-9岁',
		tag: '⼉童服饰'
	}
])
</script>

<template>
	<Table :data="goodsList">
		<template #thead>
			<tr>
				<th>编号</th>
				<th>名称</th>
				<th>图⽚</th>
				<th width="100px">标签</th>
			</tr>
		</template>
		<template #default="{ data }">
			<tr v-for="item, index in data" :key="item.id">
				<td>{{ index + 1 }}</td>
				<td>{{ item.name }}</td>
				<td>
					<img :src="item.picture" />
				</td>
				<td>
					<Tag v-model="item.tag"></Tag>
				</td>
			</tr>
		</template>
	</Table>
</template>

<style lang="scss">
#app {
	width: 1000px;
	margin: 50px auto;

	img {
		width: 100px;
		height: 100px;
		object-fit: contain;
		vertical-align: middle;
	}
}
</style>



<!-- <script setup>
import { ref } from 'vue'
import Table from './component/Table.vue'

const tableData1 = ref([
	{ id: 11, name: '狗蛋', age: 18 },
	{ id: 22, name: '⼤锤', age: 19 },
	{ id: 33, name: '铁棍', age: 17 }
])
const tableData2 = ref([
	{ id: 21, name: 'Jack', age: 18 },
	{ id: 32, name: 'Rose', age: 19 },
	{ id: 43, name: 'Henry', age: 17 }
])
</script>

<template>
	<Table :data="tableData1">
		<template #default="{ index }">
			<button @click="tableData1.splice(index, 1)">删除</button>
		</template>
	</Table>
	<Table :data="tableData2">
		<template #default="{ index }">
			<button @click="tableData2.splice(index, 1)">删除</button>
		</template>
	</Table>
</template>

<style>
body {
	background-color: #fff;
}
</style> -->