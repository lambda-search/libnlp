//
// Created by liyinbin on 2022/5/30.
//

#pragma once

#include <string>
#include <vector>

#include "libnlp/base/common.h"

namespace libnlp::dict {

    class dict_builder {
    public:

        void add_file(const std::string &format, const std::string &path);

        void add(const std::string &key);

        void add(const std::string &key, const std::string &value);

        void add(const std::string &key, const std::vector<std::string> &values);

        basic_dict_ptr get_dict(const std::string &type) const;

        bool save_to_file(const std::string &format, const std::string &filePahr) const;

    private:
        lexicon_ptr _lex;
    };

}  // namespace libnlp::dict

