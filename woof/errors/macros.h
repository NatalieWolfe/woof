#pragma once

#include <utility>

#include "woof/errors/canonical.h"

#define _WOOF_ERRORS_UNWRAP_STRING_IMPL(x) #x
#define _WOOF_ERRORS_UNWRAP_STRING(x) _WOOF_ERRORS_UNWRAP_STRING_IMPL x

#define WOOF_CHECK_NULL(ErrorClass, arg)  \
  while ((arg) == nullptr) [[unlikely]]   \
    throw ErrorClass() << #arg << " must not be null. "

#define _WOOF_CHECK_GT_IMPL(ErrorClass, lhs, rhs)                       \
  for (                                                                 \
    const auto& [lval, rval] = std::make_pair((lhs), (rhs));            \
    !(lval > rval);                                                     \
  ) [[unlikely]]                                                        \
    throw ErrorClass()                                                  \
      << "Expected `" _WOOF_ERRORS_UNWRAP_STRING(lhs) "` (" << lval     \
      << ") to be greater than `" _WOOF_ERRORS_UNWRAP_STRING(rhs) "` (" \
      << rval << "). "

#define WOOF_CHECK_GT(ErrorClass, lhs, rhs) \
  _WOOF_CHECK_GT_IMPL(ErrorClass, (lhs), (rhs))

#define _WOOF_CHECK_GTE_IMPL(ErrorClass, lhs, rhs)                            \
  for (                                                                       \
    const auto& [lval, rval] = std::make_pair((lhs), (rhs));                  \
    !(lval >= rval);                                                          \
  ) [[unlikely]]                                                              \
    throw ErrorClass()                                                        \
      << "Expected `" _WOOF_ERRORS_UNWRAP_STRING(lhs) "` (" << lhs            \
      << ") to be greater than or equal to `" _WOOF_ERRORS_UNWRAP_STRING(rhs) \
         "` (" << rhs << "). "

#define WOOF_CHECK_GTE(ErrorClass, lhs, rhs) \
  _WOOF_CHECK_GTE_IMPL(ErrorClass, (lhs), (rhs))
