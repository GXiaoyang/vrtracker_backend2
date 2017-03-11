#pragma once

// stack overflow macro to test if a class implements something
// used by result to detect if a value is a container or not
//

#define TYPE_SUPPORTS(ClassName, Expr)                         \
  template<typename U>                                         \
  struct ClassName                                             \
  {                                                            \
  private:                                                     \
    template<typename>                                         \
    static constexpr std::false_type test(...);                \
                                                               \
    template<typename T = U>                                   \
    static decltype((Expr), std::true_type{}) test(int) ;      \
                                                               \
  public:                                                      \
    static constexpr bool value = decltype(test<U>(0))::value; \
  };