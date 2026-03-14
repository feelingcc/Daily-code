#include "../server.hpp"

std::unordered_map<int , std::string> statu_desc_hash = {
    {100,  "Continue"},
    {101,  "Switching Protocol"},
    {102,  "Processing"},
    {103,  "Early Hints"},
    {200,  "OK"},
    {201,  "Created"},
    {202,  "Accepted"},
    {203,  "Non-Authoritative Information"},
    {204,  "No Content"},
    {205,  "Reset Content"},
    {206,  "Partial Content"},
    {207,  "Multi-Status"},
    {208,  "Already Reported"},
    {226,  "IM Used"},
    {300,  "Multiple Choice"},
    {301,  "Moved Permanently"},
    {302,  "Found"},
    {303,  "See Other"},
    {304,  "Not Modified"},
    {305,  "Use Proxy"},
    {306,  "unused"},
    {307,  "Temporary Redirect"},
    {308,  "Permanent Redirect"},
    {400,  "Bad Request"},
    {401,  "Unauthorized"},
    {402,  "Payment Required"},
    {403,  "Forbidden"},
    {404,  "Not Found"},
    {405,  "Method Not Allowed"},
    {406,  "Not Acceptable"},
    {407,  "Proxy Authentication Required"},
    {408,  "Request Timeout"},
    {409,  "Conflict"},
    {410,  "Gone"},
    {411,  "Length Required"},
    {412,  "Precondition Failed"},
    {413,  "Payload Too Large"},
    {414,  "URI Too Long"},
    {415,  "Unsupported Media Type"},
    {416,  "Range Not Satisfiable"},
    {417,  "Expectation Failed"},
    {418,  "I'm a teapot"},
    {421,  "Misdirected Request"},
    {422,  "Unprocessable Entity"},
    {423,  "Locked"},
    {424,  "Failed Dependency"},
    {425,  "Too Early"},
    {426,  "Upgrade Required"},
    {428,  "Precondition Required"},
    {429,  "Too Many Requests"},
    {431,  "Request Header Fields Too Large"},
    {451,  "Unavailable For Legal Reasons"},
    {501,  "Not Implemented"},
    {502,  "Bad Gateway"},
    {503,  "Service Unavailable"},
    {504,  "Gateway Timeout"},
    {505,  "HTTP Version Not Supported"},
    {506,  "Variant Also Negotiates"},
    {507,  "Insufficient Storage"},
    {508,  "Loop Detected"},
    {510,  "Not Extended"},
    {511,  "Network Authentication Required"}
};

