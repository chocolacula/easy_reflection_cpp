#include "numbers.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Numbers> {
  static TypeInfo reflect(void* value, bool is_const) {

    static constexpr ConstexprMap<Numbers, 3> constants{
    // be aware of use sequential '{' with template engines
    {
      {
        {Numbers::kOne, "kOne"},
        {Numbers::kTwo, "kTwo"},
        {Numbers::kThree, "kThree"},
        
    }}};

    return Enum(static_cast<Numbers*>(value), is_const, constants);
  }
};

template <>
TypeId TypeId::get(Numbers* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Numbers>::reflect,        //
                                                 &CommonActions<Numbers>::type_name,    //
                                                 &CommonActions<Numbers>::type_size,    //
                                                 &CommonActions<Numbers>::call_new,     //
                                                 &CommonActions<Numbers>::call_delete,  //
                                                 &CommonActions<Numbers>::copy)));
  return id;
}

}  // namespace er

// clang-format on
