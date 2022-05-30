
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

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
