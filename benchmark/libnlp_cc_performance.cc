
#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <streambuf>

#ifdef _MSC_VER
#include <direct.h>
#else

#include <unistd.h>

#endif

#include "libnlp/normalize/cc/simple_converter.h"
#include "../test/testing/test_utils_utf8.h"

namespace libnlp::cc {

    simple_converter *Initialize(const std::string &config_name) {
        chdir(PROJECT_BINARY_DIR "/data/cc");
        const std::string config_dir = LIBNLP_SOURCE_DIR "/data/cc/config/";
        const std::string config_path = config_dir + config_name + ".json";
        return new simple_converter(config_path);
    }

    void convert(const simple_converter *converter, const std::string &text) {
        converter->convert(text);
    }

    std::string ReadText(const std::string &filename) {
        const std::string benchmark_data_dir = TEST_DATA_ROOT "/benchmark/";
        const std::string data_path = benchmark_data_dir + filename;
        std::ifstream stream(data_path);
        return std::string((std::istreambuf_iterator<char>(stream)),
                           std::istreambuf_iterator<char>());
    }

    static void BM_Initialization(benchmark::State &state,
                                  std::string config_name) {
        for (auto _ : state) {
            const simple_converter *converter = Initialize(config_name);
            // Do not count the destruction time.
            state.PauseTiming();
            delete converter;
            state.ResumeTiming();
        }
    }

    BENCHMARK_CAPTURE(BM_Initialization, hk2s, "hk2s")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, hk2t, "hk2t")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, jp2t, "jp2t")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, s2hk, "s2hk")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, s2t, "s2t")->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, s2tw, "s2tw")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, s2twp, "s2twp")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, t2hk, "t2hk")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, t2jp, "t2jp")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, t2s, "t2s")->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, tw2s, "tw2s")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, tw2sp, "tw2sp")
    ->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Initialization, tw2t, "tw2t")
    ->Unit(benchmark::kMillisecond);

    static void BM_Convert2M(benchmark::State &state) {
        const std::string config_name = "s2t";
        const std::string text = ReadText("zuozhuan.txt");
        const std::unique_ptr<simple_converter> converter(Initialize(config_name));
        for (auto _ : state) {
            convert(converter.get(), text);
        }
    }

    BENCHMARK(BM_Convert2M)->Unit(benchmark::kMillisecond);

    static void BM_Convert(benchmark::State &state, int iteration) {
        std::ostringstream os;
        for (int i = 0; i < iteration; i++) {
            os << "Open Chinese convert 開放中文轉換" << i << std::endl;
        }
        const std::string text = os.str();
        const std::string config_name = "s2t";
        const std::unique_ptr<simple_converter> converter(Initialize(config_name));
        for (auto _ : state) {
            convert(converter.get(), text);
        }
    }

    BENCHMARK_CAPTURE(BM_Convert, 100, 100)->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Convert, 1000, 1000)->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Convert, 10000, 10000)->Unit(benchmark::kMillisecond);
    BENCHMARK_CAPTURE(BM_Convert, 100000, 100000)->Unit(benchmark::kMillisecond);

} // namespace libnlp::cc

BENCHMARK_MAIN();
