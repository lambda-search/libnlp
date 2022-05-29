
#include <algorithm>
#include <cmath>
#include <unordered_map>

#include "libnlp/trie/marisa/trie.h"
#include "libnlp/normalize/cc/phrase_extract.h"

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

namespace libnlp::cc {

    namespace internal {

        bool contains_punctuation(const phrase_extract::UTF8StringSlice8Bit &word) {
            static const std::vector<phrase_extract::UTF8StringSlice8Bit> punctuations = {
                    " ", "\n", "\r", "\t", "-", ",", ".", "?", "!", "*", "　",
                    "，", "。", "、", "；", "：", "？", "！", "…", "“", "”", "「",
                    "」", "—", "－", "（", "）", "《", "》", "．", "／", "＼"};
            for (const auto &punctuation : punctuations) {
                if (word.FindBytePosition(punctuation) !=
                    static_cast<phrase_extract::UTF8StringSlice8Bit::LengthType>(-1)) {
                    return true;
                }
            }
            return false;
        }

    } // namespace internal

    class phrase_extract::phrase_dict_type {
    public:
        typedef phrase_extract::phrase_signals ValueType;
        typedef std::pair<UTF8StringSlice8Bit, ValueType> ItemType;

        phrase_extract::phrase_signals &Get(const UTF8StringSlice8Bit &key) {
            marisa::Agent agent;
            agent.set_query(key.CString(), key.ByteLength());
            if (_marisa_trie.lookup(agent)) {
                int item_id = _marisa_id_item_map[agent.key().id()];
                return _items[item_id].second;
            }

            throw ShouldNotBeHere();
        }

        phrase_extract::phrase_signals &add_key(const UTF8StringSlice8Bit &key) {
            return _dict[key];
        }

        void Clear() {
            ClearDict();
            _marisa_trie.clear();
        }

        const std::vector<ItemType> &items() const { return _items; }

        void Build() {
            BuildKeys();
            BuildTrie();
        }

    private:
        void BuildKeys() {
            _items.reserve(_dict.size());
            for (const auto &item : _dict) {
                _items.push_back(item);
            }
            ClearDict();
            std::sort(
                    _items.begin(), _items.end(),
                    [](const ItemType &a, const ItemType &b) { return a.first < b.first; });
        }

        void ClearDict() {
            std::unordered_map<UTF8StringSlice8Bit, phrase_extract::phrase_signals,
                    UTF8StringSlice8Bit::Hasher>()
                    .swap(_dict);
        }

        void BuildTrie() {
            std::unordered_map<std::string, int> key_item_id_map;
            marisa::Keyset keyset;
            for (size_t i = 0; i < _items.size(); i++) {
                const auto &key = _items[i].first;
                key_item_id_map[key.to_string()] = i;
                keyset.push_back(key.CString(), key.ByteLength());
            }
            _marisa_trie.build(keyset);
            marisa::Agent agent;
            agent.set_query("");
            _marisa_id_item_map.resize(_items.size());
            while (_marisa_trie.predictive_search(agent)) {
                size_t marisa_id = agent.key().id();
                const std::string key(agent.key().ptr(), agent.key().length());
                const auto it = key_item_id_map.find(key);
                if (it == key_item_id_map.end()) {
                    throw ShouldNotBeHere();
                }
                int item_id = it->second;
                _marisa_id_item_map[marisa_id] = item_id;
            }
        }

        std::unordered_map<UTF8StringSlice8Bit, phrase_extract::phrase_signals,
                UTF8StringSlice8Bit::Hasher>
                _dict;
        std::vector<ItemType> _items;
        marisa::Trie _marisa_trie;
        std::vector<int> _marisa_id_item_map;
    };

    using namespace internal;

    bool phrase_extract::default_pre_calculation_filter(
            const phrase_extract &, const phrase_extract::UTF8StringSlice8Bit &) {
        return false;
    }

    bool phrase_extract::default_post_calculation_filter(
            const phrase_extract &phraseExtract,
            const phrase_extract::UTF8StringSlice8Bit &word) {
        const phrase_extract::phrase_signals &signals = phraseExtract.signal(word);
        const double logProbability = phraseExtract.log_probability(word);
        const double cohesionScore = signals.cohesion - logProbability * 0.5;
        const double entropyScore =
                sqrt((signals.prefixEntropy) * (signals.suffixEntropy + 1)) -
                logProbability * 0.85;
        bool accept = cohesionScore > 9 && entropyScore > 11 &&
                      signals.prefixEntropy > 0.5 && signals.suffixEntropy > 0 &&
                      signals.prefixEntropy + signals.suffixEntropy > 3;
        return !accept;
    }

