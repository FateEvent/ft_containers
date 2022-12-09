#ifndef MAP_H
# define MAP_H

# include <iostream>
# include <memory>
# include <algorithm>
# include "ContainerException.hpp"
# include "iterator.hpp"
# include "pair.hpp"
#include <map>
#include <utility>

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > > class map {

		struct Node;
	public:
		typedef Key													key_type;
		typedef T													mapped_type;
		typedef pair<const key_type, mapped_type>					value_type;
		typedef Compare												key_compare;
		typedef Allocator											allocator_type;
		typedef typename Allocator:: template rebind<Node>::other	node_allocator;
		typedef typename allocator_type::reference					reference;
		typedef typename allocator_type::const_reference			const_reference;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer				const_pointer;
		typedef typename allocator_type::size_type					size_type;
		typedef typename allocator_type::difference_type			difference_type;

//		typedef implementation-defined                   iterator;
//		typedef implementation-defined                   const_iterator;
//		typedef std::reverse_iterator<iterator>          reverse_iterator;
//		typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;

		class value_compare
			: public std::binary_function<value_type, value_type, bool>
		{
			friend class map;

		protected:
			key_compare	comp;

			value_compare(key_compare c);

		public:
			bool operator()(const value_type& x, const value_type& y) const { return x == y; };
		};

	private:
/*		struct node 
	{
		node& operator=(node& other) {left = other.left; right = other.right; parent = other.parent; color = other.color; return(*this);}
		node(): left(), right(), parent(), color(red), content() {}
		struct node *left;
		struct node *right;
		struct node *parent; 
		color_t color;
		value_type  content;
		~node() {}
	};
*/
		struct Node {
		private:
			Node			*_left;
			Node			*_right;
			Node			*_parent;

		public:
			Node() : _left(nullptr), _right(nullptr), _parent(nullptr) {}
			~Node() {}
		};

	public:
		map();
		map(map const& base);
		~map();
		map& operator=(map const& base);

	private :
		Node			*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_comp;
	};
}

#endif
