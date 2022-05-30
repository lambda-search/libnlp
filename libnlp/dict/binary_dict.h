
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include "libnlp/base/common.h"
#include "libnlp/dict/serializable_dict.h"

namespace libnlp::dict {
    /**
     * Binary dictionary for faster deserialization
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT binary_dict : public serializable_dict {
    public:
        binary_dict(const lexicon_ptr &l) : _lex(l) {}

        virtual ~binary_dict() {}

        virtual void serialize_to_file(FILE *fp) const;

        static binary_dict_ptr create_from_file(FILE *fp);

        const lexicon_ptr &get_lexicon() const { return _lex; }

        size_t key_max_length() const;

    private:
        lexicon_ptr _lex;
        std::string _key_buffer;
        std::string _value_buffer;

        void ConstructBuffer(std::string &keyBuffer, std::vector<size_t> &keyOffset,
                             size_t &keyTotalLength, std::string &valueBuffer,
                             std::vector<size_t> &valueOffset,
                             size_t &valueTotalLength) const;
    };
} // namespace libnlp::dict
