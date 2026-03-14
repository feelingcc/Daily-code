#include "../server.hpp"

class Util {
    public:
        // 字符串分割函数，将 src 字符串按照 sep 字符分割，最后放到 res 数组中，最终返回子串的数量
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
        // 读取文件的所有内容，将读取的内容放到一个 string 中
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
            if(!ifs.good()) {
                ERROR_LOG("read %s error" , filename.c_str());
                return false;
            }
            return true;
        }
        // 向文件写入内容
        static bool writeFile(const std::string& filename , const std::string& buf) {
            std::ofstream ofs(filename , std::ios::out | std::ios::binary | std::ios::trunc);
            if(!ofs.is_open()) {
                ERROR_LOG("open %s error" , filename.c_str());
                return false;
            }
            ofs.write(buf.data() , buf.size());
            if(!ofs.good()) {
                ERROR_LOG("write %s error" , filename.c_str());
                return false;
            }
            return true;
        }
};