    phrase_extract::phrase_extract()
            : _wordMinLength(2), _wordMaxLength(2), _prefixSetLength(1),
              _suffixSetLength(1), _preCalculationFilter(default_pre_calculation_filter),
              _postCalculationFilter(default_post_calculation_filter), _utf8FullText(""),
              _signals(new phrase_dict_type) {
        reset();
    }

    phrase_extract::~phrase_extract() { delete _signals; }

    void phrase_extract::reset() {
        _prefixesExtracted = false;
        _suffixesExtracted = false;
        _frequenciesCalculated = false;
        _wordCandidatesExtracted = false;
        _cohesionsCalculated = false;
        _prefixEntropiesCalculated = false;
        _suffixEntropiesCalculated = false;
        _wordsSelected = false;
        _totalOccurrence = 0;
        _logTotalOccurrence = 0;
        release_prefixes();
        release_suffixes();
        _wordCandidates.clear();
        _words.clear();
        _signals->Clear();
        _utf8FullText = UTF8StringSlice("");
        _preCalculationFilter = default_pre_calculation_filter;
        _postCalculationFilter = default_post_calculation_filter;
    }

    void phrase_extract::extract_suffixes() {
        _suffixes.reserve(_utf8FullText.UTF8Length() / 2 *
                          (_wordMaxLength + _suffixSetLength));
        for (UTF8StringSlice text = _utf8FullText; text.UTF8Length() > 0;
             text.MoveRight()) {
            const LengthType suffixLength =
                    (std::min)(static_cast<LengthType>(_wordMaxLength + _suffixSetLength),
                               text.UTF8Length());
            const UTF8StringSlice &slice = text.Left(suffixLength);
            _suffixes.push_back(UTF8StringSlice8Bit(
                    slice.CString(),
                    static_cast<UTF8StringSlice8Bit::LengthType>(slice.UTF8Length()),
                    static_cast<UTF8StringSlice8Bit::LengthType>(slice.ByteLength())));
        }
        _suffixes.shrink_to_fit();
        // Sort suffixes
        std::sort(_suffixes.begin(), _suffixes.end());
        _suffixesExtracted = true;
    }

    void phrase_extract::extract_prefixes() {
        _prefixes.reserve(_utf8FullText.UTF8Length() / 2 *
                          (_wordMaxLength + _prefixSetLength));
        for (UTF8StringSlice text = _utf8FullText; text.UTF8Length() > 0;
             text.MoveLeft()) {
            const LengthType prefixLength =
                    (std::min)(static_cast<LengthType>(_wordMaxLength + _prefixSetLength),
                               text.UTF8Length());
            const UTF8StringSlice &slice = text.Right(prefixLength);
            _prefixes.push_back(UTF8StringSlice8Bit(
                    slice.CString(),
                    static_cast<UTF8StringSlice8Bit::LengthType>(slice.UTF8Length()),
                    static_cast<UTF8StringSlice8Bit::LengthType>(slice.ByteLength())));
        }
        _prefixes.shrink_to_fit();
        // Sort suffixes reversely
        std::sort(_prefixes.begin(), _prefixes.end(),
                  [](const UTF8StringSlice8Bit &a, const UTF8StringSlice8Bit &b) {
                      return a.ReverseCompare(b) < 0;
                  });
        _prefixesExtracted = true;
    }

    void phrase_extract::calculate_frequency() {
        if (!_suffixesExtracted) {
            extract_suffixes();
        }
        for (const auto &suffix : _suffixes) {
            for (UTF8StringSlice8Bit::LengthType i = 1;
                 i <= suffix.UTF8Length() && i <= _wordMaxLength; i++) {
                const UTF8StringSlice8Bit wordCandidate = suffix.Left(i);
                _signals->add_key(wordCandidate).frequency++;
                _totalOccurrence++;
            }
        }
        _logTotalOccurrence = log(_totalOccurrence);
        _signals->Build();
        _frequenciesCalculated = true;
    }

