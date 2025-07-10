<script setup>
import Item from './component/Item.vue';
// 提供数据
const goodsObj = {
	id: '3998274',
	name: '德国百年⼯艺⾼端⽔晶玻璃红酒杯2⽀装',
	price: 139,
	picture:
		'https://yanxuan-item.nosdn.127.net/8896b897b3ec6639bbd1134d66b9715c.jpg'
}
</script>

<template>
	<div class="list">
		<item :goodsObjPircture="goodsObj.picture" :goodsObjName="goodsObj.name" :goodsObjPrice="goodsObj.price"></item>
		<!-- <item></item> -->
	</div>
</template>

<style lang="scss">
* {
	margin: 0;
	padding: 0;
	box-sizing: border-box;
}

.list {
	width: 1000px;
	margin: 0 auto;
	display: flex;
	flex-wrap: wrap;
}
</style>