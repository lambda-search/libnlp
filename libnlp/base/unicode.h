
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ostream>
#include "libnlp/base/inline_vector.h"

namespace libnlp {

    using std::string;
    using std::vector;

    typedef uint32_t rune_t;

    struct word_type {
        string word;
        uint32_t offset;
        uint32_t unicode_offset;
        uint32_t unicode_length;

        word_type(const std::string &w, uint32_t o)
                : word(w), offset(o) {
        }

        word_type(const std::string &w, uint32_t o, uint32_t unicode_offset, uint32_t unicode_length)
                : word(w), offset(o), unicode_offset(unicode_offset), unicode_length(unicode_length) {
        }
    }; // struct word_type

    inline std::ostream &operator<<(std::ostream &os, const word_type &w) {
        return os << "{\"word\": \"" << w.word << "\", \"offset\": " << w.offset << "}";
    }

    struct rune_str {
        rune_t rune;
        uint32_t offset;
        uint32_t len;
        uint32_t unicode_offset;
        uint32_t unicode_length;

        rune_str() : rune(0), offset(0), len(0), unicode_offset(0), unicode_length(0) {
        }

        rune_str(rune_t r, uint32_t o, uint32_t l)
                : rune(r), offset(o), len(l), unicode_offset(0), unicode_length(0) {
        }

        rune_str(rune_t r, uint32_t o, uint32_t l, uint32_t unicode_offset, uint32_t unicode_length)
                : rune(r), offset(o), len(l), unicode_offset(unicode_offset), unicode_length(unicode_length) {
        }
    }; // struct rune_str

    inline std::ostream &operator<<(std::ostream &os, const rune_str &r) {
        return os << "{\"rune\": \"" << r.rune << "\", \"offset\": " << r.offset << ", \"len\": " << r.len << "}";
    }

    typedef libnlp::inline_vector<rune_t> unicode;
    typedef libnlp::inline_vector<struct rune_str> rune_str_array;

    // [left, right]
    struct word_range {
        rune_str_array::const_iterator left;
        rune_str_array::const_iterator right;

        word_range(rune_str_array::const_iterator l, rune_str_array::const_iterator r)
                : left(l), right(r) {
        }

        size_t length() const {
            return right - left + 1;
        }

        bool is_all_ascii() const {
            for (rune_str_array::const_iterator iter = left; iter <= right; ++iter) {
                if (iter->rune >= 0x80) {
                    return false;
                }
            }
            return true;
        }
    }; // struct word_range

    struct rune_str_lite {
        uint32_t rune;
        uint32_t len;

        rune_str_lite() : rune(0), len(0) {
        }

        rune_str_lite(uint32_t r, uint32_t l) : rune(r), len(l) {
        }
    }; // struct rune_str_lite

    inline rune_str_lite decode_rune_in_string(const char *str, size_t len) {
        rune_str_lite rp(0, 0);
        if (str == NULL || len == 0) {
            return rp;
        }
        if (!(str[0] & 0x80)) { // 0xxxxxxx
            // 7bit, total 7bit
            rp.rune = (uint8_t) (str[0]) & 0x7f;
            rp.len = 1;
        } else if ((uint8_t) str[0] <= 0xdf && 1 < len) {
            // 110xxxxxx
            // 5bit, total 5bit
            rp.rune = (uint8_t) (str[0]) & 0x1f;

            // 6bit, total 11bit
            rp.rune <<= 6;
            rp.rune |= (uint8_t) (str[1]) & 0x3f;
            rp.len = 2;
        } else if ((uint8_t) str[0] <= 0xef && 2 < len) { // 1110xxxxxx
            // 4bit, total 4bit
            rp.rune = (uint8_t) (str[0]) & 0x0f;

            // 6bit, total 10bit
            rp.rune <<= 6;
            rp.rune |= (uint8_t) (str[1]) & 0x3f;

            // 6bit, total 16bit
            rp.rune <<= 6;
            rp.rune |= (uint8_t) (str[2]) & 0x3f;

            rp.len = 3;
        } else if ((uint8_t) str[0] <= 0xf7 && 3 < len) { // 11110xxxx
            // 3bit, total 3bit
            rp.rune = (uint8_t) (str[0]) & 0x07;

            // 6bit, total 9bit
            rp.rune <<= 6;
            rp.rune |= (uint8_t) (str[1]) & 0x3f;

            // 6bit, total 15bit
            rp.rune <<= 6;
            rp.rune |= (uint8_t) (str[2]) & 0x3f;

            // 6bit, total 21bit
            rp.rune <<= 6;
            rp.rune |= (uint8_t) (str[3]) & 0x3f;

            rp.len = 4;
        } else {
            rp.rune = 0;
            rp.len = 0;
        }
        return rp;
    }

