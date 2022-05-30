
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/dict/dict_entry.h"

namespace libnlp::dict {

    std::string multi_value_dict_entry::to_string() const {
        // TODO escape space
        size_t i = 0;
        size_t length = values().size();
        std::ostringstream buffer;
        buffer << key() << '\t';
        for (const std::string &value : values()) {
            buffer << value;
            if (i < length - 1) {
                buffer << ' ';
            }
            i++;
        }
        return buffer.str();
    }
}  // libnlp::dict
