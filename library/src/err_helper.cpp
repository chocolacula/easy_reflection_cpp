#include "er/info/type_info/variants/sequence/err_helper.h"

#include "er/error/error.h"
#include "er/reflection/reflection.h"
#include "er/tools/format.h"
#include "er/types/all_types.h"

using namespace rr;
using namespace rr::sequence;

Error ErrHelper::error(std::string_view message, TypeId type, TypeId nested_type) {
  return Error(format(message,                      //
                      reflection::type_name(type),  //
                      reflection::type_name(nested_type)));
}
