
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#include <cctype>
#include "libnlp/normalize/xstring.h"

namespace libnlp::normalize {

    namespace {
        bool is_emoji(const std::string_view &singleCharStr) {
            char len = singleCharStr.size();
            long long num = 0;
            long long multi = 1;
            for (int i = len - 1; i >= 0 ; --i) {
                char c = singleCharStr[i];
                if (i == 0) {
                    char flag = 0x1;
                    char step = 6 - len;
                    flag = flag << step;
                    for (; step > 0; --step) {
                        flag = flag | (flag >> step);
                    }
                    c = c & flag;
                } else {
                    c = c & 0x3f;
                }

                num += c * multi;
                multi *= 64;
            }

            if (num >= 0x1f300 && num <= 0x1f6ff) {
                return true;
            } else {
                return false;
            }
        }
    }

    const size_t xstring::gbk_t = 2, xstring::utf8_t = 3, xstring::utf16_t = 4;

    xstring::xstring(size_t charset_t) : _data(""), _charset_t(charset_t) {

    }

    xstring::xstring(const std::string_view &orig, size_t charset_t) :_data(orig.data(), orig.size()), _charset_t(charset_t) {

    }

    xstring::xstring(const xstring &orig, size_t charset_t) : _data(orig._data), _charset_t(charset_t) {

    }

    xstring::xstring(const std::string &orig, size_t charset_t) : _data(orig), _charset_t(charset_t) {
        this->replace_print_or_cntrl();
    }

    xstring::xstring(const char *orig, size_t charset_t) : _data(orig), _charset_t(charset_t) {
        this->replace_print_or_cntrl();

    }

    xstring::~xstring() {
    }

    xstring &xstring::operator=(const xstring &str) {
        this->_data = str._data;
        return *this;
    }

    xstring &xstring::operator=(const std::string &str) {
        this->_data = str;
        return this->replace_print_or_cntrl();
    }

    xstring &xstring::operator=(const char *str) {
        this->_data = str;
        return this->replace_print_or_cntrl();
    }

    std::string xstring::operator[](size_t p) const {
        std::string tmp = this->_data.substr(p * this->_charset_t, this->_charset_t);
        if (tmp[0] == ' ') {
            tmp.erase(0, 2);
        }
        return tmp;
    }

    xstring &xstring::replace(size_t p, size_t len, const xstring &str) {
        return this->erase(p, len).insert(p, str);
    }

    xstring &xstring::replace(size_t p, size_t len, const std::string &str) {
        return this->erase(p, len).insert(p, str);
    }

    xstring &xstring::replace(size_t p, size_t len, const char *str) {
        return this->erase(p, len).insert(p, str);
    }

    xstring &xstring::replace_punct(char replace, bool trim_blank_list) {
        std::string space(this->_charset_t - 1, replace);
        size_t back_space = std::string::npos;
        std::string alt;
        for (size_t i = 0; i < this->_data.size(); ++i) {
            if (std::isspace(this->_data[i])) {
                if(back_space == std::string::npos) {
                    back_space = i;
                    alt.append(1, this->_data[i]);
                }
                continue;
            }

            if (std::ispunct(this->_data[i])) {
                i += this->_charset_t - 1;
                if(std::isspace(replace)) {
                    continue;
                }
                alt.append(space);
                back_space = std::string::npos;
                continue;
            }
            back_space = std::string::npos;
            alt.append(1, this->_data[i]);
        }
        return *this;
    }

    xstring &xstring::trim_emoji() {
        std::string_view str(_data.data(), _data.size());
        std::string newStr = "";
        for (size_t i = 0; i < str.length();) {
            char c = str[i];
            if (c & 0x80) {
                char flag = 0x80 >> 1;
                char count = 1;
                while ((c & flag) && (flag > 0)) {
                    count += 1;
                    flag = flag >> 1;
                }

                std::string subStr = "";
                for (char j = 0; j < count; ++j) {
                    subStr.push_back(str[i + j]);
                }

                if (is_emoji(subStr)) {
                    subStr = "";
                }

                newStr.append(subStr);
                i += count;
            } else {
                newStr.push_back(c);
                i += 1;
            }
        }
        _data.swap(newStr);
        return *this;
    }

    xstring xstring::substr(size_t p, size_t len) const {
        xstring str;
        str._data.append(this->_data.substr(p * this->_charset_t, len * this->_charset_t));
        return str;
    }

    size_t xstring::size() const {
        return this->_data.size() / this->_charset_t;
    }

    size_t xstring::byte_length() const {
        return this->_data.size();
    }

    xstring &xstring::append(const xstring &str) {
        this->_data.append(str._data);
        return *this;
    }

    xstring &xstring::insert(size_t p, const xstring &str) {
        this->_data.insert(p * this->_charset_t, str._data);
        return *this;
    }

    xstring &xstring::erase(size_t p, size_t len) {
        this->_data.erase(p * this->_charset_t, len * this->_charset_t);
        return *this;
    }

    void xstring::clear() {
        this->_data.clear();
    }

    std::string xstring::data() const {
        return _data;
    }

    bool xstring::empty() {
        return this->_data.empty();
    }

    size_t xstring::find(const std::string &str) {
        size_t p = this->_data.find(str);
        if (p == std::string::npos) {
            return std::string::npos;
        }
        return p / this->_charset_t;
    }

    size_t xstring::find(const char *str) {
        size_t p = this->_data.find(str);
        if (p == std::string::npos) {
            return std::string::npos;
        }
        return p / this->_charset_t;
    }

    size_t xstring::find(const xstring &str) {
        size_t p = this->_data.find(str._data);
        if (p == std::string::npos) {
            return std::string::npos;
        }
        return p / this->_charset_t;
    }

    xstring &xstring::replace_print_or_cntrl() {
        std::string space(this->_charset_t - 1, ' ');
        for (size_t i = 0; i < this->_data.size(); ++i) {
            if (this->is_print_or_cntrl(this->_data[i])) {
                this->_data.insert(i, space);
                i += this->_charset_t - 1;
            }
        }
        return *this;
    }

    bool xstring::is_print_or_cntrl(char c) {
        return std::isprint(c) || std::iscntrl(c);
    }

    std::ostream &operator<<(std::ostream &out, const xstring &str) {
        size_t len = str.size();
        for (size_t i = 0; i < len; ++i) {
            out << str[i];
        }
        return out;
    }

    std::istream &operator>>(std::istream &in, xstring &str) {
        std::string tmp;
        in >> tmp;
        str = tmp;
        return in;
    }

    void xstring::swap(xstring &str) {
        this->_data.swap(str._data);
    }

    xstring &xstring::operator+=(const xstring &str) {
        return this->append(str);
    }

    xstring &xstring::operator-=(const xstring &str) {
        size_t p = this->find(str);
        if (p != std::string::npos) {
            return this->erase(p, str.size());
        }
        return *this;
    }

    bool operator==(const xstring &lStr, const xstring &rStr) {
        return lStr._data == rStr._data;
    }

    const xstring operator+(const xstring &lStr, const xstring &rStr) {
        xstring result;
        result._data = lStr._data + rStr._data;
        return result;
    }

    const xstring operator-(const xstring &lStr, const xstring &rStr) {
        xstring result(lStr);
        size_t p = result.find(rStr);
        if (p != std::string::npos) {
            return result.erase(p, rStr.size());
        }
        return result;
    }

}  // libnlp::normalize