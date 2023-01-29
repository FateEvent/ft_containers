#ifndef UTILITIES_H
# define UTILITIES_H

# include <iostream>
# include <type_traits>

namespace ft {

	/****************************************************************************\
	** 																			**
	**						lexicographical_compare utilities					**
	** 																			**
	\****************************************************************************/

	template <class InputIterator1, class InputIterator2>
	bool	lexicographical_compare (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1)
				return (false);
			else if (*first1 < *first2)
				return (true);
			++first1; ++first2;
		}
		return (first2 != last2);
	}
	
	template <class InputIterator1, class InputIterator2, class Compare>
	bool	lexicographical_compare (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
		for (; first1 != last1, first2 != last2; ++first1, ++first2)
		{
			if (comp(*first1, *first2))
				return (true);
			else
				return (false);
		}
		return ((first1 == last1) && (first2 != last2));
	}

	/****************************************************************************\
	** 																			**
	**						equal and equal_to utilities						**
	** 																			**
	\****************************************************************************/
	
	template<class T> struct equal_to
		: public std::binary_function<T, T, bool>
	{
		bool	operator()(const T &lhs, const T &rhs) const {	return lhs == rhs; }
	};

	template <class InputIt1, class InputIt2>
	bool	equal(InputIt1 lhs_begin, InputIt1 lhs_end, InputIt2 rhs_begin)
	{
		while (lhs_begin != lhs_end)
		{
			if (*lhs_begin != *rhs_begin)
				return (false);
			++lhs_begin;
			++rhs_begin;
		}
		return (true);
	}

	/****************************************************************************\
	** 																			**
	**							is_integral utilities							**
	** 																			**
	\****************************************************************************/

	template<class T> struct is_integral { static const bool value = false; };
	template<> struct is_integral<char> { static const bool value = true; };
	template<> struct is_integral<char16_t> { static const bool value = true; };
	template<> struct is_integral<char32_t> { static const bool value = true; };
	template<> struct is_integral<wchar_t> { static const bool value = true; };
	template<> struct is_integral<short int> { static const bool value = true; };
	template<> struct is_integral<int> { static const bool value = true; };
	template<> struct is_integral<long int> { static const bool value = true; };
	template<> struct is_integral<long long int> { static const bool value = true; };
	template<> struct is_integral<uint8_t> { static const bool value = true; };
	template<> struct is_integral<uint16_t> { static const bool value = true; };
	template<> struct is_integral<uint32_t> { static const bool value = true; };
	template<> struct is_integral<unsigned long int> { static const bool value = true; };
	template<> struct is_integral<uint64_t> { static const bool value = true; };
	template<> struct is_integral<bool> { static const bool value = true; };

	/****************************************************************************\
	** 																			**
	**							enable_if utilities								**
	** 																			**
	\****************************************************************************/

	template<bool, class T = void> struct enable_if {};
	template<class T> struct enable_if<true, T> { typedef T value; };

	/****************************************************************************\
	** 																			**
	**									utils									**
	** 																			**
	\****************************************************************************/

	template <class T>
	void	swap_elements(T &lhs, T &rhs)
	{
		T	tmp = lhs;

		lhs = rhs;
		rhs = tmp;
	};

	template <class InputIt>
	size_t	InputIt_get_len(InputIt first, InputIt last)
	{
		size_t	i = 0;

		for (; first != last; first++)
			++i;
		return (i);
	}
};

#endif
