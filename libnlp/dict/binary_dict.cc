
#include <algorithm>
#include <cassert>
#include <cstring>

#include "libnlp/dict/binary_dict.h"
#include "libnlp/dict/lexicon.h"

namespace libnlp::dict {

    size_t binary_dict::key_max_length() const {
        size_t _max_length = 0;
        for (const std::unique_ptr<dict_entry> &entry : *_lex) {
            _max_length = (std::max)(_max_length, entry->key_length());
        }
        return _max_length;
    }

    void binary_dict::serialize_to_file(FILE *fp) const {
        std::string keyBuf, valueBuf;
        std::vector<size_t> keyOffsets, valueOffsets;
        size_t keyTotalLength = 0, valueTotalLength = 0;
        ConstructBuffer(keyBuf, keyOffsets, keyTotalLength, valueBuf, valueOffsets,
                        valueTotalLength);
        // Number of items
        size_t numItems = _lex->length();
        fwrite(&numItems, sizeof(size_t), 1, fp);

        // Data
        fwrite(&keyTotalLength, sizeof(size_t), 1, fp);
        fwrite(keyBuf.c_str(), sizeof(char), keyTotalLength, fp);
        fwrite(&valueTotalLength, sizeof(size_t), 1, fp);
        fwrite(valueBuf.c_str(), sizeof(char), valueTotalLength, fp);

        size_t keyCursor = 0, valueCursor = 0;
        for (const std::unique_ptr<dict_entry> &entry : *_lex) {
            // Number of values
            size_t numValues = entry->num_values();
            fwrite(&numValues, sizeof(size_t), 1, fp);
            // Key offset
            size_t keyOffset = keyOffsets[keyCursor++];
            fwrite(&keyOffset, sizeof(size_t), 1, fp);
            // Values offset
            for (size_t i = 0; i < numValues; i++) {
                size_t valueOffset = valueOffsets[valueCursor++];
                fwrite(&valueOffset, sizeof(size_t), 1, fp);
            }
        }
        assert(keyCursor == numItems);
    }

    binary_dict_ptr binary_dict::create_from_file(FILE *fp) {
        size_t offsetBound, savedOffset;
        savedOffset = ftell(fp);
        fseek(fp, 0L, SEEK_END);
        offsetBound = ftell(fp) - savedOffset;
        fseek(fp, savedOffset, SEEK_SET);

        binary_dict_ptr dict(new binary_dict(lexicon_ptr(new lexicon)));

        // Number of items
        size_t numItems;
        size_t unitsRead = fread(&numItems, sizeof(size_t), 1, fp);
        if (unitsRead != 1) {
            throw InvalidFormat("Invalid nlp_cc binary dictionary (numItems)");
        }

        // Keys
        size_t keyTotalLength;
        unitsRead = fread(&keyTotalLength, sizeof(size_t), 1, fp);
        if (unitsRead != 1) {
            throw InvalidFormat("Invalid nlp_cc binary dictionary (keyTotalLength)");
        }
        dict->_key_buffer.resize(keyTotalLength);
        unitsRead = fread(const_cast<char *>(dict->_key_buffer.c_str()), sizeof(char),
                          keyTotalLength, fp);
        if (unitsRead != keyTotalLength) {
            throw InvalidFormat("Invalid nlp_cc binary dictionary (keyBuffer)");
        }

        // Values
        size_t valueTotalLength;
        unitsRead = fread(&valueTotalLength, sizeof(size_t), 1, fp);
        if (unitsRead != 1) {
            throw InvalidFormat("Invalid nlp_cc binary dictionary (valueTotalLength)");
        }
        dict->_value_buffer.resize(valueTotalLength);
        unitsRead = fread(const_cast<char *>(dict->_value_buffer.c_str()), sizeof(char),
                          valueTotalLength, fp);
        if (unitsRead != valueTotalLength) {
            throw InvalidFormat("Invalid nlp_cc binary dictionary (valueBuffer)");
        }

        // Offsets
        for (size_t i = 0; i < numItems; i++) {
            // Number of values
            size_t numValues;
            unitsRead = fread(&numValues, sizeof(size_t), 1, fp);
            if (unitsRead != 1) {
                throw InvalidFormat("Invalid nlp_cc binary dictionary (numValues)");
            }
            // Key offset
            size_t keyOffset;
            unitsRead = fread(&keyOffset, sizeof(size_t), 1, fp);
            if (unitsRead != 1 || keyOffset >= offsetBound) {
                throw InvalidFormat("Invalid nlp_cc binary dictionary (keyOffset)");
            }
            std::string key = dict->_key_buffer.c_str() + keyOffset;
            // Value offset
            std::vector<std::string> values;
            for (size_t j = 0; j < numValues; j++) {
                size_t valueOffset;
                unitsRead = fread(&valueOffset, sizeof(size_t), 1, fp);
                if (unitsRead != 1 || valueOffset >= offsetBound) {
                    throw InvalidFormat("Invalid nlp_cc binary dictionary (valueOffset)");
                }
                const char *value = dict->_value_buffer.c_str() + valueOffset;
                values.push_back(value);
            }
            dict_entry *entry = dict_entry_factory::create(key, values);
            dict->_lex->add(entry);
        }

        return dict;
    }

    void binary_dict::ConstructBuffer(std::string &keyBuf,
                                      std::vector<size_t> &keyOffset,
                                      size_t &keyTotalLength, std::string &valueBuf,
                                      std::vector<size_t> &valueOffset,
                                      size_t &valueTotalLength) const {
        keyTotalLength = 0;
        valueTotalLength = 0;
        // Calculate total length
        for (const std::unique_ptr<dict_entry> &entry : *_lex) {
            keyTotalLength += entry->key_length() + 1;
            assert(entry->num_values() != 0);
            if (entry->num_values() == 1) {
                const auto *svEntry =
                        static_cast<const single_value_dict_entry *>(entry.get());
                valueTotalLength += svEntry->value().length() + 1;
            } else {
                const auto *mvEntry =
                        static_cast<const multi_value_dict_entry *>(entry.get());
                for (const auto &value : mvEntry->values()) {
                    valueTotalLength += value.length() + 1;
                }
            }
        }
        // Write keys and values to buffers
        keyBuf.resize(keyTotalLength, '\0');
        valueBuf.resize(valueTotalLength, '\0');
        char *pKeyBuffer = const_cast<char *>(keyBuf.c_str());
        char *pValueBuffer = const_cast<char *>(valueBuf.c_str());
        for (const std::unique_ptr<dict_entry> &entry : *_lex) {
            strcpy(pKeyBuffer, entry->key().c_str());
            keyOffset.push_back(pKeyBuffer - keyBuf.c_str());
            pKeyBuffer += entry->key_length() + 1;
            if (entry->num_values() == 1) {
                const auto *svEntry =
                        static_cast<const single_value_dict_entry *>(entry.get());
                strcpy(pValueBuffer, svEntry->value().c_str());
                valueOffset.push_back(pValueBuffer - valueBuf.c_str());
                pValueBuffer += svEntry->value().length() + 1;
            } else {
                const auto *mvEntry =
                        static_cast<const multi_value_dict_entry *>(entry.get());
                for (const auto &value : mvEntry->values()) {
                    strcpy(pValueBuffer, value.c_str());
                    valueOffset.push_back(pValueBuffer - valueBuf.c_str());
                    pValueBuffer += value.length() + 1;
                }
            }
        }
        assert(keyBuf.c_str() + keyTotalLength == pKeyBuffer);
        assert(valueBuf.c_str() + valueTotalLength == pValueBuffer);
    }
}  // libnlp::dict