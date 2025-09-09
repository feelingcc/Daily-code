#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BUFFER_SIZE 1024    // 缓冲区大小

#define NONE_FLUSH (1<<0)
#define LINE_FLUSH (1<<1)
#define FULL_FLUSH (1<<2)

typedef struct IO_FILE {
    int fileno;   // 文件描述符
    int flags;    // 操作文件的方式
    char write_buffer[BUFFER_SIZE];   // 写入缓冲区
    int write_pos;                    // 当前写入位置
    int flush_method;                 // 刷新方式
}MyFile;

MyFile* _fopen(const char* path , const char* mode);
void _fclose(MyFile* file);
int _fwrite(MyFile* file , void* str , int len);
void _fflush(MyFile* file);