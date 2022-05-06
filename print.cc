
#include <tuple>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

void print(bool x, bool newln = true) {
	std::cout << (x ? "true" : "false");
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

template<class T> inline typename std::enable_if<std::is_member_function_pointer<decltype(&T::str)>::value, void>::type print(T& t, bool newln = true)
{
	std::cout << t.str();
	if (newln) std::cout << "\n";
}

#define dprint(x) { std::cout << #x << " == "; print(x); }

struct point {
	int x, y;
	
	public: point(int x, int y): x(x), y(y) {
	}
	
	std::string str() {
		return "point(x = " + std::to_string(x) + ", y = " + std::to_string(y) + ")";
	}
};

int main()
{
	int x = 3;
	print(x);
	
	print(3 + 4);
	
	double y = 3.3;
	print(y);
	
	std::vector<int> array = {1, 2, 3};
	print(array);
	
	std::tuple<int, float, bool> array2 = std::make_tuple(1, 2.3, true);
	print(array2);
	
	struct point p = point(3, 4);
	
	dprint(p);
	
	return 0;
}


















