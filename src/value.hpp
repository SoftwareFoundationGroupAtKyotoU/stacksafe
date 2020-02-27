#ifndef INCLUDE_GUARD_2B683EB3_DF38_441C_9ED1_AE57D7DAB1A3
#define INCLUDE_GUARD_2B683EB3_DF38_441C_9ED1_AE57D7DAB1A3

namespace stacksafe {

template <typename T>
class Value {
  using Ptr = const T*;
  Ptr ptr_;

 public:
  explicit Value(Ptr p) : ptr_{p} {}
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_2B683EB3_DF38_441C_9ED1_AE57D7DAB1A3
