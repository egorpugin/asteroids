#pragma once

#include <functional>

class scope_guard {
  using exit_func = std::function<void()>;
public:
  scope_guard(exit_func f) : f(f) {}
  ~scope_guard() { if (f) f(); }
private:
  exit_func f;
};

namespace detail {
  enum class scope_guard_on_exit {};

  template <typename F>
  scope_guard operator+(scope_guard_on_exit, F &&fn) {
    return scope_guard(std::forward<F>(fn));
  }
}

// Concatenate preprocessor tokens A and B without expanding macro definitions
// (however, if invoked from a macro, macro arguments are expanded).
#define CONCATENATE_NX(s1, s2) s1 ## s2

// Concatenate preprocessor tokens A and B after macro-expanding them.
#define CONCATENATE(s1, s2) CONCATENATE_NX(s1, s2)

#define ANONYMOUS_VARIABLE_COUNTER(s) CONCATENATE(CONCATENATE(s, _), __COUNTER__)
#define ANONYMOUS_VARIABLE_LINE(s) CONCATENATE(CONCATENATE(s, _), __LINE__)

#ifdef __COUNTER__
#define PRIMITIVES_ANONYMOUS_VARIABLE(s) ANONYMOUS_VARIABLE_COUNTER(s)
#else
#define PRIMITIVES_ANONYMOUS_VARIABLE(s) ANONYMOUS_VARIABLE_LINE(s)
#endif

#define SCOPE_EXIT \
    auto PRIMITIVES_ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) = SCOPE_EXIT_NAMED

#define SCOPE_EXIT_NAMED \
    ::detail::scope_guard_on_exit() + [&]()
