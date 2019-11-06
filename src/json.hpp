#ifndef INCLUDE_GUARD_E372F549_B09F_460E_B778_3AFBC559D1E8
#define INCLUDE_GUARD_E372F549_B09F_460E_B778_3AFBC559D1E8

#include <nlohmann/json.hpp>
#include <string>
#include "json_fwd.hpp"

namespace stacksafe {
class Value;

std::string get_operand(const Value& v);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E372F549_B09F_460E_B778_3AFBC559D1E8
