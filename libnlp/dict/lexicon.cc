
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#include <algorithm>

#include "libnlp/dict/lexicon.h"

namespace libnlp::dict {

    void lexicon::sort() {
        std::sort(_entries.begin(), _entries.end(), dict_entry::UPtrLessThan);
    }

    bool lexicon::is_sorted() {
        return std::is_sorted(_entries.begin(), _entries.end(),
                              dict_entry::UPtrLessThan);
    }

    bool lexicon::is_unique() {
        for (size_t i = 1; i < _entries.size(); ++i) {
            if (_entries[i - 1]->key() == _entries[i]->key()) {
                return false;
            }
        }
        return true;
    }

} // namespace libnlp::dict
