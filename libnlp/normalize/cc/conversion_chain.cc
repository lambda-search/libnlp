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

#include <list>

#include "libnlp/normalize/cc/conversion_chain.h"
#include "libnlp/normalize/cc/segments.h"

namespace libnlp::cc {

    conversion_chain::conversion_chain(const std::list<conversion_ptr> _conversions)
            : _conversions(_conversions) {}

    segments_ptr conversion_chain::convert(const segments_ptr &input) const {
        segments_ptr output = input;
        for (auto conversion : _conversions) {
            output = conversion->convert(output);
        }
        return output;
    }
}