
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include <functional>
#include <type_traits>
#include <utility>
#include <variant>

namespace zade {

template<class T, class E>
class Result {
  private:
	template <bool Can, class Empty = void> using enable_if_t = typename std::enable_if<Can, Empty>::type;

    bool m_has_value;
    std::variant<T, E> m_value;

  public:
    template<class ...Args, enable_if_t<std::is_constructible<T, Args&&...>::value>* = nullptr>
    Result(Args&&... args) :
        m_value(std::variant<T, E>::variant(std::forward<Args>(args)...)),
        m_has_value(true) {}
    template<class ...Args, enable_if_t<std::is_constructible<E, Args&&...>::value>* = nullptr>
    Result(Args&&... args) :
        m_value(std::variant<T, E>::variant(std::forward<Args>(args)...)),
        m_has_value(false) {}

	template<class U, class ...Args, enable_if_t<std::is_constructible<T, std::initializer_list<U> &, Args&&...>::value>* = nullptr>
    Result(std::initializer_list<U> il, Args&&... args) :
        m_value(std::variant<T, E>::variant(il, std::forward<Args>(args)...)),
        m_has_value(true) {}

	template<class U, class ...Args, enable_if_t<std::is_constructible<E, std::initializer_list<U> &, Args&&...>::value>* = nullptr>
    Result(std::initializer_list<U> il, Args&&... args) :
        m_value(std::variant<T, E>::variant(il, std::forward<Args>(args)...)),
        m_has_value(false) {}
};

}  // namespace zade
