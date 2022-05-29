//
// Created by liyinbin on 2022/5/30.
//

#include "libnlp/normalize/trimmer/emoji.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(trimmer, m) {

    m.def("trim_emoji",py::overload_cast<const char*, size_t>(&libnlp::trimmer::trim_emoji), "trim emoji")
     .def("trim_emoji",py::overload_cast<const std::string_view &>(&libnlp::trimmer::trim_emoji), "trim emoji");

#ifdef VERSION
    m.attr("__version__") = VERSION;
#else
    m.attr("__version__") = "dev";
#endif
}
