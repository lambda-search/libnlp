

#ifndef LIBNLP_NORMALIZE_NORMALIZER_H_
#define LIBNLP_NORMALIZE_NORMALIZER_H_

#include "libnlp/normalize/cc/simple_converter.h"
#include "libnlp/normalize/option.h"

namespace libnlp::normalize {

    class normalizer {
    public:
        explicit normalizer(const std::string &conf);

        std::string normalize(const std::string_view &input, normalize_options options = kDefaultNormalizeOption) const;

        std::string normalize(const std::string_view &input, const std::string_view &replace = "",
                              normalize_options options = kDefaultNormalizeOption) const;

        std::string repace_regex(const std::string_view &input, std::string_view &replacer) const;

        static const normalize_options kDefaultNormalizeOption;
    private:
        libnlp::cc::simple_converter _conv;
    };
}  // namespace libnlp::normalize

#endif // LIBNLP_NORMALIZE_NORMALIZER_H_
