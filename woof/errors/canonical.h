#pragma once

#include <experimental/source_location>
#include <string_view>

#include "woof/errors/error.h"

namespace woof {

#define _WOOF_DEFINE_ERROR(ErrorName)                 \
  class ErrorName : public ::woof::Error {            \
  public:                                             \
    explicit ErrorName(                               \
      const std::experimental::source_location& loc = \
        std::experimental::source_location::current() \
    ):                                                \
      ::woof::Error(#ErrorName, loc)                  \
    {}                                                \
  }

_WOOF_DEFINE_ERROR(AbortedError);
_WOOF_DEFINE_ERROR(AlreadyExistsError);
_WOOF_DEFINE_ERROR(CancelledError);
_WOOF_DEFINE_ERROR(DeadlineExceededError);
_WOOF_DEFINE_ERROR(FailedPreconditionError);
_WOOF_DEFINE_ERROR(InternalError);
_WOOF_DEFINE_ERROR(InvalidArgumentError);
_WOOF_DEFINE_ERROR(NotFoundError);
_WOOF_DEFINE_ERROR(OutOfRangeError);
_WOOF_DEFINE_ERROR(PermissionDeniedError);
_WOOF_DEFINE_ERROR(ResourceExhaustedError);
_WOOF_DEFINE_ERROR(UnavailableError);
_WOOF_DEFINE_ERROR(UnimplementedError);

#undef _WOOF_DEFINE_ERROR

}
