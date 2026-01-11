#pragma once
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

// 简易日志的设计
#define INFO  0
#define DEBUG 1
#define ERROR 2
#define CURRENT_LOG_LEVEL DEBUG
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
    public:
        Buffer() :_buffer(BUFFER_DEFAULT_SIZE) , _reader_idx(0) , _writer_idx(0) {}  // 初始化缓冲区，设置读写偏移为0，分配默认大小空间
        // 拷贝构造函数
        Buffer(const Buffer& buf) 
            :_buffer(buf._buffer.begin() , buf._buffer.end()) 
            ,_reader_idx(buf._reader_idx)
            ,_writer_idx(buf._writer_idx)
        {}   
        char* Begin() { return &*_buffer.begin(); } // 返回缓冲区的起始地址
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
                DEBUG_LOG("resize %ld" , _writer_idx + len);
                _buffer.resize(_writer_idx + len);
            }
        } 
        void Write(const void* data , uint64_t len) { // 写入数据（不移动写偏移）
            if(len == 0) return;
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
            assert(len <= ReadableSize());
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
            MoveReadOffset(res.size());
            return res;
        } 
        void Clear() { _reader_idx = _writer_idx = 0; }     // 清理接口
    private:
        std::vector<char> _buffer; 
        uint64_t _reader_idx;  
        uint64_t _writer_idx;
};

// 套接字类的设计
#define DEFAULT_BACKLOG 1024
class Socket{
    public:
        Socket() :_sockfd(-1) {}
        Socket(int fd) :_sockfd(fd) {}
        ~Socket() { Close(); }
        bool Create() {
            _sockfd = socket(AF_INET , SOCK_STREAM , 0);
            if(_sockfd < 0) {
                ERROR_LOG("create socket failed");
                return false;
            }
            return true;
        }
        bool Bind(const std::string& ip , uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            int res = bind(_sockfd , (struct sockaddr*)&addr , sizeof(addr));
            if(res < 0) {
                ERROR_LOG("bind server failed");
                return false;
            }
            return true;
        }
        bool Listen(int backlog = DEFAULT_BACKLOG) {
            int res = listen(_sockfd , backlog);
            if(res < 0) {
                ERROR_LOG("listen server failed");
                return false;
            }
            return true;
        }
        int Accept() {
            return accept(_sockfd , nullptr , nullptr);
        }
        bool Connect(const std::string& ip , uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            int res = connect(_sockfd , (struct sockaddr*)&addr , sizeof(addr));
            if(res < 0) {
                ERROR_LOG("connect server failed");
                return false;
            }
            return true;
        }
        ssize_t Recv(void* buf , size_t len , int flags = 0) {
            ssize_t n = recv(_sockfd , buf , len , flags);
            if(n <= 0) {
                if(errno == EAGAIN || errno == EINTR) {
                    return 0;   // 不算错误
                }
                // 读取错误或对方关闭连接都算错误
                return -1;
            }
            return n;
        }
        ssize_t Send(const void* buf , size_t len , int flags = 0) {
            return send(_sockfd , buf , len , flags);
        }
        void Close() {
            if(_sockfd > 0) close(_sockfd);
        }
        bool CreateServer(uint16_t port , const std::string& ip = "0.0.0.0") {
            if(!Create()) return false;
            if(!SetNonBlock()) return false;
            SetReuseAddress();
            if(!Bind(ip , port)) return false;
            if(!Listen()) return false;
            return true;
        }
        bool CreateClient(uint16_t port , const std::string ip) {
            if(!Create()) return false;
            // 客户端不需要显示的bind
            if(!Connect(ip , port)) return false;
            return true;
        }
        bool SetNonBlock() {
            int flag = fcntl(_sockfd , F_GETFL);
            if(flag < 0) {
                ERROR_LOG("set none block failed");
                return false;
            }
            fcntl(_sockfd , F_SETFL , flag | O_NONBLOCK);
            return true;
        }
        void SetReuseAddress() {
            int opt = 1;
            int res = setsockopt(_sockfd , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt));
            if(res < 0) {
                ERROR_LOG("set reuse address failed");
                return;
            }
            res = setsockopt(_sockfd , SOL_SOCKET , SO_REUSEPORT , &opt , sizeof(opt));
            if(res < 0) {
                ERROR_LOG("set reuse address failed");
                return;
            }
        }
    private:
        int _sockfd;
};