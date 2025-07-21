#pragma once

#include <iostream>
#include <cstring>
#include <cassert>

namespace simulateSTL
{
    class string
    {
    public:
        typedef char* iterator;
        typedef const char* const_iterator;
        iterator begin() { return _str; }
        iterator end() { return _str + _size; }
        const_iterator begin() const { return _str; }
        const_iterator end() const { return _str + _size; }
        size_t size() const { return _size; }
        size_t capacity() const { return _capacity; }
        void clear() { _size = 0; _str[_size] = 0; }
        bool empty() const { return _size == 0; }
        char& operator[](size_t pos) { assert(pos < _size); return _str[pos]; }
        const char& operator[](size_t pos) const { assert(pos < _size); return _str[pos]; }
        const char* c_str() const { return _str; } 
        bool operator==(const string& str) const { return !strcmp(_str , str._str); }
        bool operator!=(const string& str) const { return !(*this == str); }
        bool operator<(const string& str) const { return strcmp(_str , str._str) < 0; }
        bool operator<=(const string& str) const { return *this < str || *this == str; }
        bool operator>(const string& str) const { return !(*this <= str); }
        bool operator>=(const string& str) const { return !(*this < str); }
        void swap (string& str) { std::swap(_str , str._str);std::swap(_size , str._size);std::swap(_capacity , str._capacity); }
    public:
        string(const char *str = "");
        string(const string& str);
        string& operator=(string str);
        ~string();
        void reserve(size_t n);
        void resize(size_t n , char c = '\0');
        void push_back(char c);
        void pop_back();
        string& append(const char* s);
        string& operator+=(const string& str);
        string& operator+=(char c);
        string& insert(size_t pos , char c);
        string& insert(size_t pos , const char* s);
        string& erase(size_t pos);
        string substr(size_t pos = 0 , size_t len = npos) const;
        size_t find(char c , size_t pos = 0) const;
    private:
        char *_str = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;

    public:
        static const size_t npos = -1;
    };

    string::string(const char *str)
        : _size(strlen(str))
    {
        _str = new char[_size + 1]; // 兼容C保留'\0'位置
        strcpy(_str, str);
        _capacity = _size;
    }

    // 现代写法
    string::string(const string& str) {
        string temp(str._str);
        swap(temp);
    }

    // 现代写法
    string& string::operator=(string str) {
        swap(str);
        return *this;
    }

    string::~string()
    {
        // C++中可以delete nullptr
        delete[] _str;
        _size = _capacity = 0;
    }

    void string::reserve(size_t n) {
        if (n > _capacity) {
            char* temp = new char[n + 1];   // 兼容C保留'\0'
            strcpy(temp , _str);
            delete[] _str;
            _str = temp;
            _capacity = n;
        }
    }

    void string::resize(size_t n , char c) {
        if (n <= _size) {
            _size = n;
            _str[_size] = 0;
        } else {
            if (n > _capacity) {
                reserve(n);
            }
            
            while (_size < n) {
                _str[_size++] = c;
            }

            _str[_size] = 0;
        }
    }

    void string::push_back(char c) {
        if (_size == _capacity) {
            reserve(_capacity == 0 ? 4 : _capacity * 2);
        }

        _str[_size++] = c;
        _str[_size] = 0;

    }

    void string::pop_back() {
        assert(!empty());
        _str[--_size] = 0;
    }

    string& string::append(const char* s) {
        size_t len = strlen(s);
        if (_size + len > _capacity) {
            reserve(_size + len > _capacity * 2 ? _size + len : _capacity * 2);
        }
        for (size_t i = 0; i < len; i++) {
            _str[_size++] = s[i];
        }
        _str[_size] = 0;

        return *this;
    }

    string& string::operator+=(const string& str) {
        return append(str._str);
    }

    string& string::operator+=(char c) {
        push_back(c);
        return *this;
    }

    string& string::insert(size_t pos , char c) {
        assert(pos <= _size);
        if (_size == _capacity) {
            reserve(_capacity == 0 ? 4 : _capacity * 2);
        }

        size_t end = _size + 1;
        while (end > pos) {
            _str[end] = _str[end - 1];
            end--;
        }

        _str[pos] = c;
        _size++;

        return *this;
    }

    string& string::insert(size_t pos , const char* s) {
        assert(pos <= _size);
        size_t len = strlen(s);
        if (_size + len > _capacity) {
            reserve(_size + len > _capacity * 2 ? _size + len : _capacity * 2);
        }

        size_t end = _size + len;
        while (end > pos + len - 1) {
            _str[end] = _str[end - len];
            end--;
        }
        strncpy(_str + pos , s , len);
        _size += len;

        return *this;
    }

    string& string::erase(size_t pos) {
        assert(pos < _size);
        size_t end = pos;
        while (end < _size) {
            _str[end] = _str[end + 1];
            end++;
        }
        _size--;

        return *this;
    }

    string string::substr(size_t pos , size_t len) const {
        assert(pos < _size);
        // 修正len
        if (len + pos > _size || len == npos) {
            len = _size - pos;
        }
        string sub;
        sub.reserve(len);
        for (size_t i = 0; i < len; i++) {
            sub += _str[pos + i];
        }
        return sub;
    }

    size_t string::find(char c , size_t pos) const {
        assert(pos < _size);
        while (pos < _size) {
            if (_str[pos] == c) return pos;
            pos++;
        }
        return npos;
    }

    // 全局运算符重载函数
    std::ostream& operator<<(std::ostream& out, const string& s)
	{
		for (auto ch : s) {
			out << ch;
		}

		return out;
	}

    std::istream& operator>>(std::istream& in, string& s) {
		s.clear();

        // 定义缓冲区
		const int N = 256;
		char buff[N]; 
		int i = 0;  // 0 ~ 255

		char ch;
		ch = in.get();  // 获取单个字符
		while (ch != ' ' && ch != '\n')
		{
			buff[i++] = ch;
			if (i == N-1)
			{
				buff[i] = '\0';
				s += buff;

				i = 0;
			}

			ch = in.get();
		}

		if (i > 0)
		{
			buff[i] = '\0';
			s += buff;
		}

		return in;
	}
}