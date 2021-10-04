#pragma once

#include "actions.h"
#include "er/types/unknown.h"

namespace rr {

/// the main component of reflection system - big table with function pointers
/// each reflectable type has to have a record in the table
struct TheGreatTable {

  static const std::vector<Actions>& data() {
    return _data;
  }

  static size_t record(Actions actions) {
    _data.push_back(actions);
    return _data.size() - 1;
  }

 private:
  // zero index for unknown type
  static inline std::vector<Actions> _data = {Actions(&UnknownActions::reflect,      //
                                                      &UnknownActions::type_name,    //
                                                      &UnknownActions::type_size,    //
                                                      &UnknownActions::call_new,     //
                                                      &UnknownActions::call_delete,  //
                                                      &UnknownActions::copy)};
};

}  // namespace rr