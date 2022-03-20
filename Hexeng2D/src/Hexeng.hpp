#ifndef HEXENG_HPP
#define HEXENG_CPP

#include <functional>

#include "Macros.hpp"

namespace Hexeng
{

	HXG_DECLSPEC void game_loop(std::function<void()> pre = nullptr, std::function<void()> post = nullptr);

}

#endif