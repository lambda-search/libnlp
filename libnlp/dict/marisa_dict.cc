
#include <algorithm>
#include <cstring>
#include <unordered_map>

#include "libnlp/trie/marisa.h"

#include "libnlp/dict/lexicon.h"
#include "libnlp/dict/marisa_dict.h"
#include "libnlp/dict/serialized_values.h"

namespace libnlp::dict {

    namespace {
        static const char *OCD2_HEADER = "LIBNLP_CC_MARISA_0.2.0";
    }

    class marisa_dict::marisa_internal {
    public:
        std::unique_ptr<marisa::Trie> marisa;

        marisa_internal() : marisa(new marisa::Trie()) {}
    };

    marisa_dict::marisa_dict() : _internal(new marisa_internal()) {}

    marisa_dict::~marisa_dict() {}

    size_t marisa_dict::key_max_length() const { return _max_length; }

    std::optional<const dict_entry *> marisa_dict::match(const char *word,
                                                         size_t len) const {
        if (len > _max_length) {
            return std::nullopt;
        }
        const marisa::Trie &trie = *_internal->marisa;
        marisa::Agent agent;
        agent.set_query(word, len);
        if (trie.lookup(agent)) {
            return std::optional<const dict_entry *>(_lex->at(agent.key().id()));
        } else {
            return std::nullopt;
        }
    }

    std::optional<const dict_entry *> marisa_dict::match_prefix(const char *word,
                                                                size_t len) const {
        const marisa::Trie &trie = *_internal->marisa;
        marisa::Agent agent;
        agent.set_query(word, (std::min)(_max_length, len));
        const dict_entry *match = nullptr;
        while (trie.common_prefix_search(agent)) {
            match = _lex->at(agent.key().id());
        }
        if (match == nullptr) {
            return std::nullopt;
        } else {
            return std::optional<const dict_entry *>(match);
        }
    }

    std::vector<const dict_entry *> marisa_dict::match_all_prefixes(const char *word,
                                                                    size_t len) const {
        const marisa::Trie &trie = *_internal->marisa;
        marisa::Agent agent;
        agent.set_query(word, (std::min)(_max_length, len));
        std::vector<const dict_entry *> matches;
        while (trie.common_prefix_search(agent)) {
            matches.push_back(_lex->at(agent.key().id()));
        }
        std::reverse(matches.begin(), matches.end());
        return matches;
    }

    lexicon_ptr marisa_dict::get_lexicon() const { return _lex; }

    marisa_dict_ptr marisa_dict::create_from_file(FILE *fp) {
        // Verify file header
        size_t headerLen = strlen(OCD2_HEADER);
        void *buffer = malloc(sizeof(char) * headerLen);
        size_t bytesRead = fread(buffer, sizeof(char), headerLen, fp);
        if (bytesRead != headerLen || memcmp(buffer, OCD2_HEADER, headerLen) != 0) {
            throw InvalidFormat("Invalid nlp_cc dictionary header");
        }
        free(buffer);
        // Read Marisa Trie
        marisa_dict_ptr dict(new marisa_dict());
        marisa::fread(fp, dict->_internal->marisa.get());
        std::shared_ptr<serialized_values> serialized_values =
                serialized_values::create_from_file(fp);
        lexicon_ptr values_lexicon = serialized_values->get_lexicon();
        // Extract lexicon from built Marisa Trie, in order to get the order of keys.
        marisa::Agent agent;
        agent.set_query("");
        std::vector<std::unique_ptr<dict_entry>> entries;
        entries.resize(values_lexicon->length());
        size_t _max_length = 0;
        while (dict->_internal->marisa->predictive_search(agent)) {
            const std::string key(agent.key().ptr(), agent.key().length());
            size_t id = agent.key().id();
            _max_length = (std::max)(key.length(), _max_length);
            std::unique_ptr<dict_entry> entry(
                    dict_entry_factory::create(key, values_lexicon->at(id)->values()));
            entries[id] = std::move(entry);
        }
        // Read values
        dict->_lex.reset(new lexicon(std::move(entries)));
        dict->_max_length = _max_length;
        return dict;
    }

    marisa_dict_ptr marisa_dict::create_from_dict(const basic_dict &thatDict) {
        // Extract lexicon into marisa::Keyset and a map.
        const lexicon_ptr &thatLexicon = thatDict.get_lexicon();
        size_t _max_length = 0;
        marisa::Keyset keyset;
        std::unordered_map<std::string, std::unique_ptr<dict_entry>> key_value_map;
        for (size_t i = 0; i < thatLexicon->length(); i++) {
            const dict_entry *entry = thatLexicon->at(i);
            keyset.push_back(entry->key().c_str());
            key_value_map[entry->key()].reset(dict_entry_factory::create(entry));
            _max_length = (std::max)(entry->key_length(), _max_length);
        }
        // Build Marisa Trie
        marisa_dict_ptr dict(new marisa_dict());
        dict->_internal->marisa->build(keyset);
        // Extract lexicon from built Marisa Trie, in order to get the order of keys.
        marisa::Agent agent;
        agent.set_query("");
        std::vector<std::unique_ptr<dict_entry>> entries;
        entries.resize(thatLexicon->length());
        while (dict->_internal->marisa->predictive_search(agent)) {
            std::string key(agent.key().ptr(), agent.key().length());
            std::unique_ptr<dict_entry> entry = std::move(key_value_map[key]);
            entries[agent.key().id()] = std::move(entry);
        }
        // Set lexicon with entries ordered by Marisa Trie key id.
        dict->_lex.reset(new lexicon(std::move(entries)));
        dict->_max_length = _max_length;
        return dict;
    }

    void marisa_dict::serialize_to_file(FILE *fp) const {
        fwrite(OCD2_HEADER, sizeof(char), strlen(OCD2_HEADER), fp);
        marisa::fwrite(fp, *_internal->marisa);
        std::unique_ptr<serialized_values> sv(
                new serialized_values(_lex));
        sv->serialize_to_file(fp);
    }
}  // namespace libnlp::dict