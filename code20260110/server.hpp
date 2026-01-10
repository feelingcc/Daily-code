#pragma once
#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

// 简易日志的设计
#define INFO  0
#define DEBUG 1
#define ERROR 2
#define CURRENT_LOG_LEVEL INFO
// ##__VA_ARGS__表示0个或多个参数
#define LOG(level , format , ...) do{ \
    if(level < CURRENT_LOG_LEVEL) break; \
    time_t timestamp = time(nullptr); \
    struct tm* local = localtime(&timestamp); \
    char time_format[1024] = {0}; \
    strftime(time_format , sizeof(time_format) , "%Y-%m-%d %H:%M:%S" , local); \
    fprintf(stdout , "[%s:%s:%d] " format "\n" , time_format , __FILE__ , __LINE__ , ##__VA_ARGS__); \
} while(0)
#define INFO_LOG(format , ...) LOG(INFO , format , ##__VA_ARGS__)
#define DEBUG_LOG(format , ...) LOG(DEBUG , format , ##__VA_ARGS__)
#define ERROR_LOG(format , ...) LOG(ERROR , format , ##__VA_ARGS__)

// 缓冲区类的设计
#define BUFFER_DEFAULT_SIZE 1024
class Buffer{
    char* Begin() { return &*_buffer.begin(); } // 返回缓冲区的起始地址
    public:
        Buffer() :_buffer(BUFFER_DEFAULT_SIZE) , _reader_idx(0) , _writer_idx(0) {}  // 初始化缓冲区，设置读写偏移为0，分配默认大小空间
        Buffer(const Buffer& buf) {}    // 拷贝构造函数
        char* WritePosition() { return Begin() + _writer_idx; }     // 返回当前写入的起始地址
        char* ReadPosition() { return Begin() + _reader_idx; }      // 返回当前读取的起始地址
        uint64_t TailIdleSize() { return _buffer.size() - _writer_idx; } // 获取尾部空闲空间的大小
        uint64_t HeadIdleSize() { return _reader_idx; } // 获取头部空闲空间的大小
        uint64_t ReadableSize() { return _writer_idx - _reader_idx; } // 获取可读空间大小
        void MoveReadOffset(uint64_t len) {
            assert(len <= ReadableSize());
            _reader_idx += len;
        }
        void MoveWriteOffset(uint64_t len) {
            assert(len <= TailIdleSize());
            _writer_idx += len;
        }
        void EnsureWriteSpace(uint64_t len) { // 确保有足够空间写入指定长度数据（自动移动数据或扩容）
            if(len <= TailIdleSize()) {
                // 尾部空闲空间足够
                return;
            } else if(len <= TailIdleSize() + HeadIdleSize()) {
                // 尾部空闲空间 + 头部空闲空间足够，数据整体向前移动，无需扩容
                uint64_t readable_size = ReadableSize();
                std::copy(ReadPosition() , WritePosition() , Begin());
                _reader_idx = 0;
                _writer_idx = readable_size;
            } else {
                // 扩容
                _buffer.resize(_writer_idx + len);
            }
        } 
        void Write(const void* data , uint64_t len) { // 写入数据（不移动写偏移）
            EnsureWriteSpace(len);
            const char* d = (const char*)data;
            std::copy(d , d + len , WritePosition());
        } 
        void WriteAndPush(const void* data , uint64_t len) { // 写入数据并移动写偏移
            Write(data , len);
            MoveWriteOffset(len);
        } 
        void WriteString(const std::string& data) { // 写入字符串（不移动写偏移）
            Write(data.c_str() , data.size());
        }    
        void WriteStringAndPush(const std::string& data) { // 写入字符串并移动写偏移
            WriteString(data);
            MoveWriteOffset(data.size());
        }   
        void Read(void* buf , uint64_t len) { // 读取数据到指定缓冲区（不移动读偏移）
            assert(len <= ReadableSize());
            std::copy(ReadPosition() , ReadPosition() + len , (char*)buf);
        }  
        void ReadAndPop(void* buf , uint64_t len) { // 读取数据到指定缓冲区并移动读偏移
            Read(buf , len);
            MoveReadOffset(len);
        } 
        std::string ReadAsString(uint64_t len) { // 读取数据到string中（不移动读偏移）
            std::string res;
            res.resize(len);
            Read(&res[0] , len);
            return res;
        }  
        std::string ReadAsStringAndPop(uint64_t len) { // 读取数据到string中并移动读偏移
            std::string res = ReadAsString(len);
            MoveReadOffset(len);
            return res;
        } 
        char* FindCRLF() {  // 寻找换行回车符
            return std::find(ReadPosition() , WritePosition() , '\n');
        } 
        std::string GetLine() { // 获取一行数据（包括换行符，不移动读偏移）
            char* res = FindCRLF();
            if(res != WritePosition()) {
                // 找到了换行符
                return ReadAsString(res + 1 - ReadPosition());
            }
            return "";
        }   
        std::string GetLineAndPop() { // 获取一行数据（包括换行符）并移动读偏移
            std::string res = GetLine();
            MoveWriteOffset(res.size());
            return res;
        } 
        void Clear() { _reader_idx = _writer_idx = 0; }     // 清理接口
    private:
        std::vector<char> _buffer; 
        uint64_t _reader_idx;  
        uint64_t _writer_idx;
};

