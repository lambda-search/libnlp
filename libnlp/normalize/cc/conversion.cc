
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/normalize/cc/conversion.h"
#include "libnlp/normalize/cc/segments.h"
#include "libnlp/dict/dict_base.h"

namespace libnlp::cc {
    std::string conversion::convert(const char *phrase) const {
        std::ostringstream buffer;
        for (const char *pstr = phrase; *pstr != '\0';) {
            std::optional<const libnlp::dict::dict_entry *> matched = _dict->match_prefix(pstr);
            size_t matchedLength;
            if (!matched) {
                matchedLength = UTF8Util::NextCharLength(pstr);
                buffer << UTF8Util::FromSubstr(pstr, matchedLength);
            } else {
                matchedLength = (*matched)->key_length();
                buffer << (*matched)->get_default();
            }
            pstr += matchedLength;
        }
        return buffer.str();
    }

    std::string conversion::convert(const std::string &phrase) const {
        return convert(phrase.c_str());
    }

    segments_ptr conversion::convert(const segments_ptr &input) const {
        segments_ptr output(new segments);
        for (const char *segment : *input) {
            output->add_segment(convert(segment));
        }
        return output;
    }
}  // namespace libnlp::cc
