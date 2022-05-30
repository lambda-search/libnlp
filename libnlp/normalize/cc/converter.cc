
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include <cstring>

#include "libnlp/normalize/cc/conversion_chain.h"
#include "libnlp/normalize/cc/converter.h"
#include "libnlp/normalize/cc/segments.h"

using namespace libnlp::cc;

std::string converter::convert(const std::string &text) const {
    const segments_ptr &segments = _segmentation->segment(text);
    const segments_ptr &converted = _conversion_chain->convert(segments);
    return converted->to_string();
}

size_t converter::convert(const char *input, char *output) const {
    const std::string &converted = convert(input);
    strcpy(output, converted.c_str());
    return converted.length();
}
