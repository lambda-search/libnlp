
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include <cassert>
#include <cstring>

#include "libnlp/dict/lexicon.h"
#include "libnlp/dict/serialized_values.h"

namespace libnlp::dict {

    namespace {

        template<typename INT_TYPE>
        INT_TYPE ReadInteger(FILE *fp) {
            INT_TYPE num;
            size_t unitsRead = fread(&num, sizeof(INT_TYPE), 1, fp);
            if (unitsRead != 1) {
                throw InvalidFormat("Invalid nlp_cc binary dictionary.");
            }
            return num;
        }

        template<typename INT_TYPE>
        void WriteInteger(FILE *fp, INT_TYPE num) {
            size_t unitsWritten = fwrite(&num, sizeof(INT_TYPE), 1, fp);
            if (unitsWritten != 1) {
                throw InvalidFormat("Cannot write binary dictionary.");
            }
        }

    } // namespace

    size_t serialized_values::key_max_length() const { return 0; }

    void serialized_values::serialize_to_file(FILE *fp) const {
        std::string valueBuf;
        std::vector<uint16_t> valueBytes;
        uint32_t valueTotalLength = 0;
        construct_buffer(&valueBuf, &valueBytes, &valueTotalLength);
        // Number of items
        uint32_t numItems = static_cast<uint32_t>(_lex->length());
        WriteInteger(fp, numItems);

        // Data
        WriteInteger(fp, valueTotalLength);
        fwrite(valueBuf.c_str(), sizeof(char), valueTotalLength, fp);

        size_t valueCursor = 0;
        for (const std::unique_ptr<dict_entity> &entry : *_lex) {
            // Number of values
            uint16_t numValues = static_cast<uint16_t>(entry->num_values());
            WriteInteger(fp, numValues);
            // Values offset
            for (uint16_t i = 0; i < numValues; i++) {
                uint16_t numValueBytes = valueBytes[valueCursor++];
                WriteInteger(fp, numValueBytes);
            }
        }
    }

    std::shared_ptr<serialized_values> serialized_values::create_from_file(FILE *fp) {
        std::shared_ptr<serialized_values> dict(
                new serialized_values(lexicon_ptr(new lexicon)));

        // Number of items
        uint32_t numItems = ReadInteger<uint32_t>(fp);

        // Values
        uint32_t valueTotalLength = ReadInteger<uint32_t>(fp);
        std::string valueBuffer;
        valueBuffer.resize(valueTotalLength);
        size_t unitsRead = fread(const_cast<char *>(valueBuffer.c_str()), sizeof(char),
                                 valueTotalLength, fp);
        if (unitsRead != valueTotalLength) {
            throw InvalidFormat("Invalid nlp_cc binary dictionary (valueBuffer)");
        }

        // Offsets
        const char *pValueBuffer = valueBuffer.c_str();
        for (uint32_t i = 0; i < numItems; i++) {
            // Number of values
            uint16_t numValues = ReadInteger<uint16_t>(fp);
            // Value offset
            std::vector<std::string> values;
            for (uint16_t j = 0; j < numValues; j++) {
                const char *value = pValueBuffer;
                uint16_t numValueBytes = ReadInteger<uint16_t>(fp);
                pValueBuffer += numValueBytes;
                values.push_back(value);
            }
            dict_entity *entry = dict_entity::create("", values);
            dict->_lex->add(entry);
        }

        return dict;
    }

    void serialized_values::construct_buffer(std::string *valueBuffer,
                                             std::vector<uint16_t> *valueBytes,
                                             uint32_t *valueTotalLength) const {
        *valueTotalLength = 0;
        // Calculate total length.
        for (const std::unique_ptr<dict_entity> &entry : *_lex) {
            assert(entry->num_values() != 0);
            for (const auto &value : entry->values()) {
                *valueTotalLength += static_cast<uint32_t>(value.length()) + 1;
            }
        }
        // Write values to the buffer.
        valueBuffer->resize(*valueTotalLength, '\0');
        char *pValueBuffer = const_cast<char *>(valueBuffer->c_str());
        for (const std::unique_ptr<dict_entity> &entry : *_lex) {
            for (const auto &value : entry->values()) {
                strcpy(pValueBuffer, value.c_str());
                valueBytes->push_back(static_cast<uint16_t>(value.length() + 1));
                pValueBuffer += value.length() + 1;
            }
        }
        assert(valueBuffer->c_str() + *valueTotalLength == pValueBuffer);
    }
}