#ifndef STRONG_TYPEDEF_HPP_
#define STRONG_TYPEDEF_HPP_

#include <functional>
#include <type_traits>

template <typename T, typename Tag> class StrongType;

inline constexpr std::false_type strong_typedef_supports_numerics(...) {
  return {};
}

template <typename T, typename Tag> class StrongType {
  static_assert(std::is_pod<T>{}, "`T` must be a POD type.");

public:
  using value_type = T;
  using tag_type = Tag;

  StrongType() noexcept
      : value_{strong_typedef_default_value((Tag *)nullptr)} {}

  explicit StrongType(T init_value) noexcept : value_{init_value} {}

  T value() const { return value_; }

  operator T() const { return value(); }

  struct Delta : StrongType {
    using StrongType::StrongType;
  };

  StrongType &operator+=(Delta d);
  StrongType &operator-=(Delta d);

  struct Hash {};

private:
  T value_;
};

#define STRONG_TYPEDEF_PASTE_2_(a, b) a##b
#define STRONG_TYPEDEF_PASTE_(a, b) STRONG_TYPEDEF_PASTE_2_(a, b)

#define STRONG_TYPEDEF(TYPE, NAME)                                             \
  STRONG_TYPEDEF_WITH_DEFAULT(TYPE, NAME, TYPE{})

#define STRONG_TYPEDEF_WITH_DEFAULT(TYPE, NAME, VALUE)                         \
  struct STRONG_TYPEDEF_PASTE_(NAME, _TAG);                                    \
  inline TYPE strong_typedef_default_value(                                    \
      STRONG_TYPEDEF_PASTE_(NAME, _TAG) *) {                                   \
    return VALUE;                                                              \
  }                                                                            \
  using NAME = ::StrongType<TYPE, STRONG_TYPEDEF_PASTE_(NAME, _TAG)>

#define STRONG_TYPEDEF_SUPPORTS_NUMERICS(NAME)                                 \
  inline constexpr std::true_type strong_typedef_supports_numerics(            \
      STRONG_TYPEDEF_PASTE_(NAME, _TAG) *) {                                   \
    return {};                                                                 \
  }

#define STRONG_TYPEDEF_NUMERIC_OPERATOR_DEFN(op_long, op_short)                \
  template <typename T, typename Tag,                                          \
            typename = std::enable_if_t<strong_typedef_supports_numerics(      \
                (Tag *)nullptr)>>                                              \
  StrongType<T, Tag> op_long(StrongType<T, Tag> a, StrongType<T, Tag> b) {     \
    return StrongType<T, Tag>{a.value() op_short b.value()};                   \
  }

STRONG_TYPEDEF_NUMERIC_OPERATOR_DEFN(operator+, +)
STRONG_TYPEDEF_NUMERIC_OPERATOR_DEFN(operator-, -)
STRONG_TYPEDEF_NUMERIC_OPERATOR_DEFN(operator*, *)
STRONG_TYPEDEF_NUMERIC_OPERATOR_DEFN(operator/, /)

template <typename T, typename Tag>
inline StrongType<T, Tag> &StrongType<T, Tag>::operator+=(Delta d) {
  static_assert(strong_typedef_supports_numerics((Tag *)nullptr),
                "This StrongType does not support numeric operations; see "
                "STRONG_TYPEDEF_SUPPORTS_NUMERICS");

  value_ += d;

  return *this;
}

#endif // STRONG_TYPEDEF_HPP_
