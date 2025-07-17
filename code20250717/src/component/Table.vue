<script setup>
defineProps({
    data: {
        type: Array,
        default: () => []
    }
})
</script>
<template>
    <table class="my-table">
        <thead>
            <slot name="thead"></slot>
        </thead>
        <tbody>
            <slot :data="data"></slot>
        </tbody>
    </table>
</template>

<style lang="scss">
.my-table {
    width: 100%;
    border-spacing: 0;

    img {
        width: 100px;
        height: 100px;
        object-fit: contain;
        vertical-align: middle;
    }

    th {
        background: #f5f5f5;
        border-bottom: 2px solid #069;
    }

    td {
        border-bottom: 1px dashed #ccc;
    }

    td,
    th {
        text-align: center;
        padding: 10px;
        transition: all .5s;

        &.red {
            color: red;
        }
    }

    .none {
        height: 100px;
        line-height: 100px;
        color: #999;
    }
}
</style>







<!--  
<script setup>
// defineProps({
//     data: {
//         type: Array,
//         default: () => []
//     }
// });

// </script>
// <template>
    // <table class="my-table">
        // <thead>
            // <tr>
                // <th>序号</th>
                // <th>姓名</th>
                // <th>年纪</th>
                // <th>操作</th>
                // </tr>
            // </thead>
        // <tbody>
            // <tr v-for="item, index in data" :key="item.id">
                // <td>{{ index + 1 }}</td>
                // <td>{{ item.name }}</td>
                // <td>{{ item.age }}</td>
                // <td>
                    // <slot :index="index"></slot>
                    // </td>
                // </tr>
            // </tbody>
        // </table>
    // </template>
//
<style>
// .my-table {
//     width: 450px;
//     text-align: center;
//     border: 1px solid #ccc;
//     font-size: 24px;
//     margin: 30px auto;
// }

// .my-table thead {
//     background-color: #1f74ff;
//     color: #fff;
// }

// .my-table thead th {
//     font-weight: normal;
// }

// .my-table thead tr {
//     line-height: 40px;
// }

// .my-table th,
// .my-table td {
//     border-bottom: 1px solid #ccc;
//     border-right: 1px solid #ccc;
// }

// .my-table td:last-child {
//     border-right: none;
// }

// .my-table tr:last-child td {
//     border-bottom: none;
// }

// .my-table button {
//     width: 65px;
//     height: 35px;
//     font-size: 18px;
//     border: 1px solid #ccc;
//     outline: none;
//     border-radius: 3px;
//     cursor: pointer;
//     background-color: #ffffff;
//     margin-left: 5px;
// }
// </style>
-->