    void phrase_extract::extract_word_candidates() {
        if (!_frequenciesCalculated) {
            calculate_frequency();
        }
        for (const auto &item : _signals->items()) {
            const auto &wordCandidate = item.first;
            if (wordCandidate.UTF8Length() < _wordMinLength) {
                continue;
            }
            if (contains_punctuation(wordCandidate)) {
                continue;
            }
            if (_preCalculationFilter(*this, wordCandidate)) {
                continue;
            }
            _wordCandidates.push_back(wordCandidate);
        }
        // Sort by frequency
        std::sort(_wordCandidates.begin(), _wordCandidates.end(),
                  [this](const UTF8StringSlice8Bit &a, const UTF8StringSlice8Bit &b) {
                      const size_t freqA = frequency(a);
                      const size_t freqB = frequency(b);
                      if (freqA > freqB) {
                          return true;
                      } else if (freqA < freqB) {
                          return false;
                      } else {
                          return a < b;
                      }
                  });
        _wordCandidatesExtracted = true;
    }

    typedef std::unordered_map<phrase_extract::UTF8StringSlice8Bit, size_t,
            phrase_extract::UTF8StringSlice8Bit::Hasher>
            AdjacentSetType;

    template<bool SUFFIX>
    void CalculatePrefixSuffixEntropy(
            const std::vector<phrase_extract::UTF8StringSlice8Bit> &presuffixes,
            const phrase_extract::LengthType setLength,
            const phrase_extract::LengthType wordMinLength,
            const phrase_extract::LengthType wordMaxLength,
            const std::function<void(const phrase_extract::UTF8StringSlice8Bit &word,
                                     AdjacentSetType &adjacentSet)> &updateEntropy) {
        AdjacentSetType adjacentSet;
        auto setLength8Bit =
                static_cast<phrase_extract::UTF8StringSlice8Bit::LengthType>(setLength);
        for (phrase_extract::LengthType length = wordMinLength;
             length <= wordMaxLength; length++) {
            adjacentSet.clear();
            phrase_extract::UTF8StringSlice8Bit lastWord("");
            for (const auto &presuffix : presuffixes) {
                if (presuffix.UTF8Length() < length) {
                    continue;
                }
                auto length8Bit =
                        static_cast<phrase_extract::UTF8StringSlice8Bit::LengthType>(length);
                const auto &wordCandidate =
                        SUFFIX ? presuffix.Left(length8Bit) : presuffix.Right(length8Bit);
                if (wordCandidate != lastWord) {
                    updateEntropy(lastWord, adjacentSet);
                    lastWord = wordCandidate;
                }
                if (length + setLength <= presuffix.UTF8Length()) {
                    if (SUFFIX) {
                        const auto &wordSuffix =
                                presuffix.SubString(length8Bit, setLength8Bit);
                        adjacentSet[wordSuffix]++;
                    } else {
                        const auto &wordPrefix = presuffix.SubString(
                                presuffix.UTF8Length() - length8Bit - setLength8Bit,
                                setLength8Bit);
                        adjacentSet[wordPrefix]++;
                    }
                }
            }
            updateEntropy(lastWord, adjacentSet);
        }
    }

    void phrase_extract::calculate_suffi_entropy() {
        if (!_suffixesExtracted) {
            extract_suffixes();
        }
        if (!_frequenciesCalculated) {
            calculate_frequency();
        }
        CalculatePrefixSuffixEntropy<true>(
                _suffixes, _suffixSetLength, _wordMinLength, _wordMaxLength,
                [this](const phrase_extract::UTF8StringSlice8Bit &word,
                       AdjacentSetType &adjacentSet) {
                    if (word.UTF8Length() > 0) {
                        _signals->Get(word).suffixEntropy = calculate_entropy(adjacentSet);
                        adjacentSet.clear();
                    }
                });
        _suffixEntropiesCalculated = true;
    }

    void phrase_extract::calculate_prefix_entropy() {
        if (!_prefixesExtracted) {
            extract_prefixes();
        }
        if (!_frequenciesCalculated) {
            calculate_frequency();
        }
        CalculatePrefixSuffixEntropy<false>(
                _prefixes, _prefixSetLength, _wordMinLength, _wordMaxLength,
                [this](const phrase_extract::UTF8StringSlice8Bit &word,
                       AdjacentSetType &adjacentSet) {
                    if (word.UTF8Length() > 0) {
                        _signals->Get(word).prefixEntropy = calculate_entropy(adjacentSet);
                        adjacentSet.clear();
                    }
                });
        _prefixEntropiesCalculated = true;
    }

