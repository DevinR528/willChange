
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include <fmt/core.h>
#include <functional>
#include <type_traits>
#include <variant>

namespace zade {

template<class OkVal, class ErrVal>
struct Result {
  private:
	template <bool Can, class Empty = void> using enable_if_t = typename std::enable_if<Can, Empty>::type;

	bool m_has_value;
	std::variant<OkVal, ErrVal> m_value;

  public:
	Result() = default;
	Result(const Result& res) = default;
	Result(Result&& res) = default;

	template<class ...Args, enable_if_t<std::is_constructible<OkVal, Args&&...>::value>* = nullptr>
	Result(Args&&... args) :
		m_has_value(true),
		m_value(std::variant<OkVal, ErrVal>(std::forward<Args>(args)...)) {
			fmt::print("OkVal from Args... pack\n");
		}
	template<class ...Args, enable_if_t<std::is_constructible<ErrVal, Args&&...>::value>* = nullptr>
	Result(Args&&... args) :
		m_has_value(false),
		m_value(std::variant<OkVal, ErrVal>(std::forward<Args>(args)...)) {
			fmt::print("OkVal from Init List and Args... pack\n");
		}

	template<class U, class ...Args, enable_if_t<std::is_constructible<OkVal, std::initializer_list<U> &, Args&&...>::value>* = nullptr>
	Result(std::initializer_list<U> il, Args&&... args) :
		m_has_value(true),
		m_value(std::variant<OkVal, ErrVal>(il, std::forward<Args>(args)...)) {
			fmt::print("ErrVal from Args... pack\n");
		}

	template<class U, class ...Args, enable_if_t<std::is_constructible<ErrVal, std::initializer_list<U> &, Args&&...>::value>* = nullptr>
	Result(std::initializer_list<U> il, Args&&... args) :
		m_has_value(false),
		m_value(std::variant<OkVal, ErrVal>(il, std::forward<Args>(args)...)) {
			fmt::print("ErrVal from Init List and Args... pack\n");
		}

	constexpr bool has_value() const noexcept { return this->m_has_value; }
	constexpr explicit operator bool() const noexcept { return this->m_has_value; }


	constexpr OkVal& value() & { return std::get<0>(this->m_value); }
	constexpr ErrVal& error() & { return std::get<1>(this->m_value); }

	// template<class Fn, class Ret = decltype(std::mem_fn(std::declval<Fn>()))>
	// constexpr Ret and_then(Fn&& func) & {
	// 	return this->has_value() ? std::invoke(std::forward<Fn>(func), *this) : Ret(this->error());
	// }

};

}  // namespace zade