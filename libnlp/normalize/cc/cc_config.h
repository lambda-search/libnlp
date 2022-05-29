
#pragma once

#include "libnlp/base/common.h"

namespace libnlp::cc {
    /**
     * Configuration loader
     * @ingroup lib_cc_cpp_api
     */
    class LIBNLP_EXPORT cc_config {
    public:
        cc_config();

        virtual ~cc_config();

        converter_ptr create_from_string(const std::string &json,
                                   const std::string &configDirectory);

        converter_ptr create_from_file(const std::string &fileName);

    private:
        void *_internal;
    };
}  // namespace libnlp::cc
