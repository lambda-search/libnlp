/*
 * Open Chinese convert
 *
 * Copyright 2010-2017 Carbo Kuo <byvoid@byvoid.com>
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

#pragma once

#include "libnlp/base/common.h"

namespace libnlp::dict {
    /**
     * Converts a dictionary from a format to another.
     * @ingroup libnlp_dict_api
     */
    LIBNLP_EXPORT void convert_dictionary(const std::string inputFileName,
                                         const std::string outputFileName,
                                         const std::string formatFrom,
                                         const std::string formatTo);
} // namespace libnlp::dict
