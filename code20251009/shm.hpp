#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "comm.hpp"


const std::string default_path = ".";
const int default_proj_id = 0x66;
const int default_id = -1;
const int default_size = 4096;
const int default_mode = 0666;

#define CREATER "creater"
#define USER "user"

// 封装共享内存
class IpcShm {
    private:
        void _creater(int flag) {
            _shmid = shmget(_key , _shmsize , flag);
            if (_shmid < 0) {
                ERR_EXIT("shmget");
            }
            std::cout << "shmget success - shmid: " << _shmid << std::endl;
        }

        // server
        void create() {
            _creater(IPC_CREAT | IPC_EXCL | default_mode);
        }

        // client
        void get() {
            _creater(IPC_CREAT);
        }

        void attach() {
            _shmaddr = shmat(_shmid , nullptr , 0);
            if (reinterpret_cast<long long>(_shmaddr) < 0) {
                ERR_EXIT("shmat");
            }
        }

        void detach() {
            int n = shmdt(_shmaddr);
            if (n < 0) {
                ERR_EXIT("shmdt");
            }
            std::cout << "shmdt success" << std::endl;
        }

        void destroy() {
            if (_shmid == default_id)
                return;

            detach();
            if (_type == CREATER) {
                 int n = shmctl(_shmid , IPC_RMID , nullptr);
                if (n < 0) {
                    ERR_EXIT("shmctl");
                }
                std::cout << "destroy shm success - shmid: " << _shmid << std::endl;
            }
        }
    public:
        IpcShm(const std::string& type = CREATER , const std::string& pathname = default_path , const int proj_id = default_proj_id , const int size = default_size)
            : _shmid(default_id) , _shmsize(size) , _key(-1) , _shmaddr(nullptr) , _type(type)
        {
            // ftok 用于在 System V IPC 中生成唯一的key
            _key = ftok(pathname.c_str() , proj_id);
            if (_key < 0) {
                ERR_EXIT("ftok");
            }
            std::cout << "ftok success - key: " << _key << std::endl; 

            if(_type == CREATER) {
                create();
            } else if (_type == USER) {
                get();
            } else {
                std::cerr << "type error" << std::endl;
                exit(EXIT_FAILURE);
            }
            attach();
        }

        ~IpcShm() {
            destroy();
        }

        void* getVirtualAddr() { 
            std::cout << "_shmaddr: " << _shmaddr << std::endl;
            return  _shmaddr;
        }

        size_t getSize() {
            return _shmsize;
        }

        void getAttr() {
            struct shmid_ds ds;
            int n = shmctl(_shmid , IPC_STAT , &ds);
            printf("shm_segsz: %ld\n", ds.shm_segsz);
            printf("key: 0x%x\n", ds.shm_perm.__key);
            // ...
        }

    private:
        int _shmid;
        size_t _shmsize;
        key_t _key;
        void* _shmaddr;
        const std::string _type;
};
