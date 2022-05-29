
#pragma once

#include "libnlp/base/common.h"
#include "libnlp/normalize/cc/segmentation.h"

namespace libnlp::cc {
    /**
     * Controller of segmentation and conversion
     * @ingroup lib_cc_cpp_api
     */
    class LIBNLP_EXPORT converter {
    public:
        converter(const std::string &n, segmentation_ptr s,
                  conversion_chain_ptr c)
                : _name(n), _segmentation(s),
                  _conversion_chain(c) {}

        std::string convert(const std::string &text) const;

        size_t convert(const char *input, char *output) const;

        const segmentation_ptr get_segmentation() const { return _segmentation; }

        const conversion_chain_ptr GetConversionChain() const {
            return _conversion_chain;
        }

    private:
        const std::string _name;
        const segmentation_ptr _segmentation;
        const conversion_chain_ptr _conversion_chain;
    };
} // namespace libnlp::cc
