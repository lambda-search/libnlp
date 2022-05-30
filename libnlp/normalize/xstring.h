
#pragma once


#include <string>
#include <iostream>

namespace libnlp::normalize {

    class xstring {
    public:
        xstring(size_t charset_t = xstring::utf8_t);

        xstring(const xstring &orig, size_t charset_t = xstring::utf8_t);

        xstring(const std::string &orig, size_t charset_t = xstring::utf8_t);

        xstring(const std::string_view &orig, size_t charset_t = xstring::utf8_t);

        xstring(const char *orig, size_t charset_t = xstring::utf8_t);

        xstring &operator=(const xstring &str);

        xstring &operator=(const std::string &str);

        xstring &operator=(const char *str);

        virtual ~xstring();

        std::string operator[](size_t p) const;

        std::string data() const;

        xstring &replace(size_t p, size_t len, const xstring &str);

        xstring &replace(size_t p, size_t len, const std::string &str);

        xstring &replace(size_t p, size_t len, const char *str);

        xstring &replace_punct(char replace, bool trim_blank_list = false);

        xstring &trim_emoji();

        xstring substr(size_t p, size_t len) const;


        size_t size() const;

        size_t byte_length() const;

        xstring &append(const xstring &str);

        xstring &insert(size_t p, const xstring &str);

        xstring &erase(size_t p, size_t len);

        void clear();

        bool empty();

        size_t find(const std::string &str);

        size_t find(const char *str);

        size_t find(const xstring &str);

        void swap(xstring &str);

        xstring &operator+=(const xstring &str);

        xstring &operator-=(const xstring &str);

        friend std::ostream &operator<<(std::ostream &out, const xstring &str);

        friend std::istream &operator>>(std::istream &in, xstring &str);

        friend bool operator==(const xstring &lStr, const xstring &rStr);

        friend const xstring operator+(const xstring &lStr, const xstring &rStr);

        friend const xstring operator-(const xstring &lStr, const xstring &rStr);

    private:
        xstring &replace_print_or_cntrl();

        bool is_print_or_cntrl(char c);

    private:
        std::string _data;
        size_t _charset_t;
    public:
        static const size_t gbk_t, utf8_t, utf16_t;
    };

}  // libnlp::normalize