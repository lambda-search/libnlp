
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

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