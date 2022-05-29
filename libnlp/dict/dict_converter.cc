

#include "libnlp/dict/dict_converter.h"
#include "libnlp/dict/marisa_dict.h"
#include "libnlp/dict/text_dict.h"
#include "libnlp/dict/darts_dict.h"

namespace libnlp::dict {

    namespace {
        basic_dict_ptr load_dictionary(const std::string &format,
                               const std::string &inputFileName) {
            if (format == "text") {
                return serializable_dict::create_from_file<text_dict>(inputFileName);
            } else if (format == "ocd") {
                return serializable_dict::create_from_file<darts_dict>(inputFileName);
            } else if (format == "ocd2") {
                return serializable_dict::create_from_file<marisa_dict>(inputFileName);
            }
            fprintf(stderr, "Unknown dictionary format: %s\n", format.c_str());
            exit(2);
            return nullptr;
        }

        serializable_dict_ptr convert_dict(const std::string &format, const basic_dict_ptr dict) {
            if (format == "text") {
                return text_dict::create_from_dict(*dict.get());
            } else if (format == "ocd") {
                return darts_dict::create_from_dict(*dict.get());
            } else if (format == "ocd2") {
                return marisa_dict::create_from_dict(*dict.get());
            }
            fprintf(stderr, "Unknown dictionary format: %s\n", format.c_str());
            exit(2);
            return nullptr;
        }
    }  // namespace

    void convert_dictionary(const std::string inputFileName,
                           const std::string outputFileName,
                           const std::string formatFrom,
                           const std::string formatTo) {
        basic_dict_ptr dictFrom = load_dictionary(formatFrom, inputFileName);
        serializable_dict_ptr dictTo = convert_dict(formatTo, dictFrom);
        dictTo->serialize_to_file(outputFileName);
    }
} // namespace libnlp::dict
