#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_CONFIG__
#define __MORE_CONFIG__

#include <corecrt.h>

#ifndef _HAS_CXX20
#error "more heads need c++20"
#endif // !_HAS_CXX20

#define _MORE_VERSION 20240116

#endif // !__MORE_CONFIG__