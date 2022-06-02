
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include "libnlp/base/common.h"
#include "libnlp/dict/dict_entity.h"

namespace libnlp::dict {
    /**
     * Storage of all entries
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT lexicon {
    public:
        lexicon() {}

        lexicon(std::vector<std::unique_ptr<dict_entity>> e)
                : _entities(std::move(e)) {}

        lexicon(const lexicon &) = delete;

        lexicon &operator=(const lexicon &) = delete;

        // lexicon will take the ownership of the entry.
        void add(dict_entity *entry) { _entities.emplace_back(entry); }

        void add(std::unique_ptr<dict_entity> entry) {
            _entities.push_back(std::move(entry));
        }

        void sort();

        // Returns true if the lexicon is sorted by key.
        bool is_sorted();

        // Returns true if every key unique (after sorted).
        bool is_unique();

        const dict_entity *at(size_t index) const { return _entities.at(index).get(); }

        size_t length() const { return _entities.size(); }

        std::vector<std::unique_ptr<dict_entity>>::const_iterator begin() const {
            return _entities.begin();
        }

        std::vector<std::unique_ptr<dict_entity>>::const_iterator end() const {
            return _entities.end();
        }

    private:
        std::vector<std::unique_ptr<dict_entity>> _entities;
    };
} // namespace libnlp::dict
