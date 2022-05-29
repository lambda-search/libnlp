
include(require_benchmark)
include(carbin_cc_benchmark)
#[[

add_executable(libnlp_cc_performance libnlp_cc_performance.cc)
target_link_libraries(libnlp_cc_performance ${BENCHMARK_LIB} libnlp::libnlp)
add_test(BenchmarkTest libnlp_cc_performance)
]]

carbin_cc_benchmark(
        NAME libnlp_cc_performance
        SOURCES libnlp_cc_performance.cc
        PUBLIC_LINKED_TARGETS ${BENCHMARK_LIB} ${CARBIN_DYLINK} libnlp::libnlp
        PRIVATE_COMPILE_OPTIONS ${CARBIN_TEST_COPTS}
        VERBOSE
)
