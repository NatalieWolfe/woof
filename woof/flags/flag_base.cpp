#include "woof/flags/flag_base.h"

#include <string_view>

#include "woof/flags/registry.h"

namespace woof {

FlagBase::FlagBase(
  std::string_view type_name,
  std::string_view name,
  std::string_view description
):
  _type_name{type_name},
  _name{name},
  _description{description}
{
  register_flag(*this);
}


}
