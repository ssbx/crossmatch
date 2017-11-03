#include <string>

// If defined, use a malloc-based allocator for String instead
// of the standard pool allocator. The pool allocator causes ccmalloc
// to complain incorrectly about memory leaks.

#ifdef STRING_LEAK_DEBUG

#if __GNUC__ >= 3
#	include <bits/stl_alloc.h>
#else
#	include <stl_alloc.h>
#endif

	typedef std::basic_string<char,
#if __GNUC__ >= 3
			std::char_traits<char>,
			std::__allocator<char, std::__malloc_alloc_template<0> >
#else
			std::string_char_traits<char>,
			std::malloc_alloc
#endif
			 >
		dbgstring;

	typedef dbgstring String;

#else
	typedef std::string String;
#endif
