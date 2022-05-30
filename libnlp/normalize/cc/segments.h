
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <sstream>

#include "libnlp/base/common.h"

namespace libnlp::cc {
    /**
     * Segmented text
     * @ingroup lib_cc_cpp_api
     */
    class LIBNLP_EXPORT segments {
    public:
        segments() {}

        segments(std::initializer_list<const char *> initList) {
            for (const std::string &item : initList) {
                add_segment(item);
            }
        }

        segments(std::initializer_list<std::string> initList) {
            for (const std::string &item : initList) {
                add_segment(item);
            }
        }

        void add_segment(const char *unmanagedString) {
            indexes.push_back(std::make_pair(unmanaged.size(), false));
            unmanaged.push_back(unmanagedString);
        }

        void add_segment(const std::string &str) {
            indexes.push_back(std::make_pair(managed.size(), true));
            managed.push_back(str);
        }

        class iterator : public std::iterator<std::input_iterator_tag, const char *> {
        public:
            iterator(const segments *const _segments, size_t _cursor)
                    : segments(_segments), cursor(_cursor) {}

            iterator &operator++() {
                cursor++;
                return *this;
            }

            bool operator==(const iterator &that) const {
                return cursor == that.cursor && segments == that.segments;
            }

            bool operator!=(const iterator &that) const {
                return !this->operator==(that);
            }

            const char *operator*() const { return segments->at(cursor); }

        private:
            const segments *const segments;
            size_t cursor;
        };

        const char *at(size_t cursor) const {
            const auto &index = indexes[cursor];
            if (index.second) {
                return managed[index.first].c_str();
            } else {
                return unmanaged[index.first];
            }
        }

        size_t length() const { return indexes.size(); }

        iterator begin() const { return iterator(this, 0); }

        iterator end() const { return iterator(this, indexes.size()); }

        std::string to_string() const {
            // TODO implement a nested structure to reduce concatenation,
            // like a purely functional differential list
            std::ostringstream buffer;
            for (const char *segment : *this) {
                buffer << segment;
            }
            return buffer.str();
        }

    private:
        segments(const segments &) {}

        std::vector<const char *> unmanaged;
        std::vector<std::string> managed;
        // index, managed
        std::vector<std::pair<size_t, bool>> indexes;
    };
} // namespace libnlp::cc
