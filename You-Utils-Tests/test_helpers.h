//@author A0097630B
#pragma once
#ifndef YOU_UTILS_TESTS_TEST_HELPERS_H_
#define YOU_UTILS_TESTS_TEST_HELPERS_H_

#include <string>
#include <boost/variant/static_visitor.hpp>

namespace You {
namespace NLP {
namespace UnitTests {

/// Applies ToString to the element in the container.
struct ToStringVisitor : public boost::static_visitor<std::wstring> {
	/// Applies ToString to the given element in the variant.
	template<typename T>
	std::wstring operator()(const T& value) {
		return ToString(value);
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

namespace boost {

template<typename T1, typename T2, typename T3>
std::wstring ToString(const variant<T1, T2, T3>& t) {
	You::NLP::UnitTests::ToStringVisitor visitor;
	return apply_visitor(visitor, t);
}

}  // namespace boost

#endif  // YOU_UTILS_TESTS_TEST_HELPERS_H_
