
#pragma once

#include "libnlp/dict/dict_base.h"

namespace libnlp::dict {
    /**
     * Serializable dictionary interface
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT serializable_dict {
    public:
        /**
         * Serializes the dictionary and writes in to a file.
         */
        virtual void serialize_to_file(FILE *fp) const = 0;

        /**
         * Serializes the dictionary and writes in to a file.
         */
        virtual void serialize_to_file(const std::string &fileName) const {
            FILE *fp = fopen(fileName.c_str(), "wb");
            if (fp == nullptr) {
                throw FileNotWritable(fileName);
            }
            serialize_to_file(fp);
            fclose(fp);
        }

        template<typename DICT>
        static bool try_load_from_file(const std::string &fileName,
                                    std::shared_ptr<DICT> *dict) {
            FILE *fp =
#ifdef _MSC_VER
                    // well, the 'get_platform_string' shall return a 'wstring'
                    _wfopen(UTF8Util::get_platform_string(fileName).c_str(), L"rb")
#else
                    fopen(UTF8Util::get_platform_string(fileName).c_str(), "rb")
#endif // _MSC_VER
            ;

            if (fp == nullptr) {
                return false;
            }
            std::shared_ptr<DICT> loadedDict = DICT::create_from_file(fp);
            fclose(fp);
            *dict = loadedDict;
            return true;
        }

        template<typename DICT>
        static std::shared_ptr<DICT> create_from_file(const std::string &fileName) {
            std::shared_ptr<DICT> dict;
            if (!try_load_from_file<DICT>(fileName, &dict)) {
                throw FileNotFound(fileName);
            }
            return dict;
        }
    };
} // namespace libnlp::dict