    void phrase_extract::calculate_cohesions() {
        if (!_wordCandidatesExtracted) {
            extract_word_candidates();
        }
        if (!_frequenciesCalculated) {
            calculate_frequency();
        }
        for (const auto &wordCandidate : _wordCandidates) {
            _signals->Get(wordCandidate).cohesion = calculate_cohesion(wordCandidate);
        }
        _cohesionsCalculated = true;
    }

    const phrase_extract::phrase_signals &
    phrase_extract::signal(const UTF8StringSlice8Bit &wordCandidate) const {
        return _signals->Get(wordCandidate);
    }

    double phrase_extract::cohesion(const UTF8StringSlice8Bit &word) const {
        return signal(word).cohesion;
    }

    double phrase_extract::entropy(const UTF8StringSlice8Bit &word) const {
        return suffix_entropy(word) + prefix_entropy(word);
    }

    double phrase_extract::suffix_entropy(const UTF8StringSlice8Bit &word) const {
        return signal(word).suffixEntropy;
    }

    double phrase_extract::prefix_entropy(const UTF8StringSlice8Bit &word) const {
        return signal(word).prefixEntropy;
    }

    size_t phrase_extract::frequency(const UTF8StringSlice8Bit &word) const {
        const size_t frequency = signal(word).frequency;
        return frequency;
    }

    double phrase_extract::probability(const UTF8StringSlice8Bit &word) const {
        const size_t f = frequency(word);
        return static_cast<double>(f) / _totalOccurrence;
    }

    double phrase_extract::log_probability(const UTF8StringSlice8Bit &word) const {
        // log(frequency / totalOccurrence) = log(frequency) - log(totalOccurrence)
        const size_t f = frequency(word);
        return log(f) - _logTotalOccurrence;
    }

    double phrase_extract::pointwise_mutual_information(const UTF8StringSlice8Bit &wordCandidate,
                                                        const UTF8StringSlice8Bit &part1,
                                                        const UTF8StringSlice8Bit &part2) const {
        // pointwise_mutual_information(x, y) = log(P(x, y) / (P(x) * P(y)))
        //           = log(P(x, y)) - log(P(x)) - log(P(y))
        return log_probability(wordCandidate) - log_probability(part1) -
               log_probability(part2);
    }

    double phrase_extract::calculate_cohesion(
            const UTF8StringSlice8Bit &wordCandidate) const {
        // TODO Try average value
        double minPMI = INFINITY;
        for (UTF8StringSlice8Bit::LengthType leftLength = 1;
             leftLength <= wordCandidate.UTF8Length() - 1; leftLength++) {
            const auto &leftPart = wordCandidate.Left(leftLength);
            const auto &rightPart =
                    wordCandidate.Right(wordCandidate.UTF8Length() - leftLength);
            double pmi = pointwise_mutual_information(wordCandidate, leftPart, rightPart);
            minPMI = (std::min)(pmi, minPMI);
        }
        return minPMI;
    }

    double phrase_extract::calculate_entropy(
            const std::unordered_map<UTF8StringSlice8Bit, size_t,
                    UTF8StringSlice8Bit::Hasher> &choices) const {
        double totalChoices = 0;
        for (const auto &item : choices) {
            totalChoices += item.second;
        }
        double entropy = 0;
        for (const auto &item : choices) {
            const size_t occurrence = item.second;
            const double probability = occurrence / totalChoices;
            entropy += probability * log(probability);
        }
        if (entropy != 0) {
            entropy = -entropy;
        }
        return entropy;
    }

    void phrase_extract::select_words() {
        if (!_wordCandidatesExtracted) {
            extract_word_candidates();
        }
        if (!_cohesionsCalculated) {
            calculate_cohesions();
        }
        if (!_prefixEntropiesCalculated) {
            calculate_prefix_entropy();
        }
        if (!_suffixEntropiesCalculated) {
            calculate_suffi_entropy();
        }
        for (const auto &word : _wordCandidates) {
            if (!_postCalculationFilter(*this, word)) {
                _words.push_back(word);
            }
        }
        _wordsSelected = true;
    }

} // namespace libnlp::cc
