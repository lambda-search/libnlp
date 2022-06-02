//
// Created by liyinbin on 2022/6/2.
//

#include "libnlp/dict/dict_entity.h"

namespace libnlp::dict {

    std::string dict_entity::to_string() const {
        // TODO escape space
        if(_key.empty() && _values.empty()) {
            return "empty";
        }
        size_t i = 0;
        size_t length = values().size();
        std::ostringstream buffer;
        if(_values.size() == 0) {
            buffer << _key;
        } else if(_values.size() == 1) {
            buffer << _key << '\t'<<_values[0];
        } else {
            buffer << _key << '\t';

            for (const std::string &value : _values) {
                buffer << value;
                if (i < length - 1) {
                    buffer << ' ';
                }
                i++;
            }
        }
        return buffer.str();
    }
}