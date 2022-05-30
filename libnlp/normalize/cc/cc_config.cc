
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include <fstream>
#include <list>
#include <unordered_map>

#include <rapidjson/document.h>

#include "libnlp/normalize/cc/cc_config.h"
#include "libnlp/normalize/cc/conversion_chain.h"
#include "libnlp/normalize/cc/converter.h"
#include "libnlp/dict/dict_group.h"
#include "libnlp/base/exception.h"
#include "libnlp/dict/marisa_dict.h"
#include "libnlp/normalize/cc/max_match_segmentation.h"
#include "libnlp/dict/text_dict.h"
#include "libnlp/dict/darts_dict.h"

typedef rapidjson::GenericValue <rapidjson::UTF8<char>> JSONValue;

namespace libnlp::cc {

    namespace {

        class cc_config_internal {
        public:
            std::string _configDirectory;
            std::unordered_map<
                    std::string,
                    std::unordered_map<std::string, std::unordered_map<std::string, libnlp::dict::basic_dict_ptr>>>
                    _dictCache;

            const JSONValue &get_property(const JSONValue &doc, const char *name) {
                if (!doc.HasMember(name)) {
                    throw InvalidFormat("Required property not found: " + std::string(name));
                }
                return doc[name];
            }

            const JSONValue &get_object_property(const JSONValue &doc, const char *name) {
                const JSONValue &obj = get_property(doc, name);
                if (!obj.IsObject()) {
                    throw InvalidFormat("Property must be an object: " + std::string(name));
                }
                return obj;
            }

            const JSONValue &get_array_property(const JSONValue &doc, const char *name) {
                const JSONValue &obj = get_property(doc, name);
                if (!obj.IsArray()) {
                    throw InvalidFormat("Property must be an array: " + std::string(name));
                }
                return obj;
            }

            const char *get_string_property(const JSONValue &doc, const char *name) {
                const JSONValue &obj = get_property(doc, name);
                if (!obj.IsString()) {
                    throw InvalidFormat("Property must be a std::string: " +
                                        std::string(name));
                }
                return obj.GetString();
            }

            template<typename DICT>
            libnlp::dict::basic_dict_ptr load_dict_with_paths(const std::string &fileName) {
                // Working directory
                std::shared_ptr<DICT> dict;
                if (libnlp::dict::serializable_dict::try_load_from_file<DICT>(fileName, &dict)) {
                    return dict;
                }
                // Configuration directory
                if ((_configDirectory != "") && libnlp::dict::serializable_dict::try_load_from_file<DICT>(
                        _configDirectory + fileName, &dict)) {
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

            libnlp::dict::basic_dict_ptr load_dict_from_file(const std::string &type,
                                     const std::string &fileName) {
                if (type == "text") {
                    libnlp::dict::basic_dict_ptr dict = load_dict_with_paths<libnlp::dict::text_dict>(fileName);
                    return libnlp::dict::marisa_dict::create_from_dict(*dict.get());
                }
                if (type == "ocd") {
                    return load_dict_with_paths<libnlp::dict::darts_dict>(fileName);
                }
                if (type == "ocd2") {
                    return load_dict_with_paths<libnlp::dict::marisa_dict>(fileName);
                }
                throw InvalidFormat("Unknown dictionary type: " + type);
                return nullptr;
            }

            libnlp::dict::basic_dict_ptr parse_dict(const JSONValue &doc) {
                // Required: type
                std::string type = get_string_property(doc, "type");

                if (type == "group") {
                    std::list<libnlp::dict::basic_dict_ptr> dicts;
                    const JSONValue &docs = get_array_property(doc, "dicts");
                    for (rapidjson::SizeType i = 0; i < docs.Size(); i++) {
                        if (docs[i].IsObject()) {
                            libnlp::dict::basic_dict_ptr dict = parse_dict(docs[i]);
                            dicts.push_back(dict);
                        } else {
                            throw InvalidFormat("Element of the array must be an object");
                        }
                    }
                    return libnlp::dict::dict_group_ptr(new libnlp::dict::dict_group(dicts));
                } else {
                    std::string fileName = get_string_property(doc, "file");
                    // Read from cache
                    libnlp::dict::basic_dict_ptr &cache = _dictCache[type][_configDirectory][fileName];
                    if (cache != nullptr) {
                        return cache;
                    }
                    libnlp::dict::basic_dict_ptr dict = load_dict_from_file(type, fileName);

                    // Update Cache
                    cache = dict;
                    return dict;
                }
            }

            segmentation_ptr parse_segmentation(const JSONValue &doc) {
                segmentation_ptr segmentation;

                // Required: type
                std::string type = get_string_property(doc, "type");
                if (type == "mmseg") {
                    // Required: dict
                    libnlp::dict::basic_dict_ptr dict = parse_dict(get_object_property(doc, "dict"));
                    segmentation = segmentation_ptr(new max_match_segmentation(dict));
                } else {
                    throw InvalidFormat("Unknown segmentation type: " + type);
                }
                return segmentation;
            }

            conversion_ptr parse_conversion(const JSONValue &doc) {
                // Required: dict
                libnlp::dict::basic_dict_ptr dict = parse_dict(get_object_property(doc, "dict"));
                conversion_ptr conv(new conversion(dict));

                return conv;
            }

            conversion_chain_ptr parse_conversion_chain(const JSONValue &docs) {
                std::list<conversion_ptr> conversions;
                for (rapidjson::SizeType i = 0; i < docs.Size(); i++) {
                    const JSONValue &doc = docs[i];
                    if (doc.IsObject()) {
                        conversion_ptr conversion = parse_conversion(doc);
                        conversions.push_back(conversion);
                    } else {
                    }
                }
                conversion_chain_ptr chain(new conversion_chain(conversions));
                return chain;
            }

            std::string find_config_file(std::string fileName) {
                std::ifstream ifs;

                // Working directory
                ifs.open(UTF8Util::get_platform_string(fileName).c_str());
                if (ifs.is_open()) {
                    return fileName;
                }
                // Package data directory
                if (PACKAGE_DATA_DIRECTORY != "") {
                    std::string prefixedFileName = PACKAGE_DATA_DIRECTORY + fileName;
                    ifs.open(UTF8Util::get_platform_string(prefixedFileName).c_str());
                    if (ifs.is_open()) {
                        return prefixedFileName;
                    }
                    prefixedFileName += ".json";
                    ifs.open(UTF8Util::get_platform_string(prefixedFileName).c_str());
                    if (ifs.is_open()) {
                        return prefixedFileName;
                    }
                }
                throw FileNotFound(fileName);
            }
        };
    } // namespace

