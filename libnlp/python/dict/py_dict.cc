/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/dict/dict_builder.h"
#include "libnlp/dict/dict.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

PYBIND11_MODULE(dict_cc, m) {

    py::class_<libnlp::dict::dict_entity>(m, "_NlpDictEntity")
            .def(py::init<const std::string &>())
            .def(py::init<const std::string &, const std::string &>())
            .def(py::init<const std::string &, const std::vector<std::string> &>())
            .def_static("create", py::overload_cast<const std::string&>(&libnlp::dict::dict_entity::create))
            .def_static("create", py::overload_cast<const std::string &, const std::string &>(&libnlp::dict::dict_entity::create))
            .def_static("create", py::overload_cast<const std::string &, const std::vector<std::string>&>(&libnlp::dict::dict_entity::create))
            .def_static("create", py::overload_cast<const libnlp::dict::dict_entity*>(&libnlp::dict::dict_entity::create))
            .def("key", &libnlp::dict::dict_entity::key)
            .def("values", &libnlp::dict::dict_entity::values)
            .def("get_default", &libnlp::dict::dict_entity::get_default)
            .def("num_values", &libnlp::dict::dict_entity::num_values)
            .def("key_length", &libnlp::dict::dict_entity::key_length)
            .def("__repr__", &libnlp::dict::dict_entity::to_string);

    py::class_<libnlp::dict::dict>(m, "_NlpDict")
            .def("match", py::overload_cast<const std::string &>(&libnlp::dict::dict::match, py::const_))
            .def("match", py::overload_cast<const char *, size_t>(&libnlp::dict::dict::match, py::const_))
            .def("match_prefix", py::overload_cast<const std::string &>(&libnlp::dict::dict::match_prefix, py::const_))
            .def("match_prefix", py::overload_cast<const char *, size_t>(&libnlp::dict::dict::match_prefix, py::const_))
            .def("match_all_prefixes",
                 py::overload_cast<const char *, size_t>(&libnlp::dict::dict::match_all_prefixes, py::const_))
            .def("match_all_prefixes",
                 py::overload_cast<const std::string &>(&libnlp::dict::dict::match_all_prefixes, py::const_))
            .def("key_max_length", &libnlp::dict::dict::key_max_length)
            .def("size", &libnlp::dict::dict::size)
            .def("at", &libnlp::dict::dict::at)
            .def("__repr__", &libnlp::dict::dict::to_string);


    py::class_<libnlp::dict::dict_builder>(m, "_NlpDictBuilder")
            .def(py::init<const std::string &>())
            .def("add_file", py::overload_cast<const std::string &, const std::string &>(
                    &libnlp::dict::dict_builder::add_file))
            .def("add", py::overload_cast<const std::string &>(
                    &libnlp::dict::dict_builder::add))
            .def("add", py::overload_cast<const std::string &, const std::string &>(
                    &libnlp::dict::dict_builder::add))
            .def("add", py::overload_cast<const std::string &, const std::vector<std::string> &>(
                    &libnlp::dict::dict_builder::add))
            .def("add", py::overload_cast<const libnlp::dict::dict_builder &>(
                    &libnlp::dict::dict_builder::add))
            .def("save_to_file",
                 py::overload_cast<const std::string &, const std::string &>(&libnlp::dict::dict_builder::save_to_file,
                                                                             py::const_))
            .def("get_dict", py::overload_cast<const std::string &>(&libnlp::dict::dict_builder::get_dict, py::const_))
            .def("__repr__", py::overload_cast<>(&libnlp::dict::dict_builder::to_string, py::const_));


#ifdef DICT_VERSION
    m.attr("__version__") = DICT_VERSION;
#else
    m.attr("__version__") = "dev";
#endif
}
