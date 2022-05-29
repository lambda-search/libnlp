//
// Created by liyinbin on 2022/5/29.
//

#pragma once

#include <string>
#include <map>
#include <list>
#include "libnlp/dict/dict_base.h"

namespace libnlp::dict {
    class dict_loader {
    public:
        basic_dict_ptr create_dict_from_file(const std::string &type,
                                             const std::string &fileName, const std::string &configDirectory = "", bool th = false);

        std::list<basic_dict_ptr> create_dict_list(const std::map<std::string, std::string> &list, const std::string &configDirectory = "");

        basic_dict_ptr create_dict_group(const std::map<std::string, std::string> &list, const std::string &configDirectory = "");

        basic_dict_ptr combine(const std::list<basic_dict_ptr> & list);
    };
}
