
#include <meta>
#include <print>


enum struct Fruit
{
	Apple,
	Orange,
	Pear,
};

int main ()
{
	constexpr std::meta::info i = ^^Fruit::Apple;
	std::print("{}\n", std::meta::display_string_of(i));
}