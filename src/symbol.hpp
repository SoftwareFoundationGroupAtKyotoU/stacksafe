#ifndef INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772
#define INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772

class Symbol {
  static int current_;
  int num_;
  explicit Symbol(int n);

 public:
  static Symbol create();
};

#endif  // INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772
