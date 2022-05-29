
#pragma once

#include <list>

#include "libnlp/base/common.h"
#include "libnlp/dict/dict_base.h"

namespace libnlp::dict {
    /**
     * Group of dictionaries
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT dict_group : public basic_dict {
    public:
        explicit dict_group(const std::list<basic_dict_ptr> &dicts);

        static dict_group_ptr create_from_dict(const basic_dict &dict);

        virtual ~dict_group();

        std::string type() const override { return "group"; }

        size_t key_max_length() const override;

        std::optional<const dict_entry *> match(const char *word, size_t len) const override;

        std::optional<const dict_entry *> match_prefix(const char *word,
                                                       size_t len) const override;

        std::vector<const dict_entry *> match_all_prefixes(const char *word,
                                                           size_t len) const override;

        lexicon_ptr get_lexicon() const override;

        const std::list<basic_dict_ptr> get_dicts() const { return _dicts; }

    private:
        const size_t _key_max_length;
        const std::list<basic_dict_ptr> _dicts;
    };
} // namespace libnlp::dict
