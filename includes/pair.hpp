#ifndef PAIR_H
# define PAIR_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>

namespace ft
{
	template<class T1, class T2> struct pair {

		typedef T1	first_type;
		typedef T2	second_type;

		T1 first;
		T2 second;

		pair() : first(), second() {}
		pair(first_type const& t1, second_type const& t2) : first(t1), second(t2) {}
		pair(const pair& p) : first(p.first), second(p.second) {}
		template<class U, class V> pair(const pair<U, V>& p) : first(T1(p.first)), second(T2(p.second)) {}

		pair&	operator= (pair const& p) {
			first = p.first;
			second = p.second;
			return (*this);
		}

		template<class U, class V> pair&	operator= (const pair<U, V>& p) {
			first = T1(p.first);
			second = T2(p.second);
			return (*this);
		}
	};

	template<class T1, class T2>
	ft::pair<T1, T2>	make_pair(T1 first, T2 second) {
		ft::pair<T1, T2> p(first, second);
		return (p);
	}

	template<class T1, class T2>
	bool	operator== (const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) { return (lhs.first == rhs.first && lhs.second == rhs.second); }
	template<class T1, class T2>
	bool	operator!= (const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) { return (!(lhs == rhs)); }
	template<class T1, class T2>
	bool	operator< (const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) { return (lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second)); }
	template<class T1, class T2>
	bool	operator<= (const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) { return (!(rhs < lhs)); }
	template<class T1, class T2>
	bool	operator> (const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) { return (rhs < lhs); }
	template<class T1, class T2>
	bool	operator>= (const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs) { return (!(lhs < rhs)); }
}

#endif