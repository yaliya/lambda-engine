#pragma once

#include <iostream>

#ifndef NDEBUG
#define ASSERT(condition, message) do { if (!(condition)) { std::cerr << message << std::endl; std::terminate(); }  } while(false)
#endif