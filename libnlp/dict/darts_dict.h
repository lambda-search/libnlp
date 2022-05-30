
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
     * Darts dictionary
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT darts_dict : public basic_dict, public serializable_dict {
    public:
        virtual ~darts_dict();

        size_t key_max_length() const override;

        std::string type() const override { return "ocd"; }

        std::optional<const dict_entry *> match(const char *word, size_t len) const override;

        std::optional<const dict_entry *> match_prefix(const char *word,
                                                        size_t len) const override;

        lexicon_ptr get_lexicon() const override;

        void serialize_to_file(FILE *fp) const override;

        /**
         * Constructs a darts_dict from another dictionary.
         */
        static darts_dict_ptr create_from_dict(const basic_dict &thatDict);

        static darts_dict_ptr create_from_file(FILE *fp);

    private:
        darts_dict();

        size_t _max_length;
        lexicon_ptr _lex;

        class darts_internal;

        darts_internal *_internal;
    };
} // namespace libnlp::dict
