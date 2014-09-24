#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_
#define YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_

#include <stdexcept>

namespace You {
namespace QueryEngine {

class Exception : public std::exception {
};

class EmptyTaskDescriptionException : public Exception {
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_