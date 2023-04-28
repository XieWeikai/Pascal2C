#include "code_generation/ast_adapter.h"
#include "gtest/gtest.h"
#include <memory>

using namespace pascal2c::code_generation;
using namespace std;

class SubprogramTest : ::testing::Test {
  protected:
    void SetUp() override {
        string name = "test_subprogram";
        auto arg_by_reference = make_shared<Argument>();
    }

    string expected_ccode_ =
        R"(void test_subprogram(int *by_reference, int[] by_value) {
  *by_reference = by_value * 1;
  by_value[3] = 5;
})";
    shared_ptr<Subprogram> subprogram_;
};