std::unordered_map<std::string, std::string> mime_hash = {
    {".aac",        "audio/aac"},
    {".abw",        "application/x-abiword"},
    {".arc",        "application/x-freearc"},
    {".avi",        "video/x-msvideo"},
    {".azw",        "application/vnd.amazon.ebook"},
    {".bin",        "application/octet-stream"},
    {".bmp",        "image/bmp"},
    {".bz",         "application/x-bzip"},
    {".bz2",        "application/x-bzip2"},
    {".csh",        "application/x-csh"},
    {".css",        "text/css"},
    {".csv",        "text/csv"},
    {".doc",        "application/msword"},
    {".docx",       "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {".eot",        "application/vnd.ms-fontobject"},
    {".epub",       "application/epub+zip"},
    {".gif",        "image/gif"},
    {".htm",        "text/html"},
    {".html",       "text/html"},
    {".ico",        "image/vnd.microsoft.icon"},
    {".ics",        "text/calendar"},
    {".jar",        "application/java-archive"},
    {".jpeg",       "image/jpeg"},
    {".jpg",        "image/jpeg"},
    {".js",         "text/javascript"},
    {".json",       "application/json"},
    {".jsonld",     "application/ld+json"},
    {".mid",        "audio/midi"},
    {".midi",       "audio/x-midi"},
    {".mjs",        "text/javascript"},
    {".mp3",        "audio/mpeg"},
    {".mpeg",       "video/mpeg"},
    {".mpkg",       "application/vnd.apple.installer+xml"},
    {".odp",        "application/vnd.oasis.opendocument.presentation"},
    {".ods",        "application/vnd.oasis.opendocument.spreadsheet"},
    {".odt",        "application/vnd.oasis.opendocument.text"},
    {".oga",        "audio/ogg"},
    {".ogv",        "video/ogg"},
    {".ogx",        "application/ogg"},
    {".otf",        "font/otf"},
    {".png",        "image/png"},
    {".pdf",        "application/pdf"},
    {".ppt",        "application/vnd.ms-powerpoint"},
    {".pptx",       "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
    {".rar",        "application/x-rar-compressed"},
    {".rtf",        "application/rtf"},
    {".sh",         "application/x-sh"},
    {".svg",        "image/svg+xml"},
    {".swf",        "application/x-shockwave-flash"},
    {".tar",        "application/x-tar"},
    {".tif",        "image/tiff"},
    {".tiff",       "image/tiff"},
    {".ttf",        "font/ttf"},
    {".txt",        "text/plain"},
    {".vsd",        "application/vnd.visio"},
    {".wav",        "audio/wav"},
    {".weba",       "audio/webm"},
    {".webm",       "video/webm"},
    {".webp",       "image/webp"},
    {".woff",       "font/woff"},
    {".woff2",      "font/woff2"},
    {".xhtml",      "application/xhtml+xml"},
    {".xls",        "application/vnd.ms-excel"},
    {".xlsx",       "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".xml",        "application/xml"},
    {".xul",        "application/vnd.mozilla.xul+xml"},
    {".zip",        "application/zip"},
    {".3gp",        "video/3gpp"},
    {".3g2",        "video/3gpp2"},
    {".7z",         "application/x-7z-compressed"}
};

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
        // URL编码：避免URL中资源路径与查询字符串中的特殊字符与HTTP请求中特殊字符产生歧
        // 编码格式：将需要编码的字符转换为 % + 两个16进制的数字
        // 不编码的特殊字符： RFC3986文档规定 . - _ ~ 字母，数字属于绝对不编码字符
        // W3C标准中规定，查询字符串中的空格，需要编码为+， 解码则是+转空格
        static std::string urlencode(const std::string& url , bool convert_space_to_plus) {
            std::string res;
            for(auto c : url) {
                if(c == '.' || c == '-' || c == '_' || c == '~' || isalnum(c)) {
                    res += c;
                    continue;
                }
                if(c == ' ' && convert_space_to_plus) {
                    res += '+';
                    continue;
                }
                // 需要编码的字符
                char temp[4] = {0};
                snprintf(temp , 4 , "%%%02X" , c);
                res += temp;
            }
            return res;
        }
        // URL解码
        static std::string urldecode(const std::string& url , bool convert_plus_to_space) {
            std::string res;
            for(size_t i = 0; i < url.size(); i++) {
                if(url[i] == '%' && i + 2 < url.size()) {
                    char c = hexToIntFilter(url[i + 1]) * 16 + hexToIntFilter(url[i + 2]);
                    res += c;
                    i += 2;
                    continue;
                }
                if(url[i] == '+' && convert_plus_to_space) {
                    res += ' ';
                    continue;
                }
                res += url[i];
            }
            return res;
        }
        // 根据相应状态获取描述信息
        static std::string statuDesc(int statu) {
            if(::statu_desc_hash.count(statu)) 
                return ::statu_desc_hash[statu];
            return "no exist";
        }
        // 根据文件后缀名获取文件mime
        static std::string mime(const std::string& filename) {
            size_t pos = filename.rfind(".");
            if(pos == std::string::npos) {
                // 没有后缀名，返回二进制流的mime
                return "application/octet-stream";
            }
            if(::mime_hash.count(filename.substr(pos)))
                return ::mime_hash[filename.substr(pos)];
            return "application/octet-stream";
        }
        // 判断是否是文件
        static bool isDirectory(const std::string& pathname) {
            return std::filesystem::exists(pathname) && std::filesystem::is_directory(pathname);
        }
        // 判断是否是目录
        static bool isRegularFile(const std::string& pathname) {
            return std::filesystem::exists(pathname) && std::filesystem::is_regular_file(pathname);
        }
        // http请求的资源路径有效性判断
        static bool isVaildPath(const std::string& path) {
            // 正常情况，只允许访问合法的文件夹
            int level = 0;
            std::vector<std::string> seps;
            split(path , "/" , &seps);
            for(auto& sep : seps) {
                if(sep == "..") {
                    if(--level < 0)
                        return false;   // 不合法的访问资源路径
                } else {
                    level++;
                }
            }
            return true;
        }
    private:
        // 辅助函数，用于 URL 解码时十六进制转字符整数
        static int hexToIntFilter(char c) {
            if(isdigit(c)) return c - '0';
            if(islower(c)) return c - 'a' + 10;
            if(isupper(c)) return c - 'A' + 10;
            ERROR_LOG("c invalid!");
            return -1;
        }
};

struct HttpRequest {
        std::string _method;              // 请求方法
        std::string _resource_path;       // 资源路径
        std::string _version;             // 协议版本
        std::unordered_map<std::string , std::string> _headers; // 请求头
        std::unordered_map<std::string , std::string> _params;    // 查询字符串
        std::string _context;             // 请求正文
        std::smatch _match;               // 用于正则表达式匹配结果的类

        HttpRequest() :_version("HTTP/1.1") {}
        std::string getMethod() const { return _method; }
        void setMethod(const std::string& method) { _method = method; }
        std::string getResourcePath() const { return _resource_path; }
        void setResourcePath(const std::string& resource_path) { _resource_path = resource_path; }
        std::string getVersion() const { return _version; }
        void setVersion(const std::string& version) { _version = version; }
        std::string getHeader(const std::string& key) const {
            std::unordered_map<std::string , std::string>::const_iterator iter = _headers.find(key);
            if(iter != _headers.end())
                return iter->second;
            return "";
        }
        void setHeader(const std::string& key , const std::string& val) {
            _headers[key] = val;
        }
        bool hasHeader(const std::string& key) const {
            return _headers.count(key);
        }
        std::string getParam(const std::string& key) const {
            std::unordered_map<std::string , std::string>::const_iterator iter = _params.find(key);
            if(iter != _params.end())
                return iter->second;
            return "";
        }
        void setParam(const std::string& key , const std::string& val) {
            _params[key] = val;
        }
        bool hasParam(const std::string& key) const {
            return _params.count(key);
        }
        std::string getContext() const { return _context; }
        void setContext(const std::string& context) { _context = context; }
        void reset() {
            _method.clear();
            _resource_path.clear();
            _version = "HTTP/1.1";
            _headers.clear();
            _params.clear();
            _context.clear();
            std::smatch match;
            _match.swap(match);
        }
        // 短连接返回 true，长连接返回 false
        bool close() const {
            std::unordered_map<std::string , std::string>::const_iterator iter = _headers.find("Connection");
            if(iter != _headers.end() && iter->second == "keep-alive")
                return false;
            return true;
        }
        size_t ContextLength() const {
            std::unordered_map<std::string , std::string>::const_iterator iter = _headers.find("Context-Length");
            if(iter != _headers.end()) {
                return std::stol(iter->second);
            }
            return 0;
        }
};

struct HttpResponse {
        int _statu;                 // 响应状态吗
        bool _is_redirect;          // 是否重定向
        std::string _redirect_url;  // 重定向的url
        std::unordered_map<std::string , std::string> _headers; // 响应头
        std::string _context;       // 响应正文

        HttpResponse() :_statu(200) , _is_redirect(false) {}
        HttpResponse(int statu) :_statu(statu) , _is_redirect(false) {}
        void setRedirect(const std::string& url , int statu = 302) {
            // 302 临时重定向
            _statu = statu;
            _is_redirect = true;
            _redirect_url = url;
        } 
        std::string getHeader(const std::string& key) const {
            std::unordered_map<std::string , std::string>::const_iterator iter = _headers.find(key);
            if(iter != _headers.end())
                return iter->second;
            return "";
        }
        void setHeader(const std::string& key , const std::string& val) {
            _headers[key] = val;
        }
        bool hasHeader(const std::string& key) const {
            return _headers.count(key);
        }
        void setContext(const std::string& context , const std::string& type = "text/html") {
            _context = context;
            _headers["Context-Length"] = type;
            _headers["Context-Length"] = std::to_string(context.size());
        }
        // 短连接返回 true，长连接返回 false
        bool close() const {
            std::unordered_map<std::string , std::string>::const_iterator iter = _headers.find("Connection");
            if(iter != _headers.end() && iter->second == "keep-alive")
                return false;
            return true;
        }
        void reset() {
            _statu = 200;
            _is_redirect = false;
            _redirect_url.clear();
            _headers.clear();
            _context.clear();
        }
};