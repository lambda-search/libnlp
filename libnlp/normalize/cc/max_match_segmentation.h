
#pragma once

#include "libnlp/base/common.h"
#include "libnlp/dict/dict_group.h"
#include "libnlp/normalize/cc/segmentation.h"

namespace libnlp::cc {
    /**
     * Implementation of maximal match segmentation
     * @ingroup lib_cc_cpp_api
     */
    class LIBNLP_EXPORT max_match_segmentation : public segmentation {
    public:
        explicit max_match_segmentation(const libnlp::dict::basic_dict_ptr d) : _dict(d) {}

        virtual ~max_match_segmentation() = default;

        segments_ptr segment(const std::string &text) const override;

        const libnlp::dict::basic_dict_ptr get_dict() const { return _dict; }

    private:
        const libnlp::dict::basic_dict_ptr _dict;
    };
} // namespace libnlp::cc
