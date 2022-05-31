/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/normalize/xstring.h"
#include "libnlp/normalize/cc/libnlp_cc.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(normalize_cc, m) {
    py::class_<libnlp::cc::simple_converter>(m, "_NlpCC")
            .def(py::init<const std::string&>())
            .def("convert", py::overload_cast<const char*, size_t>(
                    &libnlp::cc::simple_converter::convert, py::const_));

    py::class_<libnlp::normalize::xstring>(m, "_Xstring")
            .def(py::init<const std::string &, size_t>())
            .def(py::init<const libnlp::normalize::xstring &, size_t>())
            .def(py::init<size_t>())
            .def(py::init<std::string_view, size_t>())
            .def(py::init<const char *, size_t>())
            .def("operator=", py::overload_cast<const libnlp::normalize::xstring &>(
                    &libnlp::normalize::xstring::operator=))
            .def("operator=", py::overload_cast<const std::string &>(
                    &libnlp::normalize::xstring::operator=))
            .def("operator=", py::overload_cast<const char *>(
                    &libnlp::normalize::xstring::operator=))
            .def("operator[]", &libnlp::normalize::xstring::operator[])
            .def("data", &libnlp::normalize::xstring::data)
            .def("replace", py::overload_cast<size_t, size_t, const libnlp::normalize::xstring &>(
                    &libnlp::normalize::xstring::replace))
            .def("replace", py::overload_cast<size_t, size_t, const std::string &>(
                    &libnlp::normalize::xstring::replace))
            .def("replace", py::overload_cast<size_t, size_t, const char *>(
                    &libnlp::normalize::xstring::replace))
            .def("substr", &libnlp::normalize::xstring::substr)
            .def("append", &libnlp::normalize::xstring::append)
            .def("insert", &libnlp::normalize::xstring::insert)
            .def("erase", &libnlp::normalize::xstring::erase)
            .def("clear", &libnlp::normalize::xstring::clear)
            .def("trim_emoji", &libnlp::normalize::xstring::trim_emoji)
            .def("empty", &libnlp::normalize::xstring::empty)
            .def("size", &libnlp::normalize::xstring::size)
            .def("byte_length", &libnlp::normalize::xstring::byte_length)
            .def("swap", &libnlp::normalize::xstring::swap)
            .def("find", py::overload_cast<const libnlp::normalize::xstring &>(
                    &libnlp::normalize::xstring::find))
            .def("find", py::overload_cast<const std::string &>(
                    &libnlp::normalize::xstring::find))
            .def("find", py::overload_cast<const char *>(
                    &libnlp::normalize::xstring::find))
            .def("operator+=", &libnlp::normalize::xstring::operator+=)
            .def("operator-=", &libnlp::normalize::xstring::operator-=)
            .def_readonly_static("utf8_t", &libnlp::normalize::xstring::utf8_t)
            .def_readonly_static("gbk_t", &libnlp::normalize::xstring::gbk_t)
            .def_readonly_static("utf16_t", &libnlp::normalize::xstring::utf16_t)
            .def("__str__", &libnlp::normalize::xstring::data);

#ifdef NOR_VERSION
    m.attr("__version__") = NOR_VERSION;
#else
    m.attr("__version__") = "dev";
#endif
}

