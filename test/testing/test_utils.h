
#pragma once

#include "gtest/gtest.h"
#include <string>

namespace libnlp {

using std::string;

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif // ifdef _MSC_VER

#define EXPECT_VECTOR_EQ(expected, actual)                                     \
  {                                                                            \
    const auto& a1 = (expected);                                               \
    const auto& a2 = (actual);                                                 \
    EXPECT_EQ(a1.size(), a2.size());                                           \
    for (size_t i = 0; i < a1.size(); i++) {                                   \
      EXPECT_EQ(a1[i], a2[i]) << "Where i = " << i;                            \
    }                                                                          \
  }

} // namespace libnlp
