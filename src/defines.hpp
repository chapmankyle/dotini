#pragma once

#ifndef DEFINES_HPP
#define DEFINES_HPP

#ifndef ALLOW_MULTILINE
#	define ALLOW_MULTILINE 1
#endif

#ifndef ALLOW_COMMENTS
#	define ALLOW_COMMENTS 1
#endif

#ifndef ALLOW_INLINE_COMMENTS
#	define ALLOW_INLINE_COMMENTS 1
#endif


#ifndef STOP_ON_FIRST_ERROR
#	define STOP_ON_FIRST_ERROR 0
#endif

#if ALLOW_COMMENTS
#	ifndef START_COMMENT_PREFIXES
#		define START_COMMENT_PREFIXES ";#"
#	endif
#endif

#if ALLOW_INLINE_COMMENTS
#	ifndef INLINE_COMMENT_PREFIXES
#		define INLINE_COMMENT_PREFIXES ";"
#	endif
#endif

#define MAX_SECTION_LENGTH 50
#define MAX_KEY_LENGTH     50
#define MAX_LINE_LENGTH    200

#endif //!DEFINES_HPP
