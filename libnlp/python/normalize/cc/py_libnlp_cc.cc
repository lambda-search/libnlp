
#include "libnlp/normalize/cc/libnlp_cc.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(nlp_cc, m) {
  py::class_<libnlp::cc::simple_converter>(m, "_NlpCC")
      .def(py::init<const std::string&>())
      .def("convert", py::overload_cast<const char*, size_t>(
                          &libnlp::cc::simple_converter::convert, py::const_));

#ifdef VERSION
  m.attr("__version__") = VERSION;
#else
  m.attr("__version__") = "dev";
#endif
}
