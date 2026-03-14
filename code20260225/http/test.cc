#include "http.hpp"

static size_t split(const std::string& src , const std::string& sep , std::vector<std::string>* res) {
    size_t offset = 0;
    while(offset < src.size()) {
        size_t pos = src.find(sep , offset);
        if(pos == std::string::npos) {
            res->push_back(src.substr(offset));
            return res->size();
        }
        // 防止添加空串
        if(pos > offset) {
            res->push_back(src.substr(offset , pos - offset));
        }
        offset = pos + sep.size();
    }
    return res->size();
}

static bool readFile(const std::string& filename , std::string* res) {
    std::ifstream ifs(filename , std::ios::binary | std::ios::in);
    if(!ifs.is_open()) {
        ERROR_LOG("open %s error" , filename.c_str());
        return false;
    }
    // 获取文件大小
    ifs.seekg(0 , std::ios::end);
    size_t fsize = ifs.tellg();
    ifs.seekg(0 , std::ios::beg); // 0表示偏移量，从开头偏移0个字节
    res->resize(fsize);
    ifs.read(res->data() , fsize);
    return true;
}

int main () {

    std::cout << Util::isVaildPath("/index/../index.html") << std::endl;
    std::cout << Util::isVaildPath("/index/../index/../../a.cc") << std::endl;
    std::cout << Util::isVaildPath("../a/index.html") << std::endl;


    // std::cout << Util::isDirectory("./testdir") << std::endl;
    // std::cout << Util::isDirectory("./test.cc") << std::endl;
    // std::cout << Util::isRegularFile("./testdir") << std::endl;
    // std::cout << Util::isRegularFile("./test.cc") << std::endl;

    // std::cout << Util::statuDesc(404) << std::endl;
    // std::cout << Util::statuDesc(201) << std::endl;
    // std::cout << Util::statuDesc(200) << std::endl;
    // std::cout << Util::statuDesc(800) << std::endl;

    // std::string str = "c  ";
    // std::string res = Util::urlencode(str , true);
    // std::cout << res << std::endl;
    // res = Util::urldecode(res , true);
    // std::cout << res << std::endl;

    // std::string str = "c++";
    // std::string res = Util::urlencode(str , false);
    // std::cout << res << std::endl;
    // res = Util::urldecode(res , false);
    // std::cout << res << std::endl;

    // 使用 md5sum filename 对文件内容进行摘要判断是否读取和写入成功
    // std::string res;
    // if(!Util::readFile("./http.hpp" , &res)) {
    //     return -1;
    // }
    // // std::cout << "read e" << std::endl;
    // if(!Util::writeFile("./testWrite.hpp" , res)) {
    //     return -1;
    // }

    // std::string test = "abc,,,def";
    // std::vector<std::string> res;
    // split(test , "," , &res);
    // for(auto& val : res)
    //     std::cout << "[" << val << "]" << std::endl;


    return 0;
}