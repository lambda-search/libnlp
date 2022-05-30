/*
 * Open Chinese convert
 *
 * Copyright 2010-2014 Carbo Kuo <byvoid@byvoid.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBNLP_CC_SIMPLE_CONVERTER_H_
#define LIBNLP_CC_SIMPLE_CONVERTER_H_

#include "libnlp/base/export.h"
#include <string>

/**
 * @defgroup nlpcc_simple_api nlpcc C++ Simple API
 *
 * Simple API in C++ language
 */

namespace libnlp::cc {
    /**
     * A high level converter
     * This interface does not require C++11 to compile.
     * @ingroup nlpcc_simple_api
     */
    class LIBNLP_EXPORT simple_converter {
    public:
        /**
         * Constructor of simple_converter
         * @param configFileName File name of configuration.
         */
        explicit simple_converter(const std::string &configFileName);

        ~simple_converter();

        /**
         * Converts a text
         * @param input Text to be converted.
         */
        std::string convert(const std::string &input) const;

        /**
         * Converts a text
         * @param input A C-Style std::string (terminated by '\0') to be converted.
         */
        std::string convert(const char *input) const;

        /**
         * Converts a text
         * @param input  A C-Style std::string limited by a given length to be
         * converted.
         * @param length Maximal length in byte of the input std::string.
         */
        std::string convert(const char *input, size_t length) const;

        /**
         * Converts a text and writes to an allocated buffer
         * Please make sure the buffer has sufficent space.
         * @param input  A C-Style std::string (terminated by '\0') to be converted.
         * @param output Buffer to write the converted text.
         * @return       Length of converted text.
         */
        size_t convert(const char *input, char *output) const;

        /**
         * Converts a text and writes to an allocated buffer
         * Please make sure the buffer has sufficent space.
         * @param input  A C-Style std::string limited by a given length to be
         * converted.
         * @param length Maximal length in byte of the input std::string.
         * @param output Buffer to write the converted text.
         * @return       Length of converted text.
         */
        size_t convert(const char *input, size_t length, char *output) const;

    private:
        const void *_internal_data;
    };

} // namespace libnlp::cc

#endif  // LIBNLP_CC_SIMPLE_CONVERTER_H_

