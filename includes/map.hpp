#ifndef MAP_H
# define MAP_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "ContainerException.hpp"
# include "pair.hpp"
# include "vector.hpp"
# include "map_iterator.hpp"
//#include <map>

class map_iterator;

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>,
	class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map {
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
		public:
			value_type	_data;
			Node		*_left;
			Node		*_right;
			Node		*_parent;
			int			_balance;

		public:
			Node() : _data(value_type()), _left(NULL), _right(NULL), _parent(NULL), _balance() {}
			Node(value_type pair) : _data(pair), _left(NULL), _right(NULL), _parent(NULL), _balance() {}
			~Node() {}

			Node		&operator= (Node &other) { _left = other._left; _right = other._right; _parent = other._parent; _balance = other._balance; return (*this); }
			void		change_left(Node *left) { _left = left; }
			void		change_right(Node *right) { _right = right; }
			void		change_parent(Node *parent) { _parent = parent; }
			void		change_data(value_type &data) { _data = data; }
			Node		*left() { return _left; }
			Node		*right() { return _right; }
			Node		*parent() { return _parent; }
			value_type	data() { return _data; }
			void		treat(char sep) { std::cout << data().first << sep << data().second << sep << std::endl; }

			void		suppress_node()
			{
				std::cout << "seggy" << std::endl;
				if (left())
					left()->suppress_node();
				std::cout << "seggy" << std::endl;
				if (right())
					right()->suppress_node();
				std::cout << "seggy" << std::endl;
				delete (this);
			};
		};

	public:
		explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size() {
				Node *newNode = _alloc_node.allocate(1);
				ft::pair<const char, int> z = ft::make_pair('z', 66);
				_alloc_pair.construct(&newNode->_data, z);
				_root = newNode;
			}

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
				std::cout << "bla" << sep << current->data().first << std::endl;
//				current->treat(sep);
				prefix_traversal(current->left(), sep);
				prefix_traversal(current->right(), sep);
			}
		}

		void	infix_traversal(Node *current, char sep) {
			if (current)
			{
				infix_traversal(current->left(), sep);
				current->treat(sep);
				infix_traversal(current->right(), sep);
			}
		}

		void	suffix_traversal(Node *current, char sep) {
			if (current)
			{
				suffix_traversal(current->left(), sep);
				suffix_traversal(current->right(), sep);
				current->treat(sep);
			}
		}

		void	level_order_traversal(Node *current, char sep) {
			vector<Node *>	deck;

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

/*
		void printCurrentLevel(Node *root, int level, char sep)
		{
			if (root == NULL)
				return;
			if (level == 1)
				std::cout << root->data().second << sep << std::endl;
			else if (level > 1) {
				printCurrentLevel(root->left(), level - 1, sep);
				printCurrentLevel(root->right(), level - 1, sep);
			}
		}

		int height(Node *node)
		{
			if (node == NULL)
				return 0;
			else {
				int lheight = height(node->left());
				int rheight = height(node->right());

				if (lheight > rheight) {
					return (lheight + 1);
				}
				else {
					return (rheight + 1);
				}
			}
		}

		void	level_order_traversal(Node *root, char sep) {
			int h = height(root);
			int i;
			for (i = 1; i <= h; i++)
				printCurrentLevel(root, i, sep);
		}
*/
		void	insert(Node *current, value_type &pair) {
			Node *newNode = _alloc_node.allocate(1);
//			_alloc_node.construct(newNode);
			_alloc_pair.construct(&newNode->_data, pair);

			if (current)
			{
				if (_key_comp(pair.first, current->data().first))
				{
					if (current->left())
					{
						std::cout << "left " << current->data().first << std::endl;
						insert(current->left(), pair);
					}
					else
						current->change_left(newNode);
				}
				else
				{
					if (current->right())
					{
						std::cout << "right " << current->data().first << std::endl;
						insert(current->right(), pair);
					}
					else
						current->change_right(newNode);
				}
			}
			else
				current = newNode;
			std::cout << current->data().second << std::endl;
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

	mapped_type&	operator[] (const key_type& k) {

	}

	private :
		Node			*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_key_comp;
		size_type		_size;
//		node_pointer	_ptr;
	};
}

#endif