    cc_config::cc_config() : _internal(new cc_config_internal()) {}

    cc_config::~cc_config() { delete (cc_config_internal *) _internal; }

    converter_ptr cc_config::create_from_file(const std::string &fileName) {
        cc_config_internal *impl = (cc_config_internal *) _internal;
        std::string prefixedFileName = impl->find_config_file(fileName);
        std::ifstream ifs(UTF8Util::get_platform_string(prefixedFileName));
        std::string content(std::istreambuf_iterator<char>(ifs),
                            (std::istreambuf_iterator<char>()));

#if defined(_WIN32) || defined(_WIN64)
        UTF8Util::ReplaceAll(prefixedFileName, "\\", "/");
#endif // if defined(_WIN32) || defined(_WIN64)
        size_t slashPos = prefixedFileName.rfind("/");
        std::string configDirectory = "";
        if (slashPos != std::string::npos) {
            configDirectory = prefixedFileName.substr(0, slashPos) + "/";
        }
        return create_from_string(content, configDirectory);
    }

    converter_ptr cc_config::create_from_string(const std::string &json,
                                       const std::string &configDirectory) {
        rapidjson::Document doc;

        doc.ParseInsitu<0>(const_cast<char *>(json.c_str()));
        if (doc.HasParseError()) {
            throw InvalidFormat("Error parsing JSON"); // doc.GetErrorOffset()
        }
        if (!doc.IsObject()) {
            throw InvalidFormat("Root of configuration must be an object");
        }

        // std::optional: name
        std::string name;
        if (doc.HasMember("name") && doc["name"].IsString()) {
            name = doc["name"].GetString();
        }

        cc_config_internal *impl = (cc_config_internal *) _internal;
        if (!configDirectory.empty()) {
            if (configDirectory.back() == '/' || configDirectory.back() == '\\')
                impl->_configDirectory = configDirectory;
            else
                impl->_configDirectory = configDirectory + '/';
        } else {
            impl->_configDirectory.clear();
        }

        // Required: segmentation
        segmentation_ptr segmentation =
                impl->parse_segmentation(impl->get_object_property(doc, "segmentation"));

        // Required: conversion_chain
        conversion_chain_ptr chain = impl->parse_conversion_chain(
                impl->get_array_property(doc, "conversion_chain"));
        return converter_ptr(new converter(name, segmentation, chain));
    }

}; // namespace libnlp::cc