    inline bool decode_runes_in_string(const char *s, size_t len, rune_str_array &runes) {
        runes.clear();
        runes.reserve(len / 2);
        for (uint32_t i = 0, j = 0; i < len;) {
            rune_str_lite rp = decode_rune_in_string(s + i, len - i);
            if (rp.len == 0) {
                runes.clear();
                return false;
            }
            rune_str x(rp.rune, i, rp.len, j, 1);
            runes.push_back(x);
            i += rp.len;
            ++j;
        }
        return true;
    }

    inline bool decode_runes_in_string(const std::string &s, rune_str_array &runes) {
        return decode_runes_in_string(s.c_str(), s.size(), runes);
    }

    inline bool decode_runes_in_string(const char *s, size_t len, unicode &unicode) {
        unicode.clear();
        rune_str_array runes;
        if (!decode_runes_in_string(s, len, runes)) {
            return false;
        }
        unicode.reserve(runes.size());
        for (size_t i = 0; i < runes.size(); i++) {
            unicode.push_back(runes[i].rune);
        }
        return true;
    }

    inline bool is_single_word(const std::string &str) {
        rune_str_lite rp = decode_rune_in_string(str.c_str(), str.size());
        return rp.len == str.size();
    }

    inline bool decode_runes_in_string(const std::string &s, unicode &unicode) {
        return decode_runes_in_string(s.c_str(), s.size(), unicode);
    }

    inline unicode decode_runes_in_string(const std::string &s) {
        unicode result;
        decode_runes_in_string(s, result);
        return result;
    }


    // [left, right]
    inline word_type
    get_word_from_runes(const std::string &s, rune_str_array::const_iterator left, rune_str_array::const_iterator right) {
        assert(right->offset >= left->offset);
        uint32_t len = right->offset - left->offset + right->len;
        uint32_t unicode_length = right->unicode_offset - left->unicode_offset + right->unicode_length;
        return word_type(s.substr(left->offset, len), left->offset, left->unicode_offset, unicode_length);
    }

    inline string
    get_string_from_runes(const std::string &s, rune_str_array::const_iterator left, rune_str_array::const_iterator right) {
        assert(right->offset >= left->offset);
        uint32_t len = right->offset - left->offset + right->len;
        return s.substr(left->offset, len);
    }

    inline void get_words_from_word_ranges(const std::string &s, const std::vector<word_range> &wrs, vector<word_type> &words) {
        for (size_t i = 0; i < wrs.size(); i++) {
            words.push_back(get_word_from_runes(s, wrs[i].left, wrs[i].right));
        }
    }

    inline vector<word_type> get_words_from_word_ranges(const std::string &s, const std::vector<word_range> &wrs) {
        vector<word_type> result;
        get_words_from_word_ranges(s, wrs, result);
        return result;
    }

    inline void get_strings_from_words(const std::vector<word_type> &words, vector<string> &strs) {
        strs.resize(words.size());
        for (size_t i = 0; i < words.size(); ++i) {
            strs[i] = words[i].word;
        }
    }

}  // namespace libnlp

