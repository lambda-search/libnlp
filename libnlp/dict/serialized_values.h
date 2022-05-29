

#pragma once

#include "libnlp/base/common.h"
#include "libnlp/dict/serializable_dict.h"

namespace libnlp::dict {
    /**
     * Binary format for dictionary values serialization.
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT serialized_values : public serializable_dict {
    public:
        explicit serialized_values(const lexicon_ptr &l) : _lex(l) {}

        virtual ~serialized_values() = default;

        void serialize_to_file(FILE *fp) const override;

        static std::shared_ptr<serialized_values> create_from_file(FILE *fp);

        const lexicon_ptr &get_lexicon() const { return _lex; }

        size_t key_max_length() const;

    private:
        lexicon_ptr _lex;

        void construct_buffer(std::string *valueBuffer,
                              std::vector<uint16_t> *valueBytes,
                              uint32_t *valueTotalLength) const;
    };
} // namespace libnlp::dict
