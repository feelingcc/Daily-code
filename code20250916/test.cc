#include <iostream>
#include <fstream>
#include <string>

struct StuInfo {
    char name[20];
    int age;
    char gender;
};

int main() {
    const std::string filename = "log.txt";
    
    // 先创建并写入文件
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing" << std::endl;
        return 1;
    }
    
    StuInfo stu1 = {"Jack", 18, 'm'};
    outFile.write(reinterpret_cast<char*>(&stu1), sizeof(stu1));
    std::cout << "Write pos after writing: " << outFile.tellp() << std::endl; // 28
    outFile.close();
    
    
    // 再打开文件进行读取
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file for reading" << std::endl;
        return 1;
    }
    
    std::cout << "Read pos initially: " << inFile.tellg() << std::endl; // 0
        
    // 读取数据
    StuInfo readStu;
    inFile.read(reinterpret_cast<char*>(&readStu), sizeof(readStu));
    std::cout << "Read: " << readStu.name << ", " << readStu.age << ", " << readStu.gender << std::endl;
    
    inFile.close();
    
    
    return 0;
}