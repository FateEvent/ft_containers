#ifndef STACK_H
# define STACK_H

# include <iostream>
# include "vector.hpp"
#include <stack>

namespace ft
{
	template <class T, class Container = std::vector<T> >
	class stack
	{
	protected:
		Container											c;

	public:
		typedef Container									container_type;
		typedef typename container_type::value_type			value_type;
		typedef typename container_type::reference			reference;
		typedef typename container_type::const_reference	const_reference;
		typedef typename container_type::size_type			size_type;

		explicit stack(const container_type& q = container_type()) : c(q) {}

		stack(const stack& q) : c(q.c) {}

		~stack(void) {};

		stack&	operator= (const stack& q) { c = q.c; return *this; }

		bool	empty()	const { return c.empty(); }

		size_type	size()	const { return c.size(); }

		reference	top(void) { return c.back(); }

		void	push(const value_type& v) { c.push_back(v); }

		void	pop() { c.pop_back(); }

		void	swap(stack& ms) {
			c.swap(ms.c);
		};
	};

	template <class T, class Container>
		bool operator == (const ft::stack<T, Container> &lhs, const ft::stack<T, Container> &rhs) { return lhs.c == rhs.c; };
	template <class T, class Container>
		bool operator != (const ft::stack<T, Container> &lhs, const ft::stack<T, Container> &rhs) { return lhs.c != rhs.c; };
	template <class T, class Container>
		bool operator < (const ft::stack<T, Container> &lhs, const ft::stack<T, Container> &rhs) { return lhs.c < rhs.c; };
	template <class T, class Container>
		bool operator <= (const ft::stack<T, Container> &lhs, const ft::stack<T, Container> &rhs) { return lhs.c <= rhs.c; };
	template <class T, class Container>
		bool operator > (const ft::stack<T, Container> &lhs, const ft::stack<T, Container> &rhs) { return lhs.c > rhs.c; };
	template <class T, class Container>
		bool operator >= (const ft::stack<T, Container> &lhs, const ft::stack<T, Container> &rhs) { return lhs.c >= rhs.c; };
}

#endif