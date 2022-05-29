
#include <algorithm>
#include <map>

#include "libnlp/dict/dict_group.h"
#include "libnlp/dict/lexicon.h"
#include "libnlp/dict/text_dict.h"

namespace libnlp::dict {

    namespace {

        size_t get_key_max_length(const std::list<basic_dict_ptr> &dicts) {
            size_t keyMaxLength = 0;
            for (const basic_dict_ptr &dict : dicts) {
                keyMaxLength = (std::max)(keyMaxLength, dict->key_max_length());
            }
            return keyMaxLength;
        }

    } // namespace

    dict_group::dict_group(const std::list<basic_dict_ptr> &ds)
            : _key_max_length(get_key_max_length(ds)), _dicts(ds) {}

    dict_group::~dict_group() {}

    size_t dict_group::key_max_length() const { return _key_max_length; }

    std::optional<const dict_entry *> dict_group::match(const char *word,
                                                        size_t len) const {
        for (const auto &dict : _dicts) {
            const std::optional<const dict_entry *> &prefix = dict->match(word, len);
            if (prefix) {
                return prefix;
            }
        }
        return std::nullopt;
    }

    std::optional<const dict_entry *> dict_group::match_prefix(const char *word,
                                                               size_t len) const {
        for (const auto &dict : _dicts) {
            const std::optional<const dict_entry *> &prefix = dict->match_prefix(word, len);
            if (prefix) {
                return prefix;
            }
        }
        return std::nullopt;
    }

    std::vector<const dict_entry *> dict_group::match_all_prefixes(const char *word,
                                                                   size_t len) const {
        std::map<size_t, const dict_entry *> matched;
        // match all prefixes from all dictionaries
        for (const auto &dict : _dicts) {
            const std::vector<const dict_entry *> &entries =
                    dict->match_all_prefixes(word, len);
            for (const auto &entry : entries) {
                size_t entryLen = entry->key_length();
                // If the current length has already result, skip
                if (matched.find(entryLen) == matched.end()) {
                    matched[entryLen] = entry;
                }
            }
        }
        std::vector<const dict_entry *> matchedEntries;
        for (auto i = matched.rbegin(); i != matched.rend(); i++) {
            matchedEntries.push_back(i->second);
        }
        return matchedEntries;
    }

    lexicon_ptr dict_group::get_lexicon() const {
        lexicon_ptr allLexicon(new lexicon);
        for (const auto &dict : _dicts) {
            const auto &lexicon = dict->get_lexicon();
            for (const std::unique_ptr<dict_entry> &item : *lexicon) {
                allLexicon->add(dict_entry_factory::create(item.get()));
            }
        }
        allLexicon->sort();
        // Fixme deduplicate
        return allLexicon;
    }

    dict_group_ptr dict_group::create_from_dict(const basic_dict &dict) {
        text_dict_ptr newDict = text_dict::create_from_dict(dict);
        return dict_group_ptr(new dict_group(std::list<basic_dict_ptr>{newDict}));
    }
}  // namespace libnlp::dict