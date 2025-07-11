#include <stdio.h>

struct list_head {
    struct list_head * next , *prev;
};

// 模拟Linux PCB数据结构设计
struct task_struct {
    int pid;
    int ppid;
    int status;
    // ...
    struct list_head process_links; // 进程管理的双链表
    // ...
    int x;
    int y;
    // ...
    struct list_head run_queue; // CPU运行队列
    // ...
};

int main () {
    // 初始化3个进程服务
    struct task_struct proc1 = {1 , 1 , 1 , {NULL , NULL} , 1 , 1 , {NULL , NULL}};
    struct task_struct proc2 = {2 , 2 , 2 , {NULL , NULL} , 2 , 2 , {NULL , NULL}};
    struct task_struct proc3 = {3 , 3 , 3 , {NULL , NULL} , 3 , 3 , {NULL , NULL}};
    // 连接
    proc1.process_links.next = &proc2.process_links;
    proc2.process_links.prev = &proc1.process_links;

    proc2.process_links.next = &proc3.process_links;
    proc3.process_links.prev = &proc2.process_links;

    proc1.process_links.prev = &proc3.process_links;
    proc3.process_links.next = &proc1.process_links;

    // 定义进程双向链表的头节点
    struct list_head* process_list_head; 
    process_list_head = &proc1.process_links;

    size_t offset = (size_t)(&((struct task_struct*)0)->process_links);   // 获得process_links成员相较于结构体对象起始地址的偏移量
    printf("%ld\n" , offset); 
    int proc2_pid = ((struct task_struct*)((char*)(process_list_head->next) - offset))->pid;
    printf("%d\n" , proc2_pid); 

    printf("x = %d\n" , ((struct task_struct*)((char*)(process_list_head->next) - (size_t)(&((struct task_struct*)0)->process_links)))->pid  );

    // (&proc1.process_links)->next == &proc2.process_links

    // (((struct task_struct*)0)->process_links) // 获得process_links成员相较于结构体对象起始地址的偏移量
    // &proc2.process_links - (&((struct task_struct*)0)->process_links) // 使用当前结构体对象的process_links成员的地址 - 偏移量 = 当前结构体起始地址 这样就可以访问其他的结构体对象成员了
    // (&proc2.process_links - (&((struct task_struct*)0)->process_links))->pid;
    // (&proc2.process_links - (&((struct task_struct*)0)->process_links))->x;
    // (&((struct task_struct*)0)->process_links)


    // ((struct task_struct*)((process_list_head->next - &((struct task_struct*)0)->process_links)))->pid;
    // printf("%p\n%p\n%p\n" , proc1.process_links.next , process_list_head->next , &proc2.process_links);

    // printf("%p\n%p\n" , &proc2.process_links , process_list_head->next);
    // printf("-------------------------------------------------------------\n");
    // printf("%ld\n" , process_list_head->next - (size_t)(&((struct task_struct*)0)->process_links));
    // printf("%ld\n" , (&proc2.process_links - &((struct task_struct*)0)->process_links));


    // printf("proc2：pid：%d , x = %d" , (&proc2.process_links - (&((struct task_struct*)0)->process_links))->pid);

    // printf("%d" , );

    // 定义运行队列双向链表的头节点
    struct list_head* run_queue_head;
    run_queue_head = &proc1.run_queue;

    
    

    return 0;
}