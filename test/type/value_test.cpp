
//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// value_test.cpp
//
// Identification: /peloton/test/type/value_test.cpp
//
// Copyright (c) 2015, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "common/harness.h"

#include "storage/tuple.h"
#include "type/value.h"

namespace peloton {
namespace test {

//===--------------------------------------------------------------------===//
// Value Tests
//===--------------------------------------------------------------------===//

class ValueTests : public PelotonTest {};

TEST_F(ValueTests, RawSetTest) {
  // This is going to be a nasty ride, so strap yourself in...

  // Make an integer
  int intVal = 9999;

  // Then make a Value with nothing in it
  // It should definitely be equivalent to NULL
  type::Value val;
  EXPECT_TRUE(val.IsNull());

  // Now use the dirty RawSet to update the object directly
  // without having to create a new Value object
  val.RawSet(type::Type::INTEGER, &intVal);

  EXPECT_FALSE(val.IsNull());
  EXPECT_TRUE(val.IsInlined());

  auto newVal = type::ValueFactory::GetIntegerValue(intVal);
  auto result = val.CompareEquals(newVal);
  EXPECT_EQ(type::CmpBool::CMP_TRUE, result);
}

TEST_F(ValueTests, VarcharCopyTest) {
  std::string str = "hello hello world";

  // Try it once with and without a pool
  for (int i = 0; i < 2; i++) {
    type::VarlenPool *pool = nullptr;
    if (i == 0) {
      pool = TestingHarness::GetInstance().GetTestingPool();
    }
    type::Value val1 = type::ValueFactory::GetVarcharValue(str, pool);
    type::Value val2 = val1.Copy();

    // The Value objects should not be the same
    EXPECT_NE(val1.GetData(), val2.GetData());

    // But their underlying data should be equal
    auto result = val1.CompareEquals(val2);
    EXPECT_EQ(type::CmpBool::CMP_TRUE, result);
  }  // FOR
}
}
}
