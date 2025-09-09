#include "MyFile.h"

MyFile* _fopen(const char* path , const char* mode) {
    MyFile* file = (MyFile*)malloc(sizeof(MyFile));
    if (!file)
        return NULL;
    int flags = 0;
    int iscreate = 0;
    if (!strcmp(mode , "w")) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
        iscreate = 1;   // 需要新建文件
    } else if (!strcmp(mode , "r")) {
        flags = O_RDONLY;
    } else if (!strcmp(mode , "a")) {
        flags = O_CREAT | O_APPEND | O_WRONLY;
        iscreate = 1;
    } // else if () ...

    if (iscreate)
        file->fileno = open(path  , flags , 0666);
    else 
        file->fileno = open(path , flags);
    
    file->flags = flags;
    memset(file->write_buffer , 0 , sizeof(file->write_buffer));
    file->write_pos = 0;
    file->flush_method = LINE_FLUSH;

    return file;
}

void _fclose(MyFile* file) {
    if (file->fileno < 0) return;
    // 关闭文件时，需要刷新缓冲区
    _fflush(file);
    close(file->fileno);    // 关闭文件描述符
    free(file); // 释放空间
}

int _fwrite(MyFile* file , void* str , int len) {
    // 严谨一点应该判断缓冲区是否可以存放下
    // 写入其实就是将数据拷贝至缓冲区
    memcpy(file->write_buffer + file->write_pos , str , len);
    file->write_pos += len;
    ssize_t n = 0;
    if (file->flush_method == LINE_FLUSH && file->write_buffer[file->write_pos - 1] == '\n') {
        // // 满足行刷新
        n = write(file->fileno , file->write_buffer , file->write_pos);
        file->write_pos = 0;
        // 复用
        // _fflush(file);
    }
    return n;
}

void _fflush(MyFile* file) {
    if (file->fileno < 0)
        return;

    ssize_t n = write(file->fileno , file->write_buffer , file->write_pos);
    (void)n;
    file->write_pos = 0;
    fsync(file->fileno); // 将文件缓冲区的内容刷新到磁盘的系统调用`
}