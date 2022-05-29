

#pragma once

#include "libnlp/base/common.h"
#include "libnlp/dict/serializable_dict.h"

namespace libnlp::dict {
    /**
     * Text dictionary
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT text_dict : public basic_dict, public serializable_dict {
    public:
        /**
         * Constructor of text_dict.
         * _lexicon must be sorted.
         */
        explicit text_dict(const lexicon_ptr &_lexicon);

        virtual ~text_dict();

        std::string type() const override { return "text"; }

        size_t key_max_length() const override;

        std::optional<const dict_entry *> match(const char *word, size_t len) const override;

        lexicon_ptr get_lexicon() const override;

        void serialize_to_file(FILE *fp) const override;

        /**
         * Constructs a text_dict from another dictionary.
         */
        static text_dict_ptr create_from_dict(const basic_dict &dict);

        static text_dict_ptr create_from_file(FILE *fp);

        static text_dict_ptr create_from_sorted_file(FILE *fp);

    private:
        const size_t _max_length;
        const lexicon_ptr _lex;
    };
} // namespace libnlp::dict
