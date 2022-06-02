
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include <algorithm>
#include <cstring>

#include "libnlp/dict/binary_dict.h"
#include "libnlp/dict/darts_dict.h"
#include "libnlp/dict/lexicon.h"
#include "libnlp/trie/darts.h"

namespace libnlp::dict {

    static const char *OCDHEADER = "NLPCCDARTS1";

    class darts_dict::darts_internal {
    public:
        binary_dict_ptr binary;
        void *buffer;
        Darts::DoubleArray *doubleArray;

        darts_internal() : binary(nullptr), buffer(nullptr), doubleArray(nullptr) {}

        ~darts_internal() {
            if (buffer != nullptr) {
                free(buffer);
            }
            if (doubleArray != nullptr) {
                delete doubleArray;
            }
        }
    };

    darts_dict::darts_dict() { _internal = new darts_internal; }

    darts_dict::~darts_dict() { delete _internal; }

    size_t darts_dict::key_max_length() const { return _max_length; }

    std::optional<const dict_entity *> darts_dict::match(const char *word,
                                                         size_t len) const {
        if (len > _max_length) {
            return std::nullopt;
        }
        Darts::DoubleArray &dict = *_internal->doubleArray;
        Darts::DoubleArray::result_pair_type result;

        dict.exactMatchSearch(word, result, len);
        if (result.value != -1) {
            return std::optional<const dict_entity *>(
                    _lex->at(static_cast<size_t>(result.value)));
        } else {
            return std::nullopt;
        }
    }

    std::optional<const dict_entity *> darts_dict::match_prefix(const char *word,
                                                                size_t len) const {
        const size_t DEFAULT_NUM_ENTRIES = 64;
        Darts::DoubleArray &dict = *_internal->doubleArray;
        Darts::DoubleArray::value_type results[DEFAULT_NUM_ENTRIES];
        Darts::DoubleArray::value_type maxMatchedResult;
        size_t numMatched = dict.commonPrefixSearch(
                word, results, DEFAULT_NUM_ENTRIES, (std::min)(_max_length, len));
        if (numMatched == 0) {
            return std::nullopt;
        } else if ((numMatched > 0) && (numMatched < DEFAULT_NUM_ENTRIES)) {
            maxMatchedResult = results[numMatched - 1];
        } else {
            Darts::DoubleArray::value_type *rematchedResults =
                    new Darts::DoubleArray::value_type[numMatched];
            numMatched = dict.commonPrefixSearch(word, rematchedResults, numMatched,
                                                 (std::min)(_max_length, len));
            maxMatchedResult = rematchedResults[numMatched - 1];
            delete[] rematchedResults;
        }
        if (maxMatchedResult >= 0) {
            return std::optional<const dict_entity *>(
                    _lex->at(static_cast<size_t>(maxMatchedResult)));
        } else {
            return std::nullopt;
        }
    }

    lexicon_ptr darts_dict::get_lexicon() const { return _lex; }

    darts_dict_ptr darts_dict::create_from_file(FILE *fp) {
        darts_dict_ptr dict(new darts_dict());

        Darts::DoubleArray *doubleArray = new Darts::DoubleArray();
        size_t headerLen = strlen(OCDHEADER);
        void *buffer = malloc(sizeof(char) * headerLen);
        size_t bytesRead = fread(buffer, sizeof(char), headerLen, fp);
        if (bytesRead != headerLen || memcmp(buffer, OCDHEADER, headerLen) != 0) {
            throw InvalidFormat("Invalid nlp_cc dictionary header");
        }
        free(buffer);

        size_t dartsSize;
        bytesRead = fread(&dartsSize, sizeof(size_t), 1, fp);
        if (bytesRead * sizeof(size_t) != sizeof(size_t)) {
            throw InvalidFormat("Invalid nlp_cc dictionary header (dartsSize)");
        }
        buffer = malloc(dartsSize);
        bytesRead = fread(buffer, 1, dartsSize, fp);
        if (bytesRead != dartsSize) {
            throw InvalidFormat("Invalid nlp_cc dictionary size of darts mismatch");
        }
        doubleArray->set_array(buffer);

        auto internal = dict->_internal;
        internal->buffer = buffer;
        internal->binary = binary_dict::create_from_file(fp);
        internal->doubleArray = doubleArray;
        dict->_lex = internal->binary->get_lexicon();
        dict->_max_length = internal->binary->key_max_length();
        return dict;
    }

    darts_dict_ptr darts_dict::create_from_dict(const basic_dict &thatDict) {
        darts_dict_ptr dict(new darts_dict());

        Darts::DoubleArray *doubleArray = new Darts::DoubleArray();
        std::vector<std::string> keys;
        std::vector<const char *> keys_cstr;
        size_t _max_length = 0;
        const lexicon_ptr &lex = thatDict.get_lexicon();
        size_t lexiconCount = lex->length();
        keys.resize(lexiconCount);
        keys_cstr.resize(lexiconCount);
        for (size_t i = 0; i < lexiconCount; i++) {
            const dict_entity *entry = lex->at(i);
            keys[i] = entry->key();
            keys_cstr[i] = keys[i].c_str();
            _max_length = (std::max)(entry->key_length(), _max_length);
        }
        doubleArray->build(lex->length(), &keys_cstr[0]);
        dict->_lex = lex;
        dict->_max_length = _max_length;
        auto internal = dict->_internal;
        internal->doubleArray = doubleArray;
        return dict;
    }

    void darts_dict::serialize_to_file(FILE *fp) const {
        Darts::DoubleArray &dict = *_internal->doubleArray;

        fwrite(OCDHEADER, sizeof(char), strlen(OCDHEADER), fp);

        size_t dartsSize = dict.total_size();
        fwrite(&dartsSize, sizeof(size_t), 1, fp);
        fwrite(dict.array(), sizeof(char), dartsSize, fp);

        _internal->binary.reset(new binary_dict(_lex));
        _internal->binary->serialize_to_file(fp);
    }
}  // libnlp::dict
