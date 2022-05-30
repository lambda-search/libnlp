/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/python/dict/py_dict_entry.h"
#include "libnlp/python/dict/py_dict_base.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(dict_loader, m) {
    py::class_<libnlp::dict::dict_loader>(m, "_NlpDict")
            .def(py::init<const std::string&>())
            .def("convert", py::overload_cast<const char*, size_t>(
                    &libnlp::cc::simple_converter::convert, py::const_));

#ifdef DICT_VERSION
    m.attr("__version__") = DICT_VERSION;
#else
    m.attr("__version__") = "dev";
#endif
}
