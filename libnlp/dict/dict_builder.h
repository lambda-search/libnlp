
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#pragma once

#include <string>
#include <vector>

#include "libnlp/base/common.h"
#include "libnlp/dict/dict.h"

namespace libnlp::dict {

    class dict_builder {
    public:
        explicit dict_builder(const std::string &name) : _name(name), _lex(new lexicon()) {}

        ~dict_builder() = default;

        void add_file(const std::string &format, const std::string &path);

        void add(const dict_builder &other);

        void add(const std::string &key);

        void add(const std::string &key, const std::string &value);

        void add(const std::string &key, const std::vector<std::string> &values);

        dict get_dict(const std::string &type) const;

        bool save_to_file(const std::string &format, const std::string &filePahr) const;

        std::string to_string() const {
            return "entries: " + std::to_string(_lex->length());
        }

    private:
        std::string _name;
        lexicon_ptr _lex;
    };

}  // namespace libnlp::dict

