#include "server.hpp"

void test_buffer();

int main() {

    // test_buffer();
    
    return 0;
}

void test_buffer() {
    Buffer buffer;
    int i = 300;
    while(i--) {
        std::string msg = "hello world" + std::to_string(i) + "\n";
        buffer.WriteStringAndPush(msg);
    }
    // std::string str = buffer.ReadAsString(buffer.ReadableSize());
    // std::cout << str << std::endl;

    std::string line = buffer.GetLineAndPop();
    std::cout << line << std::endl;
    line = buffer.GetLineAndPop();
    std::cout << line << std::endl;

    Buffer b(buffer);
    std::string str = b.ReadAsString(b.ReadableSize());
    std::cout << str << std::endl;

    // while(true) {
    //     std::string line = buffer.GetLineAndPop();
    //     if(line.empty()) break;
    //     std::cout << line << std::endl;
    // }
}