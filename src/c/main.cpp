#include <pybind11/pybind11.h>
#include <coverfloat.h>

namespace py = pybind11;

std::string hello_from_bin() { return "Hello from cover-float!"; }

std::string run_test_vector(const std::string &test_vector, bool suppress_error_check = true) {
  char* output = nullptr;

  int res = coverfloat_runtestvector(
      test_vector.c_str(),
      test_vector.size(),
      &output,
      suppress_error_check
  );

  std::string result (output);
  free(output);

  if (res != EXIT_SUCCESS) {
      throw py::value_error("Error running test vector: " + result);
  }

  return result;
}

PYBIND11_MODULE(_core, m) {
  m.doc() = "pybind11 hello module";

  m.def("hello_from_bin", &hello_from_bin, R"pbdoc(
      A function that returns a Hello string.
  )pbdoc");

  m.def("run_test_vector", &run_test_vector, R"pbdoc(
      Run the given vector through the coverfloat reference model.
  )pbdoc", py::arg("test_vector"), py::arg("suppress_error_check") = true);
}
