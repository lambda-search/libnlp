
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#pragma once

#include "libnlp/base/common.h"

namespace libnlp::cc {
    /**
     * Abstract segmentation
     * @ingroup lib_cc_cpp_api
     */
    class LIBNLP_EXPORT segmentation {
    public:
        virtual segments_ptr segment(const std::string &text) const = 0;
    };
} // namespace libnlp::cc
