//@author A0097630B
#pragma once
#ifndef YOU_UTILS_LOG_LOGGER_H_
#define YOU_UTILS_LOG_LOGGER_H_

#include <string>
#include <boost/lexical_cast.hpp>

#include "log_message.h"
#include "log_severity.h"

namespace You {
namespace Utils {

/// This \ref Logger class deals with logging one severity of message. It can
/// also be used to specify the category of the message, using operator[].
///
/// \ref Logger instances instantiate \ref LogMessage objects, which when
/// destroyed, flushes the message to the log.
///
/// \ref LogMessage objects lazily evaluate their arguments, so that no
/// computation overhead is incurred if the message is not used (because of
/// filtering)
class Logger {
	friend class Log;

public:
	/// Returns a new \ref LogMessage that has the given category specified.
	///
	/// \param[in] category The category of the message to construct.
	template<typename TCategory>
	LogMessage operator[](const TCategory& category) {
		LogMessage result(*this, boost::lexical_cast<std::wstring>(category));

		return result;
	}

	/// Returns a new \ref LogMessage that has no category.
	///
	/// \param[in] message The message to construct. This can be implicitly
	///                    convertible to a string.
	template<typename TMessage>
	LogMessage operator<<(const TMessage& message) {
		LogMessage result(*this);
		result << message;

		return std::move(result);
	}

	/// Checks if the current logger is active. If the current logger is not
	/// active then all messages streamed to the logger is silently discarded.
	bool isActive() const;

	/// Writes the given message and category to the log.
	void write(const std::wstring& message,
		const std::wstring& category = std::wstring());

private:
	Logger(LogSeverity severity);
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

private:
	LogSeverity severity;
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOGGER_H_
