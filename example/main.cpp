#include <cfloat>
#include <iostream>
#include <utility>

#include "er/serialization/json.h"
#include "er/serialization/yaml.h"
#include "generated/reflection.h"
#include "print.h"

using namespace er;

int main() {
  // You can get an Enum constant string representation by reflecting them
  auto white = Colors::kWhite;

  // reflection::reflect(ptr) returns a TypeInfo structure
  // which could represent Integer, Floating, Enum and other kinds of types
  auto white_info = reflection::reflect(&white);

  // You can check kind of the TypeInfo by calling is<Type>() and get it further
  if (white_info.is<Enum>()) {
    println("it's a string representation of {}", white_info.get<Enum>().to_string());
  }

  // or use type matching
  white_info.match([](Enum& e) { println("Matched and printed: {}", e.to_string()); },
                   [](auto&& /*unused*/) { println("It's not an enum"); });

  println();

  // it is possible to parse enum constant from string value too
  auto the_enum = white_info.get<Enum>();

  the_enum.parse("kPurple").match(  //
      [&](None) { println("kWhite recolored to {}", the_enum.to_string()); },
      [](Error& err) { println("Got an error message: {}", err.what()); });

  // if the constant doesn't exist
  // an Error object with message will be returned
  the_enum.parse("kPurple").match(  //
      [](None) { println("Got an enum constant"); },
      [](Error& err) { println("Got an error message: {}", err.what()); });

  println();

  // it works for old plain enum type but in this case there isn't any generated code for 'Animals'
  auto giraffe = Animals::kGiraffe;
  try {
    auto giraffe_info = reflection::reflect(&giraffe);
  } catch (const std::exception& ex) {
    println("Reflection throws the exception: '{}'", ex.what());
  }

  println();

  // and of course you can use structs and classes
  Bicycle gt_avalanche;
  gt_avalanche.id = 34894613;
  gt_avalanche.manufacturer = "GT";
  gt_avalanche.model = "Avalanche";
  gt_avalanche.wheel_size_inch = 26;
  gt_avalanche.frame_weight = 12;
  gt_avalanche.colors = {Colors::kBlack, Colors::kRed};

  auto bicycle_info = reflection::reflect(&gt_avalanche);

  // setting a value is possible with runtime but fast type checking and casting
  // Note: field 'frame_weight' reflected with alias 'weight'
  auto* field_p = bicycle_info.get<Object>().get_field("weight").unwrap().var().rt_cast<float>().unwrap();
  *field_p = 14.9F;

  // or by using separate reflection step for a field
  auto field_var = bicycle_info.get<Object>().get_field("weight").unwrap().var();
  auto ok = reflection::reflect(field_var).get<Floating>().set(DBL_MAX);

  if (ok.is_error()) {
    println("The value too big for a float field");
  } else {
    println("New value has been set");
  }

  println();

  // you can even use static fields without an instance
  Bicycle* bicycle_ptr = nullptr;
  auto static_info = reflection::reflect(bicycle_ptr);
  const auto* is_cool = static_info.get<Object>().get_field("kIsCool").unwrap().var().rt_cast<const bool>().unwrap();

  if (*is_cool) {
    println("Bikes are definatelly cool");
  } else {
    println("Bikes are boring");
  }

  // for debug purposes there is a possibility to print out struct content
  // via print() and sprint() functions in Reflection namespace
  println();
  reflection::print(bicycle_info);

  // TypeInfo made from nullptr could print static fields only
  println("\n{}", reflection::sprint(static_info));

  // and for a sweet one you can serialize to struct and vice vera
  const auto* str =
      "{\"weight\":16.400000,"
      "\"model\":\"Aurum\","
      "\"colors\":[\"kGray\",\"kGreen\"],"
      "\"wheel_size\":27.500000,"
      "\"manufacturer\":\"Norco\","
      "\"is_hardtail\":false,"
      "\"id\":17893448}";

  auto norco_aurum = serialization::json::from_string<Bicycle>(str).unwrap();
  reflection::print(&norco_aurum);

  // even a map could be deserialized, it works with two fields by default:
  // 'key' for a key
  // 'val' for a value
  // only two fields allowed, the order doesn't metter
  // with special tag '!!map|key:val' in the start of a JSON array fields names could be implicitly specified
  const auto* m_str =
      "[\"!!map|game:price_usd\","
      "{\"game\":\"Blasphemous\",\"price_usd\":34},"
      "{\"price_usd\":60,\"game\":\"Call Of Duty\"}]";
  auto m = serialization::json::from_string<std::map<std::string, int>>(m_str).unwrap();

  println("\n{}", serialization::json::to_string(&m).unwrap());

  return 0;
}