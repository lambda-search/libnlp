
#pragma once

#include "libnlp/dict/dict_group.h"
#include "text_dict_test_base.h"

namespace libnlp::dict {

    class DictGroupTestBase : public TextDictTestBase {
    protected:
        dict_group_ptr CreateDictGroupForConversion() const {
            basic_dict_ptr phrasesDict = CreateDictForPhrases();
            basic_dict_ptr charactersDict = CreateDictForCharacters();
            dict_group_ptr dictGroup(
                    new dict_group(std::list<basic_dict_ptr>{phrasesDict, charactersDict}));
            return dictGroup;
        }
    };

} // namespace libnlp::cc
