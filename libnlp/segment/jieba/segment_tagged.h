

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include "libnlp/segment/jieba/segment_base.h"

namespace libnlp::jieba {

    class segment_tagged : public segment_base {
    public:
        segment_tagged() {
        }

        virtual ~segment_tagged() {
        }

        virtual bool tag(const string &src, vector <pair<string, string>> &res) const = 0;

        virtual const dict_trie *get_dict_trie() const = 0;

    }; // class segment_tagged

} // cppjieba
