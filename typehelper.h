#pragma once

template<typename T>
struct has_value_type
{
private:
	template<typename C> static constexpr std::true_type test(typename C::value_type);
	template<typename C> static constexpr std::false_type  test(...);
public:
	static constexpr bool value = decltype(test<T>(0))::value; \
		typedef T type;
};


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