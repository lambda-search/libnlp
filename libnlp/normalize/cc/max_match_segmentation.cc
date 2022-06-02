
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/normalize/cc/max_match_segmentation.h"
#include "libnlp/normalize/cc/segments.h"

using namespace libnlp::cc;

segments_ptr max_match_segmentation::segment(const std::string &text) const {
    segments_ptr segs(new segments);
    const char *segStart = text.c_str();
    size_t segLength = 0;
    auto clearBuffer = [&segs, &segStart, &segLength]() {
        if (segLength > 0) {
            segs->add_segment(UTF8Util::FromSubstr(segStart, segLength));
            segLength = 0;
        }
    };
    size_t length = text.length();
    for (const char *pstr = text.c_str(); *pstr != '\0';) {
        const std::optional<const libnlp::dict::dict_entity *> &matched = _dict->match_prefix(pstr, length);
        size_t matchedLength;
        if (!matched) {
            matchedLength = UTF8Util::NextCharLength(pstr);
            segLength += matchedLength;
        } else {
            clearBuffer();
            matchedLength = (*matched)->key_length();
            segs->add_segment((*matched)->key());
            segStart = pstr + matchedLength;
        }
        pstr += matchedLength;
        length -= matchedLength;
    }
    clearBuffer();
    return segs;
}
