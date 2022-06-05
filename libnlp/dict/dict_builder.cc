
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include <cassert>
#include "libnlp/dict/dict_builder.h"
#include "libnlp/dict/marisa_dict.h"
#include "libnlp/dict/text_dict.h"
#include "libnlp/dict/darts_dict.h"
#include "libnlp/dict/lexicon.h"
#include "libnlp/dict/serializable_dict.h"

namespace libnlp::dict {

    void dict_builder::add_file(const std::string& format, const std::string &inputFileName) {
        basic_dict_ptr dict;
        if (format == "text") {
            dict =  serializable_dict::create_from_file<text_dict>(inputFileName);
        } else if (format == "ocd") {
            dict = serializable_dict::create_from_file<darts_dict>(inputFileName);
        } else if (format == "ocd2") {
            dict = serializable_dict::create_from_file<marisa_dict>(inputFileName);
        } else {
            fprintf(stderr, "Unknown dictionary format: %s\n", format.c_str());
            return;
        }
        auto lex = dict->get_lexicon();
        for(auto &it : *lex) {
            _lex->add(it.get());
        }
    }

    void dict_builder::add(const dict_builder &other) {
        for(auto &entity : *(other._lex)) {
            _lex->add(dict_entity::create(entity->key(), entity->values()));
        }
    }
    void dict_builder::add(const std::string &key) {
        auto de = dict_entity::create(key);
        _lex->add(de);
    }

    void dict_builder::add(const std::string &key, const std::string &value) {

        auto de = dict_entity::create(key,value);
        assert(de);
        assert(_lex);
        _lex->add(de);
    }

    void dict_builder::add(const std::string &key, const std::vector<std::string> &values) {
        auto de = dict_entity::create(key, values);
        _lex->add(de);
    }

    dict dict_builder::get_dict(const std::string &type) const {
        _lex->sort();
        basic_dict_ptr ptr = text_dict_ptr(new text_dict(_lex));
        if (type == "text") {
            return dict(ptr);
        } else if (type == "ocd") {
            return dict(darts_dict::create_from_dict(*ptr.get()));
        } else if (type == "ocd2") {
            return dict(marisa_dict::create_from_dict(*ptr.get()));
        } else {
            fprintf(stderr, "Unknown dictionary format: %s\n", type.c_str());
            return dict(nullptr);
        }
    }

    bool dict_builder::save_to_file(const std::string &format, const std::string &filePahr) const {
        basic_dict_ptr ptr = text_dict_ptr(new text_dict(_lex));
        serializable_dict_ptr dict;

        if (format == "text") {
            dict =  text_dict::create_from_dict(*ptr.get());
        } else if (format == "ocd") {
            dict =  darts_dict::create_from_dict(*ptr.get());
        } else if (format == "ocd2") {
            dict = marisa_dict::create_from_dict(*ptr.get());
        }

        if(!dict) {
            return false;
        }

        try {
            dict->serialize_to_file(filePahr);
        } catch(libnlp::Exception &e) {
            fprintf(stderr, "Unknown dictionary format: %s\n", e.what());
            return false;
        }
        return true;
    }

}  // namespace libnlp::dict
