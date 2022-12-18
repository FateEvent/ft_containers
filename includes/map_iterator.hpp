#ifndef MAP_ITERATOR_H
# define MAP_ITERATOR_H

# include <iostream>
# include <memory>
# include <cstddef>
# include "map.hpp";

class map;

namespace ft
{
	template <typename T, typename Node, typename Content>
	class map_iterator
	{
	public :
		typedef	std::bidirectional_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::size_t						size_type;
		typedef std::ptrdiff_t					difference_type;
		typedef Content*						pointer;
		typedef Content&						reference;

		map_iterator() : _ptr(&_present), last(NULL), _present(), _stop() {}
		map_iterator(Node *ptr) : _ptr(ptr), _last(NULL), _present(), _stop() {}

		Node	*base() const { return _ptr; }

		value_type	&operator* () const { return *_ptr; }
		value_type	*operator-> () const { return _ptr; }

		void	infix_traversal(Node *current) {
			if (current)
			{
				infix_traversal(current->left(), sep);
				current->treat(sep);
				infix_traversal(current->right(), sep);
			}
		}

		const map_iterator	&operator++ () {
			if (ptr)
			{
				infix_traversal(ptr->left(), sep);
				if (ptr->_data())
				current->treat(sep);
				infix_traversal(ptr->right(), sep);
			}
		}
/*		const map_iterator 	operator++ (int) { map_iterator tmp = *this; ++(*this); return tmp; }
		const map_iterator	&operator-- () {

		}
		const map_iterator 	operator-- (int) { map_iterator tmp = *this; --(*this); return tmp; }
*/
		const map_iterator	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		const map_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		const map_iterator	operator+ (std::size_t dist) { return (_ptr + dist); }
		const map_iterator	operator- (std::size_t dist) { return (_ptr - dist); }
		const map_iterator	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t			operator- (const map_iterator &it) { return (_ptr - it._ptr); }

		bool		operator== (const map_iterator &it) { return this->base() == it.base(); }
		bool		operator!= (const map_iterator &it) { return this->base() != it.base(); }
		bool		operator< (const map_iterator &it) { return this->base() < it.base(); }
		bool		operator<= (const map_iterator &it) { return this->base() <= it.base(); }
		bool		operator> (const map_iterator &it) { return this->base() > it.base(); }
		bool		operator>= (const map_iterator &it) { return this->base() >= it.base(); }

	private:
		Node	*_ptr;
		Node	*_last;
		Node	_present;
		int		_stop;
	};
}

#endif