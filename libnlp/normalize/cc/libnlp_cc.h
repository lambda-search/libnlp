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

#ifndef LIBNLP_NORMALIZE_CC_LIBNLP_CC_H_
#define LIBNLP_NORMALIZE_CC_LIBNLP_CC_H_

#ifdef __cplusplus

#include "libnlp/base/export.h"
#include "libnlp/normalize/cc/simple_converter.h"
#include <string>

extern "C" {
#else
#include <stddef.h>
#endif

#ifndef LIBNLP_EXPORT
#define LIBNLP_EXPORT
#endif

/**
 * @defgroup nlp_cc_c_api libnlp C API
 *
 * API in C language
 */

/**
 * Filename of default Simplified to Traditional configuration
 *
 * @ingroup nlp_cc_c_api
 */
#define NLP_CC_DEFAULT_CONFIG_SIMP_TO_TRAD "s2t.json"

/**
 * Filename of default Traditional to Simplified configuration
 *
 * @ingroup nlp_cc_c_api
 */
#define NLP_CC_DEFAULT_CONFIG_TRAD_TO_SIMP "t2s.json"

/**
 * Type of libnlp descriptor
 *
 * @ingroup nlp_cc_c_api
 */
typedef void *nlpcc_t;

/**
 * Makes an instance of libnlp
 *
 * @param configFileName Location of configuration file. If this is set to NULL,
 *                       NLP_CC_DEFAULT_CONFIG_SIMP_TO_TRAD will be loaded.
 * @return            A description pointer of the newly allocated instance of
 *                    nlpcc_t. On error the return value will be (nlpcc_t) -1.
 * @ingroup nlp_cc_c_api
 */
LIBNLP_EXPORT nlpcc_t nlp_cc_open(const char *configFileName);
#ifdef _MSC_VER
/**
 * Makes an instance of libnlp (wide char / Unicode)
 *
 * @param configFileName Location of configuration file. If this is set to NULL,
 *                       NLP_CC_DEFAULT_CONFIG_SIMP_TO_TRAD will be loaded.
 * @return            A description pointer of the newly allocated instance of
 *                    nlpcc. On error the return value will be (nlpcc_t) -1.
 * @ingroup nlp_cc_c_api
 */
LIBNLP_EXPORT nlpcc_t nlp_cc_open_w(const wchar_t* configFileName);
#endif /* _MSC_VER */

/**
 * Destroys an instance of libnlp
 *
 * @param cc The description pointer.
 * @return 0 on success or non-zero number on failure.
 * @ingroup nlp_cc_c_api
 */
LIBNLP_EXPORT int nlp_cc_close(nlpcc_t cc);

/**
 * Converts UTF-8 std::string
 *
 * @param cc The nlpcc_t description pointer.
 * @param input  The UTF-8 encoded std::string.
 * @param length The maximum length in byte to convert. If length is (size_t)-1,
 *               the whole std::string (terminated by '\0') will be converted.
 * @param output The buffer to store converted text. You MUST make sure this
 *               buffer has sufficient space.
 *
 * @return       The length of converted std::string or (size_t)-1 on error.
 *
 * @ingroup nlp_cc_c_api
 */
LIBNLP_EXPORT size_t nlp_cc_convert_utf8_to_buffer(nlpcc_t cc,
                                                   const char *input,
                                                   size_t length, char *output);

/**
 * Converts UTF-8 std::string
 * This function returns an allocated C-Style std::string, which stores
 * the converted std::string.
 * You MUST call nlp_cc_convert_utf8_free() to release allocated memory.
 *
 * @param cc The nlpcc_t description pointer.
 * @param input  The UTF-8 encoded std::string.
 * @param length The maximum length in byte to convert. If length is (size_t)-1,
 *               the whole std::string (terminated by '\0') will be converted.
 *
 * @return       The newly allocated UTF-8 std::string that stores text
 * converted, or NULL on error.
 * @ingroup nlp_cc_c_api
 */
LIBNLP_EXPORT char *nlp_cc_convert_utf8(nlpcc_t cc, const char *input,
                                        size_t length);

/**
 * Releases allocated buffer by nlp_cc_convert_utf8
 *
 * @param str    Pointer to the allocated std::string buffer by
 * nlp_cc_convert_utf8.
 *
 * @ingroup nlp_cc_c_api
 */
LIBNLP_EXPORT void nlp_cc_convert_utf8_free(char *str);

/**
 * Returns the last error message
 *
 * Note that this function is the only one which is NOT thread-safe.
 *
 * @ingroup nlp_cc_c_api
 */
LIBNLP_EXPORT const char *nlp_cc_error(void);

#ifdef __cplusplus
} // extern "C"
#endif

/**
 * @defgroup lib_cc_cpp_api libnlp C++ Comprehensive API
 *
 * Comprehensive API in C++ language
 */

#endif  // LIBNLP_NORMALIZE_CC_LIBNLP_CC_H_
