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
#include <map>

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

		typedef ft::map_iterator<key_type, mapped_type, Node>		iterator;
//		typedef implementation-defined								const_iterator;
//		typedef std::reverse_iterator<iterator>						reverse_iterator;
//		typedef std::reverse_iterator<const_iterator>				const_reverse_iterator;

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
			int			_height;

		public:
			Node() : _data(value_type()), _left(NULL), _right(NULL), _parent(NULL), _balance() {}
			Node(value_type pair) : _data(pair), _left(NULL), _right(NULL), _parent(NULL), _balance() {}
			~Node() {}

			Node		&operator= (Node &other) { _left = other._left; _right = other._right; _parent = other._parent; _balance = other._balance; return (*this); }
			void		set_data(value_type &data) { _data = data; }
			void		set_left(Node *left) { _left = left; }
			void		set_right(Node *right) { _right = right; }
			void		set_parent(Node *parent) { _parent = parent; }
			value_type	&data() { return _data; }
			Node		*left() { return _left; }
			Node		*right() { return _right; }
			Node		*parent() { return _parent; }
			int			&balance() { return _balance; }
			int			&height() { return _height; }
			void		treat(char sep) { std::cout << data().first << sep << data().second << sep << std::endl; }
		};

	public:
		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size() {
				_root = new_node();
			}

		template <class InputIterator>
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
		map(const map& x);

		~map() { delete_node(_root); }
		map&	operator= (map const& base);
		Node	*root() { return _root; }
		void	change_root(Node *current) { _root = current; }

		void	prefix_traversal(Node *current, char sep) {
			if (current)
			{
				current->treat(sep);
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

		iterator	begin()	{
			iterator	it(_root);

			it.leftmost();
			return (it);
		}

		iterator	end() { return iterator(_root); }

		Node	*new_node(const value_type& pair = value_type())
		{
			Node *ptr = _alloc_node.allocate(1);
//			_alloc_node.construct(ptr);
			try
			{
				_alloc_pair.construct(&ptr->_data, pair);
			}
			catch (...)
			{
				_alloc_node.deallocate(ptr, 1);
				throw;
			}
			ptr->_left = NULL;
			ptr->_right = NULL;
			ptr->_parent = NULL;
			ptr->_balance = 0;
			ptr->_height = 1;
			return (ptr);
		}

		void	delete_node(Node *node)
		{
			_alloc_node.destroy(node);
			_alloc_node.deallocate(node, 1);
		}

		int	get_height(Node *node)
		{
			if (node)
				return (node->height());
			return (0);
		}

		Node *right_rotation(Node *_y)
		{
			Node *_x = _y->left();
			Node *_t2 = _x->right();
		
			// Perform rotation
			_x->_right = _y;
			_y->_left = _t2;
		
			// Update heights
			_y->_height = std::max(get_height(_y->left()),
							get_height(_y->right())) + 1;
			_x->_height = std::max(get_height(_x->left()),
							get_height(_x->right())) + 1;
		
			// Return new root
			return _x;
		}
		
		// A utility function to left
		// rotate subtree rooted with x
		// See the diagram given above.
		Node *left_rotation(Node *_x)
		{
			Node *_y = _x->right();
			Node *_t2 = _y->left();
		
			// Perform rotation
			_y->_left = _x;
			_x->_right = _t2;
		
			// Update heights
			_x->_height = std::max(get_height(_x->left()),   
							get_height(_x->right())) + 1;
			_y->_height = std::max(get_height(_y->left()),
							get_height(_y->right())) + 1;
		
			// Return new root
			return _y;
		}
		
		// Get Balance factor of node N
		int get_balance(Node *node)
		{
			if (node == NULL)
				return 0;
			return get_height(node->left()) - get_height(node->right());
		}

		pair<iterator, bool>	insert(const value_type& val)
		{
			Node		*newNode = new_node(val);
			iterator	_x(_root);
			iterator	_y;

			while (_x != NULL) {
				_y.set_ptr(_x.base());
				if (_key_comp(val.first, _x.base()->data().first))
					_x.set_ptr(_x.base()->left());
				else if (val.first == _x.base()->data().first)
				{
					delete_node(newNode);
					return (ft::make_pair(_x, false));
				}
				else
					_x.set_ptr(_x.base()->right());
			}
			if (_y == NULL)
			{
				_y.set_ptr(newNode);
				newNode->set_parent(_y.base()->parent());
			}
			else if (_key_comp(val.first, _y.base()->data().first))
			{
				_y.base()->set_left(newNode);
				newNode->set_parent(_y.base());
			}
			else
			{
				_y.base()->set_right(newNode);
				newNode->set_parent(_y.base());
			}
			std::cout << "1 " << _y.base()->height() << std::endl;
			_y.base()->_height = 1 + std::max(get_height(_y.base()->left()), get_height(_y.base()->right()));
			int	balance = get_balance(_y.base());
			if (balance > 1 && _key_comp(val.first, _y.base()->left()->data().first))
				return (ft::make_pair(iterator(right_rotation(_y.base())), true));
			if (balance < -1 && !_key_comp(val.first, _y.base()->right()->data().first))
				return (ft::make_pair(iterator(left_rotation(_y.base())), true));
			if (balance > 1 && !_key_comp(val.first, _y.base()->left()->data().first))
			{
				_y.base()->set_left(left_rotation(_y.base()->left()));
				return (ft::make_pair(iterator(right_rotation(_y.base())), true));
			}
			if (balance < -1 && _key_comp(val.first, _y.base()->right()->data().first))
			{
				_y.base()->set_right(right_rotation(_y.base()->right()));
				return (ft::make_pair(iterator(left_rotation(_y.base())), true));
			}
			std::cout << "2 " << _y.base()->height() << std::endl;
			++_size;
			return (ft::make_pair(_y, true));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			if (pos >= begin() && pos <= end())
			{
				Node		*newNode = new_node(val);
				iterator	_x(pos);
				iterator	_y;

				while (_x != NULL) {
					_y.set_ptr(_x.base());
					if (_key_comp(val.first, _x.base()->data().first))
						_x.set_ptr(_x.base()->left());
					else if (val.first == _x.base()->data().first)
					{
						delete_node(newNode);
						return (_x);
					}
					else
						_x.set_ptr(_x.base()->right());
				}
				if (_y == NULL)
				{
					_y.set_ptr(newNode);
					newNode->set_parent(_y.base()->parent());
				}
				else if (_key_comp(val.first, _y.base()->data().first))
				{
					_y.base()->set_left(newNode);
					newNode->set_parent(_y.base());
				}
				else
				{
					_y.base()->set_right(newNode);
					newNode->set_parent(_y.base());
				}
				_y.base()->_height = 1 + std::max(get_height(_y.base()->left()), get_height(_y.base()->right()));
				int	balance = get_balance(_y.base());
				if (balance > 1 && _key_comp(val.first, _y.base()->left()->data().first))
					return right_rotation(_y.base());
				if (balance < -1 && !_key_comp(val.first, _y.base()->right()->data().first))
					return left_rotation(_y.base());
				if (balance > 1 && !_key_comp(val.first, _y.base()->left()->data().first))
				{
					_y.base()->set_left(left_rotation(_y.base()->left()));
					return (iterator(right_rotation(_y.base())));
				}
				if (balance < -1 && _key_comp(val.first, _y.base()->right()->data().first))
				{
					_y.base()->set_right(right_rotation(_y.base()->right()));
					return (iterator(left_rotation(_y.base())));
				}
				++_size;
				return (_y);
			}
			else
				throw(ContainerException("out_of_range"));
		}
/*
		Node*	insert(Node* node, int key)
		{
			if (node == NULL)
				return(new_node(key));
		
			if (key < node->key)
				node->left = insert(node->left, key);
			else if (key > node->key)
				node->right = insert(node->right, key);
			else // Equal keys are not allowed in BST
				return node;
		
			node->_height = 1 + std::max(get_height(node->left),
								get_height(node->right));
		
			int balance = get_balance(node);
		
			// If this node becomes unbalanced, then
			// there are 4 cases
		
			// Left Left Case
			if (balance > 1 && key < node->left->key)
				return right_rotation(node);
		
			// Right Right Case
			if (balance < -1 && key > node->right->key)
				return left_rotation(node);
		
			// Left Right Case
			if (balance > 1 && key > node->left->key)
			{
				node->left = left_rotation(node->left);
				return right_rotation(node);
			}
		
			// Right Left Case
			if (balance < -1 && key < node->right->key)
			{
				node->right = right_rotation(node->right);
				return left_rotation(node);
			}
		
			return node;
		}
*/
		template<class InputIterator> void	insert(InputIterator first, InputIterator last)
		{
			for (; first < last; ++first)
				insert(*first);
		}

//		mapped_type&	operator[] (const key_type& k) {

//		}

	private :
		Node			*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_key_comp;
		size_type		_size;
	};
}

#endif