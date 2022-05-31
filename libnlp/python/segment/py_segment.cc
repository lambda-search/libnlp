/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/segment/jieba/jieba.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(segment_cc, m) {
    py::class_<libnlp::jieba::jieba_engine>(m, "_JiebaEngine")
            .def(py::init<const std::string&,const std::string&,const std::string&,const std::string&,const std::string&>());
            //.def("convert", py::overload_cast<const char*, size_t>(
             //       &libnlp::cc::simple_converter::convert, py::const_));

#ifdef SEG_VERSION
    m.attr("__version__") = SEG_VERSION;
#else
    m.attr("__version__") = "dev";
#endif
}


