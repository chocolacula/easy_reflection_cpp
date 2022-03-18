#include "colors.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Colors> {
  static TypeInfo reflect(void* value, bool is_const) {

    static constexpr ConstexprMap<Colors, 6> constants{
    // be aware of use sequential '{' with template engines
    {
      {
        {Colors::kWhite, "kWhite"},
        {Colors::kRed, "kRed"},
        {Colors::kGreen, "kGreen"},
        {Colors::kBlue, "kBlue"},
        {Colors::kBlack, "kBlack"},
        {Colors::kGray, "kGray"},
        
    }}};

    return Enum(static_cast<Colors*>(value), is_const, constants);
  }
};

template <>
TypeId TypeId::get(Colors* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Colors>::reflect,        //
                                                 &CommonActions<Colors>::type_name,    //
                                                 &CommonActions<Colors>::type_size,    //
                                                 &CommonActions<Colors>::call_new,     //
                                                 &CommonActions<Colors>::call_delete,  //
                                                 &CommonActions<Colors>::copy)));
  return id;
}

}  // namespace er

// clang-format on
