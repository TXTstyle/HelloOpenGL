#pragma once

#include <iostream>

#define ASSERT(x) if (!(x)) throw std::runtime_error("Assert called!");