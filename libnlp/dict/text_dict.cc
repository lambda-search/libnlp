
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include <algorithm>
#include <cassert>

#include "libnlp/dict/lexicon.h"
#include "libnlp/dict/text_dict.h"

namespace libnlp::dict {

    static size_t get_key_max_length(const lexicon_ptr &lexicon) {
        size_t _max_length = 0;
        for (const auto &entry : *lexicon) {
            size_t keyLength = entry->key_length();
            _max_length = (std::max)(keyLength, _max_length);
        }
        return _max_length;
    }

    static dict_entity *ParseKeyValues(const char *buff, size_t lineNum) {
        size_t length;
        const char *pbuff = UTF8Util::FindNextInline(buff, '\t');
        if (UTF8Util::IsLineEndingOrFileEnding(*pbuff)) {
            throw InvalidTextDictionary("Tabular not found " + std::string(buff),
                                        lineNum);
        }
        length = static_cast<size_t>(pbuff - buff);
        std::string key = UTF8Util::FromSubstr(buff, length);
        std::vector<std::string> values;
        while (!UTF8Util::IsLineEndingOrFileEnding(*pbuff)) {
            buff = pbuff = UTF8Util::NextChar(pbuff);
            pbuff = UTF8Util::FindNextInline(buff, ' ');
            length = static_cast<size_t>(pbuff - buff);
            const std::string &value = UTF8Util::FromSubstr(buff, length);
            values.push_back(value);
        }
        if (values.size() == 0) {
            throw InvalidTextDictionary("No value in an item", lineNum);
        } else if (values.size() == 1) {
            return dict_entity::create(key, values.at(0));
        } else {
            return dict_entity::create(key, values);
        }
    }

    static lexicon_ptr ParseLexiconFromFile(FILE *fp) {
        const int ENTRY_BUFF_SIZE = 4096;
        char buff[ENTRY_BUFF_SIZE];
        lexicon_ptr lex(new lexicon);
        UTF8Util::SkipUtf8Bom(fp);
        size_t lineNum = 1;
        while (fgets(buff, ENTRY_BUFF_SIZE, fp)) {
            lex->add(ParseKeyValues(buff, lineNum));
            lineNum++;
        }
        return lex;
    }

    text_dict::text_dict(const lexicon_ptr &l)
            : _max_length(get_key_max_length(l)), _lex(l) {
        assert(_lex->is_sorted());
        assert(_lex->is_unique());
    }

    text_dict::~text_dict() {}

    text_dict_ptr text_dict::create_from_sorted_file(FILE *fp) {
        const lexicon_ptr &lexicon = ParseLexiconFromFile(fp);
        return text_dict_ptr(new text_dict(lexicon));
    }

    text_dict_ptr text_dict::create_from_file(FILE *fp) {
        const lexicon_ptr &lexicon = ParseLexiconFromFile(fp);
        lexicon->sort();
        if (!lexicon->is_unique()) {
            throw InvalidFormat("The text dictionary contains duplicated keys.");
        }
        return text_dict_ptr(new text_dict(lexicon));
    }

    text_dict_ptr text_dict::create_from_dict(const basic_dict &dict) {
        return text_dict_ptr(new text_dict(dict.get_lexicon()));
    }

    size_t text_dict::key_max_length() const { return _max_length; }

    std::optional<const dict_entity *> text_dict::match(const char *word, size_t len) const {
        std::unique_ptr<dict_entity> entry(
                new dict_entity(std::string(word, len)));
        const auto &found = std::lower_bound(_lex->begin(), _lex->end(), entry,
                                             dict_entity::smart_ptr_less_than);
        if ((found != _lex->end()) && ((*found)->key() == entry->key())) {
            return std::optional<const dict_entity *>(found->get());
        } else {
            return std::nullopt;
        }
    }

    lexicon_ptr text_dict::get_lexicon() const { return _lex; }

    void text_dict::serialize_to_file(FILE *fp) const {
        for (const auto &entry : *_lex) {
            fprintf(fp, "%s\n", entry->to_string().c_str());
        }
    }
}