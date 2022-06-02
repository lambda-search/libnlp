
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
        std::sort(_entities.begin(), _entities.end(), dict_entity::smart_ptr_less_than);
    }

    bool lexicon::is_sorted() {
        return std::is_sorted(_entities.begin(), _entities.end(),
                              dict_entity::smart_ptr_less_than);
    }

    bool lexicon::is_unique() {
        for (size_t i = 1; i < _entities.size(); ++i) {
            if (_entities[i - 1]->key() == _entities[i]->key()) {
                return false;
            }
        }
        return true;
    }

} // namespace libnlp::dict
