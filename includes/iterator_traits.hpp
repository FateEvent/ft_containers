#ifndef ITERATOR_TRAITS_H
# define ITERATOR_TRAITS_H

# include <iostream>
# include <memory>
# include "ArrayException.hpp"

namespace ft
{
	template<class Iter> struct iterator_traits
	{
		typedef typename Iter::iterator_category	iterator_category; //§19.2.3
		typedef typename Iter::value_type			value_type; // type of element typedef typename Iter::difference_type difference_type;
		typedef typename Iter::pointer				pointer; // return type of operator– >() typedef typename Iter::reference reference; // return type of operator*()
	};

	template<class T> struct iterator_traits<T*>
	{ // specialization for pointers typedef random_access_iterator_tag iterator_category;
		typedef T			value_type;
		typedef ptrdiff_t	difference_type;
		typedef T			*pointer;
		typedef T			&reference;
	};
}

#endif