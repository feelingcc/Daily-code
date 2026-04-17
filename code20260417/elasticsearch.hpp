#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <jsoncpp/json/json.h>

namespace es {
    class Base {
        public:
            Base(const std::string& key) :_key(key) {}
            // 向 Json::Value 添加基本数据类型元素
            template <typename T>
            Base& add(const std::string& key , const T& val) {
                _val[key] = val;
                return *this;
            }
            // 向 Json::Value 添加基本数组类型元素
            template <typename T>
            Base& append(const std::string& key , const T& val) {
                _val[key].append(val);
                return *this;
            }
            const std::string& key() const { return _key; }
            virtual const Json::Value& val() const { return _val; }
            virtual std::string to_json_string() const {
                Json::Value root;
                root[_key] = _val;
                return root.toStyledString();
            }
        protected:
            std::string _key;
            Json::Value _val;
    };

    class Object : public Base {
        public:
            Object(const std::string& key)
                :Base(key)
            {}
            std::shared_ptr<Base> getElement(const std::string& key) {
                std::unordered_map<std::string , std::shared_ptr<Base>>::iterator iter = _subs.find(key);
                if(iter == _subs.end()) {
                    return nullptr;
                }
                return iter->second;
            }
            void addElement(const std::shared_ptr<Base>& sub) {
                _subs[sub->key()] = sub;
            }
            virtual std::string to_json_string() const {
                Json::Value root;
                for(auto& [key , baseptr] : _subs) {
                    root[key] = baseptr->val();
                }
                return root.toStyledString();
            }
        private:
            std::unordered_map<std::string , std::shared_ptr<Base>> _subs;
    };
    // 默认分词器
    class Tokenizer : public Base {
        public:
            Tokenizer(const std::string& key)
                :Base(key)
            {
                this->word();
                this->type();
            }
            void word(const std::string& tokenizer = "ik_max_word") {
                this->add("tokenizer" , tokenizer);
            }
            void type(const std::string& type = "custom") {
                this->add("type" , type);
            }
    };
    class Analyzer : public Object {
        public:
            Analyzer()
                :Object("analyzer")
            {}
            std::shared_ptr<Tokenizer> tokenizer(const std::string& tokenizer_name = "ik_max") {
                std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer>(tokenizer_name);
                this->addElement(tokenizer);    
                return tokenizer;
            }
    };
}