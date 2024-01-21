#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_CONFIG__
#define __MORE_CONFIG__

#include <corecrt.h>

#ifndef _HAS_CXX20
#error "more heads need c++20"
#endif // !_HAS_CXX20

#define _MORE_CRT ::more::_MORE_CRT_NAME
#define _MORE_DETAILS ::more::_MORE_CRT_NAME::details
#define _MORE_CRT_BEGIN namespace more::_MORE_CRT_NAME {
#define _MORE_CRT_END }
#define _MORE_DETAILS_BEGIN namespace details {
#define _MORE_DETAILS_END }
#define _USING_MORE_NAMESPACE(name) using namespace ::more::name

#define _MORE_VERSION 20240121

#endif // !__MORE_CONFIG__