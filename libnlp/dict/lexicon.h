
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include "libnlp/base/common.h"
#include "libnlp/dict/dict_entry.h"

namespace libnlp::dict {
    /**
     * Storage of all entries
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT lexicon {
    public:
        lexicon() {}

        lexicon(std::vector<std::unique_ptr<dict_entry>> e)
                : _entries(std::move(e)) {}

        lexicon(const lexicon &) = delete;

        lexicon &operator=(const lexicon &) = delete;

        // lexicon will take the ownership of the entry.
        void add(dict_entry *entry) { _entries.emplace_back(entry); }

        void add(std::unique_ptr<dict_entry> entry) {
            _entries.push_back(std::move(entry));
        }

        void sort();

        // Returns true if the lexicon is sorted by key.
        bool is_sorted();

        // Returns true if every key unique (after sorted).
        bool is_unique();

        const dict_entry *at(size_t index) const { return _entries.at(index).get(); }

        size_t length() const { return _entries.size(); }

        std::vector<std::unique_ptr<dict_entry>>::const_iterator begin() const {
            return _entries.begin();
        }

        std::vector<std::unique_ptr<dict_entry>>::const_iterator end() const {
            return _entries.end();
        }

    private:
        std::vector<std::unique_ptr<dict_entry>> _entries;
    };
} // namespace libnlp::dict
