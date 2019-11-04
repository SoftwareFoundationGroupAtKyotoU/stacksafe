#ifndef INCLUDE_GUARD_C1AC39A1_F679_4ECB_9D0B_D02FAC20B878
#define INCLUDE_GUARD_C1AC39A1_F679_4ECB_9D0B_D02FAC20B878

namespace stacksafe {

class Number {
  const int num_;

 public:
  explicit Number(int n);
  int value() const;
};
bool operator<(const Number& lhs, const Number& rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_C1AC39A1_F679_4ECB_9D0B_D02FAC20B878
