#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

int main () {

    // 1. 创建并打开文件
    ofstream file("log.txt", ios::out);
    
    // 2. 判断是否成功打开
    if (!file) {
        cout << "无法打开文件!" << endl;
        return -1;
    }
    // 3. 写入各种数据
    file.put('H').put('i').put('\n'); // 链式调用put

    file << "hello file stream\n";
        
    // 4. 移动位置并覆盖数据
    file.seekp(0); // 回到开头
    file << "OVERWRITE"; // 覆盖前9个字符
    
    // 5. 追加更多数据
    file.seekp(0, ios::end); // 移动到末尾
    file << "###追加的数据###";
    
    // 6. 关闭文件（析构函数也会自动调用）
    file.close();

    return 0;
}

// int main () {

//     std::ofstream outFile("log.txt" , std::ios::out);  // 不存在则创建
//     if (!outFile) {
//         std::cout << "打开文件失败" << std::endl;
//         return -1;
//     }

//     outFile << "xxxxx xxxxx\n";

//     outFile.close();

//     return 0;
// }

// int main () {

//     ofstream file("app_example.txt", ios::app);
    
//     file << "第一行文字" << endl;
//     file << "第二行文字" << endl;
    
//     // 尝试移动指针并写入 - 但会被强制回到末尾
//     file.seekp(0, ios::beg);
//     file << "尝试在开头写入 - 但实际会追加到末尾" << endl;
    
//     file.close();

//     return 0;
// }

// int main () {

//     fstream file("log.txt", ios::out);
    
//     file << "xxxxx1" << endl;
//     file << "xxxxx2" << endl;
    
//     // 可以自由移动到任何位置
//     file.seekp(3, ios::beg);
//     file << "yyy" << endl;

//     file.close();

//     return 0;
// }

// int main ()  {

    // std::string token;
    // std::stringstream ss("apple banana cherry");
    // while (ss >> token) {
    //     std::cout << token << std::endl;
    // }

    // std::stringstream ss("apple banana");
    // std::string token;
    // ss >> token;
    // std::cout << token << std::endl;        // apple
    // std::cout << ss.fail() << std::endl;    // 0
    // std::cout << "---------------------" << std::endl;
    // ss >> token;
    // std::cout << token << std::endl;        // banana
    // std::cout << ss.fail() << std::endl;    // 0
    // std::cout << "---------------------" << std::endl;
    // ss >> token;
    // std::cout << token << std::endl;        // 读取的还是上次旧值：banana
    // std::cout << ss.fail() << std::endl;    // 1
    // std::cout << "---------------------" << std::endl;

    // // 清空重新使用
    // ss.clear();
    // ss.str("grape");
    // ss >> token;
    // std::cout << token << std::endl;        // grape
    // std::cout << ss.fail() << std::endl;    // 0


//     return 0;
// }