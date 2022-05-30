################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

include(require_benchmark)
include(carbin_cc_benchmark)

carbin_cc_benchmark(
        NAME libnlp_cc_performance
        SOURCES libnlp_cc_performance.cc
        PUBLIC_LINKED_TARGETS ${BENCHMARK_LIB} ${CARBIN_DYLINK} libnlp::libnlp
        PRIVATE_COMPILE_OPTIONS ${CARBIN_TEST_COPTS}
        VERBOSE
)
