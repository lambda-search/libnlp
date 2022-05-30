
#include "libnlp/normalize/cc/libnlp_cc.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(nlp_cc, m) {


#ifdef VERSION
  m.attr("__version__") = VERSION;
#else
  m.attr("__version__") = "dev";
#endif
}
