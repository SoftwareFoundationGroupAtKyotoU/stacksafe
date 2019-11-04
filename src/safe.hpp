#ifndef INCLUDE_GUARD_83A9DF1A_4262_4B6D_9E02_0C43F6C3474B
#define INCLUDE_GUARD_83A9DF1A_4262_4B6D_9E02_0C43F6C3474B

namespace stacksafe {

class Safe {
  bool safe_;

 public:
  Safe();
  explicit operator bool() const;
  void unsafe();
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_83A9DF1A_4262_4B6D_9E02_0C43F6C3474B
