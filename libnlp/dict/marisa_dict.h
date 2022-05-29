

#pragma once

#include "libnlp/base/common.h"
#include "libnlp/dict/serializable_dict.h"

namespace libnlp::dict {
    /**
     * Darts dictionary
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT marisa_dict : public basic_dict, public serializable_dict {
    public:
        virtual ~marisa_dict();

        size_t key_max_length() const override;

        std::string type() const override { return "ocd2"; }

        std::optional<const dict_entry *> match(const char *word, size_t len) const override;

        std::optional<const dict_entry *> match_prefix(const char *word,
                                                        size_t len) const override;

        std::vector<const dict_entry *> match_all_prefixes(const char *word,
                                                                size_t len) const override;

        lexicon_ptr get_lexicon() const override;

        void serialize_to_file(FILE *fp) const override;

        /**
         * Constructs a marisa_dict from another dictionary.
         */
        static marisa_dict_ptr create_from_dict(const basic_dict &thatDict);

        static marisa_dict_ptr create_from_file(FILE *fp);

    private:
        marisa_dict();

        size_t _max_length;
        lexicon_ptr _lex;

        class marisa_internal;

        std::unique_ptr<marisa_internal> _internal;
    };
} // namespace libnlp::dict
