#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>

#define ERR_EXIT(errorstr)   \
    do {                     \
        perror(errorstr);    \
        exit(EXIT_FAILURE);  \
    } while(0)

const std::string path = ".";
const int proj_id = 0x66;
const int default_id = -1;
const int default_size = 4096;
const int default_mode = 0666;

// 封装共享内存
class IpcShm {
    private:
        void _creater(int flag) {
            // ftok 用于在 System V IPC 中生成唯一的key
            key_t key = ftok(path.c_str() , proj_id);
            if (key < 0) {
                ERR_EXIT("ftok");
            }
            std::cout << "ftok success - key: " << key << std::endl; 
            _shmid = shmget(key , _shmsize , flag);
            if (_shmid < 0) {
                ERR_EXIT("shmget");
            }
            std::cout << "shmget success - shmid: " << _shmid << std::endl;
        }
    public:
        IpcShm(const int size = default_size)
            : _shmid(default_id) , _shmsize(size) , _shmaddr(nullptr)
        {}

        // server
        void create() {
            _creater(IPC_CREAT | IPC_EXCL | default_mode);
        }

        // client
        void get() {
            _creater(IPC_CREAT);
        }

        void* attach() {
            _shmaddr = shmat(_shmid , nullptr , 0);
            if (reinterpret_cast<long long>(_shmaddr) < 0) {
                ERR_EXIT("shmat");
            }
            std::cout << "_shmaddr: " << _shmaddr << std::endl;
            return _shmaddr;
        }

        void destroy() {
            int n = shmctl(_shmid , IPC_RMID , nullptr);
            if (n < 0) {
                ERR_EXIT("shmctl");
            }
            std::cout << "destroy shm success - shmid: " << _shmid << std::endl;
        }
    private:
        int _shmid;
        int _shmsize;
        void* _shmaddr;
};
