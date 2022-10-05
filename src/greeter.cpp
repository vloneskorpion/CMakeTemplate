#include "greeter/greeter.hpp"
#include <fmt/core.h>

namespace greeter {
std::string greetings(const std::string& name)
{
	return fmt::format("Good morning, {}!", name);
}
}