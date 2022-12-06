#ifndef MAP_H
# define MAP_H

# include <iostream>
# include <memory>
# include <algorithm>
# include "iterator.hpp"
//# include "pair.hpp"
# include "ContainerException.hpp"
#include <map>
#include <utility>

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, T> > > class map {

	public:
		typedef Key											key_type;
		typedef T											mapped_type;
//		typedef pair<const key_type, mapped_type>			value_type;
		typedef Compare										key_compare;
		typedef Allocator									allocator_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef typename allocator_type::size_type			size_type;
		typedef typename allocator_type::difference_type	difference_type;

//		typedef implementation-defined                   iterator;
//		typedef implementation-defined                   const_iterator;
//		typedef std::reverse_iterator<iterator>          reverse_iterator;
//		typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;

	struct Node
	{
	public:
		value_type	data;
		Node		*left;
		Node		*right;
	};
		

	public:
		map();
		map(map const& base);
		~map();
		map& operator=(map const& base);
	};
}

#endif
