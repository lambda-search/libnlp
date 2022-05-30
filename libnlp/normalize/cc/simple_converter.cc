
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#ifdef _MSC_VER
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#endif // _MSC_VER

#include "libnlp/normalize/cc/cc_config.h"
#include "libnlp/normalize/cc/converter.h"
#include "libnlp/base/utf8_util.h"
#include "libnlp/normalize/cc/simple_converter.h"
#include "libnlp_cc.h"

using namespace libnlp::cc;

struct internal_data {
    const converter_ptr _converter;

    internal_data(const converter_ptr &conv) : _converter(conv) {}
};

simple_converter::simple_converter(const std::string &configFileName) {
    try {
        cc_config config;
        _internal_data = new internal_data(config.create_from_file(configFileName));
    } catch (Exception &ex) {
        throw std::runtime_error(ex.what());
    }
}

simple_converter::~simple_converter() { delete (internal_data *) _internal_data; }

std::string simple_converter::convert(const std::string &input) const {
    try {
        const internal_data *data = (internal_data *) _internal_data;
        return data->_converter->convert(input);
    } catch (Exception &ex) {
        throw std::runtime_error(ex.what());
    }
}

std::string simple_converter::convert(const char *input) const {
    return convert(std::string(input));
}

std::string simple_converter::convert(const char *input, size_t length) const {
    if (length == static_cast<size_t>(-1)) {
        return convert(std::string(input));
    } else {
        return convert(UTF8Util::FromSubstr(input, length));
    }
}

size_t simple_converter::convert(const char *input, char *output) const {
    try {
        const internal_data *data = (internal_data *) _internal_data;
        return data->_converter->convert(input, output);
    } catch (Exception &ex) {
        throw std::runtime_error(ex.what());
    }
}

size_t simple_converter::convert(const char *input, size_t length,
                                char *output) const {
    if (length == static_cast<size_t>(-1)) {
        return convert(input, output);
    } else {
        std::string trimmed = UTF8Util::FromSubstr(input, length);
        return convert(trimmed.c_str(), output);
    }
}

static std::string cError;

nlpcc_t nlp_cc_open_internal(const char *configFileName) {
    try {
        if (configFileName == nullptr) {
            configFileName = NLP_CC_DEFAULT_CONFIG_SIMP_TO_TRAD;
        }
        simple_converter *instance = new simple_converter(configFileName);
        return instance;
    } catch (std::runtime_error &ex) {
        cError = ex.what();
        return reinterpret_cast<nlpcc_t>(-1);
    }
}

#ifdef _MSC_VER
nlpcc_t nlp_cc_open_w(const wchar_t* configFileName) {
  try {
    if (configFileName == nullptr) {
      return nlp_cc_open_internal(nullptr);
    }
    std::string utf8fn = UTF8Util::U16ToU8(configFileName);
    return nlp_cc_open_internal(utf8fn.c_str());
  } catch (std::runtime_error& ex) {
    cError = ex.what();
    return reinterpret_cast<nlpcc_t>(-1);
  }
}
nlpcc_t nlp_cc_open(const char* configFileName) {
  if (configFileName == nullptr) {
    return nlp_cc_open_internal(nullptr);
  }
  std::wstring wFileName;
  int convcnt = MultiByteToWideChar(CP_ACP, 0, configFileName, -1, NULL, 0);
  if (convcnt > 0) {
    wFileName.resize(convcnt);
    MultiByteToWideChar(CP_ACP, 0, configFileName, -1, &wFileName[0], convcnt);
  }
  return nlp_cc_open_w(wFileName.c_str());
}
#else

nlpcc_t nlp_cc_open(const char *configFileName) {
    return nlp_cc_open_internal(configFileName);
}

#endif

int nlp_cc_close(nlpcc_t cc) {
    simple_converter *instance = reinterpret_cast<simple_converter *>(cc);
    delete instance;
    return 0;
}

size_t nlp_cc_convert_utf8_to_buffer(nlpcc_t cc, const char *input,
                                     size_t length, char *output) {
    try {
        simple_converter *instance = reinterpret_cast<simple_converter *>(cc);
        return instance->convert(input, length, output);
    } catch (std::runtime_error &ex) {
        cError = ex.what();
        return static_cast<size_t>(-1);
    }
}

char *nlp_cc_convert_utf8(nlpcc_t cc, const char *input, size_t length) {
    try {
        simple_converter *instance = reinterpret_cast<simple_converter *>(cc);
        std::string converted = instance->convert(input, length);
        char *output = new char[converted.length() + 1];
        strncpy(output, converted.c_str(), converted.length());
        output[converted.length()] = '\0';
        return output;
    } catch (std::runtime_error &ex) {
        cError = ex.what();
        return nullptr;
    }
}

void nlp_cc_convert_utf8_free(char *str) { delete[] str; }

const char *nlp_cc_error(void) { return cError.c_str(); }
