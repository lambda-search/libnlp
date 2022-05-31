

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include "libnlp/base/string_util.h"
#include "libnlp/segment/jieba/segment_tagged.h"
#include "libnlp/segment/jieba/dict_trie.h"

namespace libnlp::jieba {
    using namespace libnlp;

    static const char *const POS_M = "m";
    static const char *const POS_ENG = "eng";
    static const char *const POS_X = "x";

    class pos_tagger {
    public:
        pos_tagger() {
        }

        ~pos_tagger() {
        }

        bool tag(const std::string &src, std::vector<std::pair<std::string, std::string> > &res, const segment_tagged &segment) const {
            std::vector<std::string> CutRes;
            segment.cut(src, CutRes);

            for (std::vector<std::string>::iterator itr = CutRes.begin(); itr != CutRes.end(); ++itr) {
                res.push_back(make_pair(*itr, lookup_tag(*itr, segment)));
            }
            return !res.empty();
        }

        string lookup_tag(const std::string &str, const segment_tagged &segment) const {
            const dict_unit *tmp = NULL;
            rune_str_array runes;
            const dict_trie *dict = segment.get_dict_trie();
            assert(dict != NULL);
            if (!decode_runes_in_string(str, runes)) {
                XLOG(ERROR) << "Decode failed.";
                return POS_X;
            }
            tmp = dict->find(runes.begin(), runes.end());
            if (tmp == NULL || tmp->tag.empty()) {
                return special_rule(runes);
            } else {
                return tmp->tag;
            }
        }

    private:
        const char *special_rule(const rune_str_array &unicode) const {
            size_t m = 0;
            size_t eng = 0;
            for (size_t i = 0; i < unicode.size() && eng < unicode.size() / 2; i++) {
                if (unicode[i].rune < 0x80) {
                    eng++;
                    if ('0' <= unicode[i].rune && unicode[i].rune <= '9') {
                        m++;
                    }
                }
            }
            // ascii char is not found
            if (eng == 0) {
                return POS_X;
            }
            // all the ascii is number char
            if (m == eng) {
                return POS_M;
            }
            // the ascii chars contain english letter
            return POS_ENG;
        }

    }; // class pos_tagger

} // namespace libnlp::jieba

