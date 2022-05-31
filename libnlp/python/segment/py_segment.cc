/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/segment/jieba/jieba.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;


PYBIND11_MODULE(segment_cc, m) {

    py::class_<libnlp::word_type>(m, "_WordType")
            .def(py::init<const std::string, uint32_t>())
            .def_readwrite("word", &libnlp::word_type::word)
            .def_readwrite("offset", &libnlp::word_type::offset)
            .def("__repr__", &libnlp::word_type::to_string);


    py::bind_vector<std::vector<libnlp::word_type>>(m, "_WordTypeList")
            .def("__str__", [](const std::vector<libnlp::word_type> &v) {
                std::string ret = "[";
                for (size_t i = 0; i < v.size(); i++) {
                    ret += v[i].to_string();
                    if (i != v.size() - 1) {
                        ret += ",";
                    }
                }
                ret += "]";
                return ret;
            });

    py::class_<libnlp::jieba::jieba_engine>(m, "_JiebaPY")
            .def(py::init<const std::string &, const std::string &, const std::string &, const std::string &, const std::string &>())
            .def("cut",
                 py::overload_cast<const std::string &, bool>(&libnlp::jieba::jieba_engine::cut, py::const_))
            .def("cut_with_pos",
                 py::overload_cast<const std::string &, bool>(&libnlp::jieba::jieba_engine::cut_with_pos, py::const_))
            .def("cut_all",
                 py::overload_cast<const std::string &>(&libnlp::jieba::jieba_engine::cut_all, py::const_))
            .def("cut_all_with_pos",
                 py::overload_cast<const std::string &>(&libnlp::jieba::jieba_engine::cut_all_with_pos, py::const_))
            .def("cut_for_search",
                 py::overload_cast<const std::string &, bool>(&libnlp::jieba::jieba_engine::cut_for_search, py::const_))
            .def("cut_for_search_with_pos",
                 py::overload_cast<const std::string &, bool>(&libnlp::jieba::jieba_engine::cut_for_search_with_pos,
                                                              py::const_))
            .def("cut_hmm",
                 py::overload_cast<const std::string &>(&libnlp::jieba::jieba_engine::cut_hmm, py::const_))
            .def("cut_hmm_wih_pos",
                 py::overload_cast<const std::string &>(&libnlp::jieba::jieba_engine::cut_hmm_wih_pos, py::const_))
            .def("cut_small",
                 py::overload_cast<const std::string &, size_t>(&libnlp::jieba::jieba_engine::cut_small, py::const_))
            .def("cut_small_with_pos",
                 py::overload_cast<const std::string &, size_t>(&libnlp::jieba::jieba_engine::cut_small_with_pos,
                                                                py::const_))
            .def("tag",
                 py::overload_cast<const std::string &>(&libnlp::jieba::jieba_engine::tag, py::const_))
            .def("lookup_tag",
                 py::overload_cast<const std::string &>(&libnlp::jieba::jieba_engine::lookup_tag, py::const_))
            .def("insert_user_word",
                 py::overload_cast<const std::string &, const std::string &>(
                         &libnlp::jieba::jieba_engine::insert_user_word))
            .def("insert_user_word",
                 py::overload_cast<const std::string &, int, const std::string &>(
                         &libnlp::jieba::jieba_engine::insert_user_word))
            .def("find", &libnlp::jieba::jieba_engine::find)
            .def("reset_separators", &libnlp::jieba::jieba_engine::reset_separators)
            .def("load_user_dict",
                 py::overload_cast<const std::vector<std::string> &>(&libnlp::jieba::jieba_engine::load_user_dict))
            .def("load_user_dict",
                 py::overload_cast<const std::set<std::string> &>(&libnlp::jieba::jieba_engine::load_user_dict))
            .def("load_user_dict",
                 py::overload_cast<const std::string &>(&libnlp::jieba::jieba_engine::load_user_dict));


#ifdef SEG_VERSION
    m.attr("__version__") = SEG_VERSION;
#else
    m.attr("__version__") = "dev";
#endif
}


