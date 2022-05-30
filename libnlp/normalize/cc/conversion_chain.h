
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <list>

#include "libnlp/base/common.h"
#include "libnlp/normalize/cc/conversion.h"

namespace libnlp::cc {
    /**
     * Chain of conversions
     * Consists of a list of conversions. Converts input in sequence.
     * @ingroup lib_cc_cpp_api
     */
    class LIBNLP_EXPORT conversion_chain {
    public:
        explicit conversion_chain(const std::list<conversion_ptr> _conversions);

        segments_ptr convert(const segments_ptr &input) const;

        const std::list<conversion_ptr> get_conversions() const { return _conversions; }

    private:
        const std::list<conversion_ptr> _conversions;
    };
} // namespace libnlp::cc
