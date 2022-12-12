#ifndef MAP_H
# define MAP_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "ContainerException.hpp"
# include "iterator.hpp"
# include "pair.hpp"
# include "stack.hpp"
# include "deque.hpp"
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
		typedef typename allocator_type::reference					reference;
		typedef typename allocator_type::const_reference			const_reference;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer				const_pointer;
		typedef typename allocator_type::size_type					size_type;
		typedef typename allocator_type::difference_type			difference_type;
		typedef typename Allocator:: template rebind<Node>::other	node_allocator;
		typedef typename node_allocator::pointer					node_pointer;

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
			bool operator()(const value_type& x, const value_type& y) const { return x == y; }
		};

	private:
		struct Node {
		private:
			value_type	_data;
			Node		*_left;
			Node		*_right;
			int			_balance;

		public:
			Node() : _data(value_type()), _left(NULL), _right(NULL), _balance() {}
			Node(value_type pair) : _data(pair), _left(NULL), _right(NULL), _balance() {}
			~Node() {}

			Node		&operator= (Node &other) { _left = other._left; _right = other._right; _balance = other._balance; return (*this); }
			void		change_left(Node *left) { _left = left; }
			void		change_right(Node *right) { _right = right; }
			void		change_data(value_type &data) { _data = data; }
			Node		*left() { return _left; }
			Node		*right() { return _right; }
			value_type	&data() { return _data; }
			void		treat(char sep) { std::cout << data().first << sep << data().second << sep << std::endl; }

			void		suppress_node()
			{
				if (left())
					left()->suppress_node();
				if (right())
					right()->suppress_node();
				delete (this);
			};
		};

	public:
		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size(1) { std::cout << "seggy" << std::endl; }

		template <class InputIterator>
		map (InputIterator first, InputIterator last,       const key_compare& comp = key_compare(),       const allocator_type& alloc = allocator_type());
		map (const map& x);

		~map() { _root->suppress_node(); };
		map&	operator= (map const& base);
		Node	*root() { return _root; }
		void	change_root(Node *current) { _root = current; }

		void	prefix_traversal(Node *current, char sep) {
			if (current)
			{
				current->treat(sep);
				prefix_traversal(current->left, sep);
				prefix_traversal(current->right, sep);
			}
		}

		void	infix_traversal(Node *current, char sep) {
			if (current)
			{
				infix_traversal(current->left, sep);
				current->treat(sep);
				infix_traversal(current->right, sep);
			}
		}

		void	suffix_traversal(Node *current, char sep) {
			if (current)
			{
				suffix_traversal(current->left, sep);
				suffix_traversal(current->right, sep);
				current->treat(sep);
			}
		}

		void	level_order_traversal(Node *current, char sep) {
			deque<Node *>	deck;

			deck.push_back(current);
			while (!deck.empty())
			{
				current = deck.front();
				deck.pop_front();
				current->treat(sep);
				if (current->left())
					deck.push_back(current->left());
				if (current->right())
					deck.push_back(current->right());
			}
		}

		void	insert(Node *current, value_type &pair) {
			Node *newNode = new Node(pair);

			if (current)
			{
				if (_key_comp(pair.first, current->data().first))
				{
					if (current->left())
						insert(current->left(), pair);
					else
						current->change_left(newNode);
				}
				else
				{
					if (current->right())
						insert(current->right(), pair);
					else
						current->change_right(newNode);
				}
			}
			else
				current = newNode;
		}
/*
		Node	*new_node(value_type content = value_type())
		{
			Node *ptr = _alloc_node.allocate(1);
			_alloc_node.construct(ptr);
			try
			{
				_alloc_pair.construct(&ptr->content, content);
			}
			catch (...)
			{
				_alloc_node.deallocate(ptr, 1);
				throw;
			}
			return (ptr);
		}

		void	delete_node(Node *n)
		{
			_alloc_node.destroy(n);
			_alloc_node.deallocate(n, 1);
		}
*/
//		pair<iterator,bool> insert (const value_type& val);
//		iterator insert (iterator position, const value_type& val);
//		template <class InputIterator>  void insert (InputIterator first, InputIterator last);

	private :
		Node			*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_key_comp;
		size_type		_size;
		node_pointer	_ptr;
	};
}

#endif