#include <memory>
#include <string_view>

#include "er/reflection/reflection.h"
#include "er/type_info/type_info.h"
#include "er/variable/box.h"
#include "generated/reflection.h"
#include "gtest/gtest.h"

using namespace er;

TEST(TightString, Sizeof) {
  ASSERT_TRUE(sizeof(std::string) > sizeof(std::string_view));
  ASSERT_EQ(sizeof(TightString), sizeof(Error));
  ASSERT_EQ(sizeof(TightString), sizeof(std::string));
}

TEST(TightString, Ownership) {
  // check correct destroying meanwile
  for (auto i = 0; i < 100; i++) {
    TightString str;
    str = "Constant string which we can easily share";
    ASSERT_FALSE(str.is_owned());
  }

  for (auto i = 0; i < 100; i++) {
    TightString str;
    str = std::string_view("String view with sharable constant string inside");
    ASSERT_FALSE(str.is_owned());
  }

  for (auto i = 0; i < 100; i++) {
    TightString str;
    str = std::string("std::string which own the data in dynamic memory");
    ASSERT_TRUE(str.is_owned());
  }
}

TEST(TypeInfo, VariantIndex) {
  bool b;
  auto info = er::reflection::reflect(&b);
  ASSERT_TRUE(info.is<Bool>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kBool);

  int i;
  info = er::reflection::reflect(&i);
  ASSERT_TRUE(info.is<Integer>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kInteger);

  float f;
  info = er::reflection::reflect(&f);
  ASSERT_TRUE(info.is<Floating>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kFloating);

  std::string str;
  info = er::reflection::reflect(&str);
  ASSERT_TRUE(info.is<String>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kString);

  Numbers e;
  info = er::reflection::reflect(&e);
  ASSERT_TRUE(info.is<Enum>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kEnum);

  Simple o;
  info = er::reflection::reflect(&o);
  ASSERT_TRUE(info.is<Object>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kObject);

  int a[4];
  info = er::reflection::reflect(&a);
  ASSERT_TRUE(info.is<Array>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kArray);

  std::vector<int> seq;
  info = er::reflection::reflect(&seq);
  ASSERT_TRUE(info.is<Sequence>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kSequence);

  std::unordered_map<int, int> m;
  info = er::reflection::reflect(&m);
  ASSERT_TRUE(info.is<Map>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kMap);
}

TEST(SmartPointers, Unique) {
  for (auto i = 0; i < REPEATS; i++) {
    auto unique = std::make_unique<Tresholds>();

    auto type = TypeId::get<std::unique_ptr<Tresholds>>();
    Box box(type);

    auto unique_info = reflection::reflect(&unique);
    auto box_info = reflection::reflect(box.var());

    ASSERT_EQ(unique_info.var().type(), box_info.var().type());

    // copy should move unique_ptr
    reflection::copy(box.var(), Var(&unique));

    ASSERT_EQ(unique.get(), nullptr);
  }
}

TEST(SmartPointers, Shared) {
  for (auto i = 0; i < REPEATS; i++) {
    auto shared = std::make_shared<Tresholds>();
    {
      auto type = TypeId::get<std::shared_ptr<Tresholds>>();
      Box box(type);

      auto shared_info = reflection::reflect(&shared);
      auto box_info = reflection::reflect(box.var());

      ASSERT_EQ(shared_info.var().type(), box_info.var().type());

      reflection::copy(box.var(), Var(&shared));

      ASSERT_EQ(shared.use_count(), 2);
    }

    ASSERT_EQ(shared.use_count(), 1);
  }
}

TEST(Array, ConstructDestroy) {
  static int constructor_calls = 0;
  static int destructor_calls = 0;

  struct Complex {
    explicit Complex() {
      constructor_calls++;
    }
    ~Complex() {
      destructor_calls++;
    }
  };

  for (auto i = 0; i < REPEATS; i++) {
    // test a complex type
    const int n = 5;
    uint8_t mem[n * sizeof(Complex)];

    TypeActions<Complex[n]>::construct(&mem[0]);
    ASSERT_EQ(constructor_calls, n);
    constructor_calls = 0;

    TypeActions<Complex[n]>::destroy(&mem[0]);
    ASSERT_EQ(destructor_calls, n);
    destructor_calls = 0;
  }
}
