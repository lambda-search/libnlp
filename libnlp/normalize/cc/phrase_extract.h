
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#pragma once

#include <functional>
#include <unordered_map>

#include "libnlp/base/common.h"
#include "libnlp/base/utf8_string_view.h"

namespace libnlp::cc {

    class LIBNLP_EXPORT phrase_extract {
    public:
        typedef UTF8StringSlice::LengthType LengthType;

        typedef UTF8StringSliceBase<unsigned char> UTF8StringSlice8Bit;

        phrase_extract();

        virtual ~phrase_extract();

        void Extract(const std::string &text) {
            set_full_text(text);
            extract_suffixes();
            calculate_frequency();
            calculate_suffi_entropy();
            release_suffixes();
            extract_prefixes();
            calculate_prefix_entropy();
            release_prefixes();
            extract_word_candidates();
            calculate_cohesions();
            select_words();
        }

        void set_full_text(const std::string &fullText) {
            _utf8FullText = UTF8StringSlice(fullText.c_str());
        }

        void set_full_text(const char *fullText) {
            _utf8FullText = UTF8StringSlice(fullText);
        }

        void set_full_text(const UTF8StringSlice &fullText) { _utf8FullText = fullText; }

        void set_word_min_length(const LengthType l) {
            _wordMinLength = l;
        }

        void set_word_max_length(const LengthType l) {
            _wordMaxLength = l;
        }

        void set_prefix_set_length(const LengthType l) {
            _prefixSetLength = l;
        }

        void set_suffix_set_length(const LengthType l) {
            _suffixSetLength = l;
        }

        // PreCalculationFilter is called after frequencies statistics.
        void set_pre_calculation_filter(
                const std::function<bool(const phrase_extract &,
                                         const UTF8StringSlice8Bit &)> &filter) {
            _preCalculationFilter = filter;
        }

        void set_post_calculation_filter(
                const std::function<bool(const phrase_extract &,
                                         const UTF8StringSlice8Bit &)> &filter) {
            _postCalculationFilter = filter;
        }

        void release_suffixes() { std::vector<UTF8StringSlice8Bit>().swap(_suffixes); }

        void release_prefixes() { std::vector<UTF8StringSlice8Bit>().swap(_prefixes); }

        const std::vector<UTF8StringSlice8Bit> &words() const { return _words; }

        const std::vector<UTF8StringSlice8Bit> &word_candidates() const {
            return _wordCandidates;
        }

        struct phrase_signals {
            size_t frequency;
            double cohesion;
            double suffixEntropy;
            double prefixEntropy;
        };

        const phrase_signals &signal(const UTF8StringSlice8Bit &wordCandidate) const;

        double cohesion(const UTF8StringSlice8Bit &wordCandidate) const;

        double entropy(const UTF8StringSlice8Bit &wordCandidate) const;

        double suffix_entropy(const UTF8StringSlice8Bit &wordCandidate) const;

        double prefix_entropy(const UTF8StringSlice8Bit &wordCandidate) const;

        size_t frequency(const UTF8StringSlice8Bit &word) const;

        double probability(const UTF8StringSlice8Bit &word) const;

        double log_probability(const UTF8StringSlice8Bit &word) const;

        void reset();

        void extract_suffixes();

        void extract_prefixes();

        void extract_word_candidates();

        void calculate_frequency();

        void calculate_cohesions();

        void calculate_suffi_entropy();

        void calculate_prefix_entropy();

        void select_words();

        static bool
        default_pre_calculation_filter(const phrase_extract &,
                                    const phrase_extract::UTF8StringSlice8Bit &);

        static bool
        default_post_calculation_filter(const phrase_extract &,
                                     const phrase_extract::UTF8StringSlice8Bit &);

    private:
        class phrase_dict_type;

        // Pointwise Mutual Information
        double pointwise_mutual_information(const UTF8StringSlice8Bit &wordCandidate,
                   const UTF8StringSlice8Bit &part1,
                   const UTF8StringSlice8Bit &part2) const;

        double calculate_cohesion(const UTF8StringSlice8Bit &wordCandidate) const;

        double calculate_entropy(
                const std::unordered_map<UTF8StringSlice8Bit, size_t,
                        UTF8StringSlice8Bit::Hasher> &choices) const;

        LengthType _wordMinLength;
        LengthType _wordMaxLength;
        LengthType _prefixSetLength;
        LengthType _suffixSetLength;
        std::function<bool(const phrase_extract &, const UTF8StringSlice8Bit &)>
                _preCalculationFilter;
        std::function<bool(const phrase_extract &, const UTF8StringSlice8Bit &)>
                _postCalculationFilter;

        bool _prefixesExtracted;
        bool _suffixesExtracted;
        bool _frequenciesCalculated;
        bool _wordCandidatesExtracted;
        bool _cohesionsCalculated;
        bool _prefixEntropiesCalculated;
        bool _suffixEntropiesCalculated;
        bool _wordsSelected;

        UTF8StringSlice _utf8FullText;
        size_t _totalOccurrence;
        double _logTotalOccurrence;
        std::vector<UTF8StringSlice8Bit> _prefixes;
        std::vector<UTF8StringSlice8Bit> _suffixes;
        std::vector<UTF8StringSlice8Bit> _wordCandidates;
        std::vector<UTF8StringSlice8Bit> _words;
        phrase_dict_type *_signals;

        friend class PhraseExtractTest;
    };

} // namespace libnlp::cc
