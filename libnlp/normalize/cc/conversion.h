
#pragma once

#include "libnlp/base/common.h"
#include "libnlp/normalize/cc/segmentation.h"

namespace libnlp::cc {
    /**
     * conversion interface
     * @ingroup lib_cc_cpp_api
     */
    class LIBNLP_EXPORT conversion {
    public:
        conversion(libnlp::dict::basic_dict_ptr d) : _dict(d) {}

        // convert single phrase
        std::string convert(const std::string &phrase) const;

        // convert single phrase
        std::string convert(const char *phrase) const;

        // convert segmented text
        segments_ptr convert(const segments_ptr &input) const;

        const libnlp::dict::basic_dict_ptr get_dict() const { return _dict; }

    private:
        const libnlp::dict::basic_dict_ptr _dict;
    };
} // namespace libnlp::cc
