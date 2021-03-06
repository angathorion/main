// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef YOU_NLP_STDAFX_H_
#define YOU_NLP_STDAFX_H_

#include <SDKDDKVer.h>

#include <string>
#include <stdexcept>

#include "parser_logger.h"  // included out-of-order for boost::spirit logging.

#pragma warning(disable: 4503 4714)  // Boost is too awesome.
#pragma warning(push)
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning(disable: 4127 4100 4244 4512 4555)
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/variant.hpp>
#pragma warning(pop)

#endif  // YOU_NLP_STDAFX_H_
