//@author A0097630B
#include "stdafx.h"
#include "logger.h"
#include "log_message.h"

namespace You {
namespace Utils {

LogMessage::LogMessage(Logger& log, std::wstring category)
: logger(log), category(std::move(category)) {
}

LogMessage::LogMessage(LogMessage&& rhs)
: logger(rhs.logger),
category(std::move(rhs.category)),
components(std::move(rhs.components)) {
}

LogMessage::~LogMessage() {
	if (!components.empty() && logger.isActive()) {
		logger.write(evaluate(), category);
	}
}

std::wstring LogMessage::evaluate() const {
	std::wstring result;
	std::for_each(begin(components), end(components),
		[&result](const std::function<std::wstring()>& pickle) {
			result += pickle();
		});

	return result;
}

}  // namespace Utils
}  // namespace You
