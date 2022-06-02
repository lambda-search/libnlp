
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

// Microsoft Visual C++ specific
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma warning(disable : 4251 4266 4350 4503 4512 4514 4710 4820)
#endif

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "libnlp/base/export.h"
#include "libnlp/config/libnlp_config.h"

// Forward decalarations and alias
namespace libnlp::cc {
    class cc_config;

    class conversion;

    class conversion_chain;

    class converter;


    class segmentation;

    class segments;


    typedef std::shared_ptr<conversion> conversion_ptr;
    typedef std::shared_ptr<conversion_chain> conversion_chain_ptr;
    typedef std::shared_ptr<converter> converter_ptr;
    typedef std::shared_ptr<segmentation> segmentation_ptr;
    typedef std::shared_ptr<segments> segments_ptr;


} // namespace libnlp::cc

namespace libnlp::dict {
    class basic_dict;

    class dict_entity;

    class dict_group;

    class lexicon;

    class marisa_dict;

    class multi_value_dict_entry;

    class non_value_dict_entry;

    class binary_dict;

    class darts_dict;

    class text_dict;

    class single_value_dict_entry;

    class serializable_dict;

    typedef std::shared_ptr<text_dict> text_dict_ptr;
    typedef std::shared_ptr<basic_dict> basic_dict_ptr;
    typedef std::shared_ptr<dict_group> dict_group_ptr;
    typedef std::shared_ptr<lexicon> lexicon_ptr;
    typedef std::shared_ptr<marisa_dict> marisa_dict_ptr;
    typedef std::shared_ptr<binary_dict> binary_dict_ptr;
    typedef std::shared_ptr<darts_dict> darts_dict_ptr;
    typedef std::shared_ptr<serializable_dict> serializable_dict_ptr;
}  // namespace libnlp::dict

#ifndef PKGDATADIR
const std::string PACKAGE_DATA_DIRECTORY = "";
#else  // ifndef PKGDATADIR
const std::string PACKAGE_DATA_DIRECTORY = PKGDATADIR "/";
#endif // ifndef PKGDATADIR

#ifndef VERSION
#define VERSION "1.0.*"
#endif // ifndef VERSION
