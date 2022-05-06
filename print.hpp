
#include <tuple>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

void print(bool newln = true) {
	if (newln) std::cout << "\n";
}

void print(bool x, bool newln = true) {
	std::cout << (x ? "true" : "false");
	if (newln) std::cout << "\n";
}

void print(char* x, bool newln = true) {
	std::cout << x;
	if (newln) std::cout << "\n";
}

void print(const char* x, bool newln = true) {
	std::cout << x;
	if (newln) std::cout << "\n";
}

void print(int x, bool newln = true) {
	std::cout << x;
	if (newln) std::cout << "\n";
}

void print(float x, bool newln = true) {
	std::cout << x;
	if (newln) std::cout << "\n";
}

void print(double x, bool newln = true) {
	std::cout << x;
	if (newln) std::cout << "\n";
}

void print(std::string value, bool newln = true) {
	std::cout << value << "\n";
	if (newln) std::cout << "\n";
}

void print(std::string_view value, bool newln = true) {
	std::cout << value << "\n";
	if (newln) std::cout << "\n";
}

template<class T> void print(std::vector<T> value, bool newln = true) {
	std::cout << "[";
	for (int i = 0, n = value.size(); i < n; i++)
	{
		print(value[i], false);
		
		if (i + 1 < n)
		{
			std::cout << ", ";
		}
	}
	std::cout << "]";
	if (newln) std::cout << "\n";
}

template<std::size_t I = 0, typename... Tp> inline typename std::enable_if<I == sizeof...(Tp), void>::type print(std::tuple<Tp...>& t, bool newln = true)
{
	;
}

template<std::size_t I = 0, typename... Tp> inline typename std::enable_if<I  < sizeof...(Tp), void>::type print(std::tuple<Tp...>& t, bool newln = true)
{
	if (!I) std::cout << "(";
	print(std::get<I>(t), false);
	if (I + 1 < sizeof...(Tp))
	{
		std::cout << ", ";
		print<I + 1, Tp...>(t);
	}
	if (!I) std::cout << ")";
	if (!I && newln) std::cout << "\n";
}

template<typename F, typename S, typename... Tp> inline void print(F x, S y, Tp... t)
{
	print(x, false);
	print(y, t...);
}

template<class T> inline typename std::enable_if<std::is_member_function_pointer<decltype(&T::str)>::value, void>::type print(T& t, bool newln = true)
{
	std::cout << t.str();
	if (newln) std::cout << "\n";
}

#define dprint(x) { std::cout << #x << " == "; print(x); }


















