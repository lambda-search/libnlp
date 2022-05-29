//
// Created by liyinbin on 2022/5/29.
//

#include "libnlp/dict/dict_loader.h"
#include "libnlp/dict/serializable_dict.h"
#include "libnlp/dict/marisa_dict.h"
#include "libnlp/dict/darts_dict.h"
#include "libnlp/dict/text_dict.h"
#include "libnlp/dict/dict_group.h"


namespace libnlp::dict {
    namespace {
        template<typename DICT>
        libnlp::dict::basic_dict_ptr
        load_dict_with_paths(const std::string &fileName, const std::string &configDirectory) {
            // Working directory
            std::shared_ptr<DICT> dict;
            if (libnlp::dict::serializable_dict::try_load_from_file<DICT>(fileName, &dict)) {
                return dict;
            }
            // Configuration directory
            if ((configDirectory != "") && libnlp::dict::serializable_dict::try_load_from_file<DICT>(
                    configDirectory + fileName, &dict)) {
                return dict;
            }
            // Package data directory
            if ((PACKAGE_DATA_DIRECTORY != "") &&
                libnlp::dict::serializable_dict::try_load_from_file<DICT>(
                        PACKAGE_DATA_DIRECTORY + fileName, &dict)) {
                return dict;
            }
            throw FileNotFound(fileName);
        }
    }  // namespace

    basic_dict_ptr dict_loader::create_dict_from_file(const std::string &type,
                                                      const std::string &fileName,
                                                      const std::string &configDirectory, bool th) {
        if (type == "text") {
            libnlp::dict::basic_dict_ptr dict = load_dict_with_paths<libnlp::dict::text_dict>(fileName,
                                                                                              configDirectory);
            return libnlp::dict::marisa_dict::create_from_dict(*dict.get());
        }
        if (type == "ocd") {
            return load_dict_with_paths<libnlp::dict::darts_dict>(fileName, configDirectory);
        }
        if (type == "ocd2") {
            return load_dict_with_paths<libnlp::dict::marisa_dict>(fileName, configDirectory);
        }
        if (th) {
            throw InvalidFormat("Unknown dictionary type: " + type);
        }
        return nullptr;
    }

    std::list<basic_dict_ptr>
    dict_loader::create_dict_list(const std::map<std::string, std::string> &list, const std::string &configDirector) {
        std::list<basic_dict_ptr> dict_list;
        for (auto &item : list) {
            auto ptr = create_dict_from_file(item.first, item.second, configDirector, false);
            if (ptr) {
                dict_list.push_back(ptr);
            }
        }
        return dict_list;
    }

    basic_dict_ptr dict_loader::combine(const std::list<basic_dict_ptr> &list) {
        return dict_group_ptr(new dict_group(list));
    }

    basic_dict_ptr
    dict_loader::create_dict_group(const std::map<std::string, std::string> &list, const std::string &configDirectory) {
        auto dict_list = create_dict_list(list, configDirectory);
        return dict_group_ptr(new dict_group(dict_list));
    }

}  // libnlp::dict