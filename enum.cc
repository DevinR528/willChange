
template<> struct enum_value<> {
	
	std::string name;
	unsigned value;
	
	public: enum_value(std::string name, unsigned value): name(name), value(value) {
	}
	
	constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
	}
	
	template<typename FormatContext>
	auto format(const zade::Token& tkn, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), "enum_value({})", name);
	}
};
