#ifndef MAP_H
# define MAP_H

# include <iostream>
# include <memory>
# include <algorithm>
# include "ContainerException.hpp"
# include "iterator.hpp"
# include "pair.hpp"
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
			Node() : _data(value_type()), _left(nullptr), _right(nullptr), _balance() {}
			Node(value_type pair) : _data(pair), _left(nullptr), _right(nullptr), _balance() {}
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

		class Tree {
			Node	*_root;
		public:
			Tree() : _root(nullptr) {}
			~Tree() { _root->suppress_node(); }

			Node	&root() { return _root; }
			void	change_root(Node *current) { _root = current; }
			void	prefix_traversal(Node *current, char sep) {
				if (current)
				{
					current->treat();
					prefix_traversal(current->left, sep);
					prefix_traversal(current->right, sep);
				}
			}

			void	infix_traversal(Node *current, char sep) {
				if (current)
				{
					infix_traversal(current->left, sep);
					current->treat();
					infix_traversal(current->right, sep);
				}
			}

			void	suffix_traversal(Node *current, char sep) {
				if (current)
				{
					suffix_traversal(current->left, sep);
					suffix_traversal(current->right, sep);
					current->treat();
				}
			}

			void	

		};

	public:
		map();
		map(map const& base);
		~map();
		map&	operator= (map const& base);

//		pair<iterator,bool> insert (const value_type& val);
//		iterator insert (iterator position, const value_type& val);
//		template <class InputIterator>  void insert (InputIterator first, InputIterator last);

		Node	*btree_create_node(value_type data)
		{
			Node	*newNode;

			newNode = _alloc_node(1);
			if (newNode)
			{
				newNode->_data = data;
				newNode->_left = NULL;
				newNode->_right = NULL;
				newNode->_balance = 0;
			}
			return (newNode);
		}

		void	btree_insert_data(Node **root, void *item, int (*cmpf)
			(void *, void *))
		{
			Node	*newNode;

			newNode = btree_create_node(item);
			if (root)
			{
				if (*root)
				{
					if (cmpf(item, (*root)->item) < 0)
					{
						if ((*root)->left)
							btree_insert_data(&(*root)->left, item, cmpf);
						else
							(*root)->left = newNode;
					}
					else
					{
						if ((*root)->right)
							btree_insert_data(&(*root)->right, item, cmpf);
						else
							(*root)->right = newNode;
					}
				}
				else
					*root = newNode;
			}
		}

	private :
		Node			*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_comp;
	};
}

#endif