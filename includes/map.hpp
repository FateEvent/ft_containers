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
			int			_height;

		public:
			Node() : _data(value_type()), _left(NULL), _right(NULL), _parent(NULL), _height(1) {}
			Node(value_type pair) : _data(pair), _left(NULL), _right(NULL), _parent(NULL), _height(1) {}
			~Node() {}

			Node		&operator= (Node &other) { _left = other._left; _right = other._right; _parent = other._parent; _height = other._height; return (*this); }
			void		set_data(const value_type &data) { _data = data; }
			void		set_left(Node *left) { _left = left; }
			void		set_right(Node *right) { _right = right; }
			void		set_parent(Node *parent) { _parent = parent; }
			value_type	&data() { return _data; }
			Node		*left() { return _left; }
			Node		*right() { return _right; }
			Node		*parent() { return _parent; }
			int			&height() { return _height; }
			void		treat(char sep) { std::cout << data().first << sep << data().second << sep << std::endl; }

			reference	operator* () const { return _data(); }
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

		Node	*new_node(const value_type& pair = value_type(), Node *parent = NULL)
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
			ptr->_parent = parent;
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
			std::cout << "dest" << std::endl;
			Node *_x = _y->left();
			Node *_t2 = _x->right();
			_x->set_right(_y);
			_y->set_left(_t2);
			_y->_height = std::max(get_height(_y->left()), get_height(_y->right())) + 1;
			_x->_height = std::max(get_height(_x->left()), get_height(_x->right())) + 1;
			return (_x);
		}

		Node *left_rotation(Node *_x)
		{
			std::cout << "sinist" << std::endl;
			Node *_y = _x->right();
			Node *_t2 = _y->left();
			_y->set_left(_x);
			_x->set_right(_t2);
			_x->_height = std::max(get_height(_x->left()), get_height(_x->right())) + 1;
			_y->_height = std::max(get_height(_y->left()), get_height(_y->right())) + 1;
			return (_y);
		}

		int get_balance(Node *node)
		{
			if (node == NULL)
				return 0;
			return (get_height(node->left()) - get_height(node->right()));
		}
/*
		pair<iterator, bool>	insert(const value_type& val)
		{
			iterator	_y(insert(begin(), val));

			_size = get_height(root());
			if (val.first == _y.base()->data().first && val.second == _y.base()->data().second)
				return (ft::make_pair(_y, true));
			return (ft::make_pair(_y, false));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			if (pos >= begin() && pos <= end())
			{
				Node		*node = _avl_tree_insert(pos.base(), pos.base()->parent(), val);
				iterator	_y(node);

				_size = get_height(root());
				return (_y);
			}
			else
				throw(ContainerException("out_of_range"));
		}

		void	update_height(Node *node)
		{
			if (node != NULL) {

				// Store the height of the
				// current node
				int val = 1;

				// Store the height of the left
				// and the right subtree
				if (node->left() != NULL)
					val = node->left()->height() + 1;

				if (node->right() != NULL)
					val = std::max(val, node->right()->height() + 1);

				// Update the height of the
				// current node
				node->_height = val;
			}
		}

		Node	*LLR(Node *node)
		{
			Node	*tmp = node->left();
			node->set_left(tmp->right());
			if (tmp->right() != NULL)
				tmp->right()->set_parent(node);
			tmp->set_right(node);
			tmp->set_parent(node->parent());
			node->set_parent(tmp);
			if (tmp->parent() != NULL
				&& _key_comp(node->data().first, tmp->parent()->data().first)) {
				tmp->parent()->set_left(tmp);
			}
			else {
				if (tmp->parent() != NULL)
					tmp->parent()->set_right(tmp);
			}
			node = tmp;
			update_height(node->left());
			update_height(node->right());
			update_height(node);
			update_height(node->parent());

			return node;
		}

		Node	*RRR(Node *node)
		{
			Node	*tmp = node->right();
			node->set_right(tmp->left());
			if (tmp->left() != NULL)
				tmp->left()->set_parent(node);
			tmp->set_left(node);
			tmp->set_parent(node->parent());
			node->set_parent(tmp);
			if (tmp->parent() != NULL
				&& _key_comp(node->data().first, tmp->parent()->data().first)) {
				tmp->parent()->set_left(tmp);
			}
			else {
				if (tmp->parent() != NULL)
					tmp->parent()->set_right(tmp);
			}
			node = tmp;
			update_height(node->left());
			update_height(node->right());
			update_height(node);
			update_height(node->parent());
			return node;
		}

		Node	*LRR(Node *node)
		{
			node->set_left(RRR(node->left()));
			return LLR(node);
		}

		Node	*RLR(Node *node)
		{
			node->set_right(LLR(node->right()));
			return RRR(node);
		}

		Node *_avl_tree_insert(Node *node, Node *parent, const value_type &val)
		{
			if (node == NULL)
				node = new_node(val, parent);
			else if (node->data().first > val.first) {
				node->set_left(_avl_tree_insert(node->left(), node, val));
				int firstheight = 0;
				int secondheight = 0;
				if (node->left() != NULL)
					firstheight = node->left()->height();
				if (node->right() != NULL)
					secondheight = node->right()->height();
				if (abs(firstheight - secondheight) == 2) {
					if (node->left() != NULL
						&& val.first < node->left()->data().first) {
						node = LLR(node);
					}
					else {
						node = LRR(node);
					}
				}
			}
			else if (node->data().first < val.first) {
				node->set_right(_avl_tree_insert(node->right(), node, val));
				int firstheight = 0;
				int secondheight = 0;
				if (node->left() != NULL)
					firstheight = node->left()->height();
				if (node->right() != NULL)
					secondheight = node->right()->height();
				if (abs(firstheight
						- secondheight)
					== 2) {
					if (node->right() != NULL
						&& _key_comp(val.first, node->right()->data().first)) {
						node = RLR(node);
					}
					else {
						node = RRR(node);
					}
				}
			}
			else {
			}
			update_height(node);
			return node;
		}

		bool AVLsearch(Node *node, const value_type& pair)
		{
			if (node == NULL)
				return false;
			else if (node->data().first == pair.first)
				return true;
			else if (node->data().first > pair.first) {
				bool val = AVLsearch(node->left(), pair.first);
				return val;
			}
			else {
				bool val = AVLsearch(node->right(), pair.first);
				return val;
			}
		}
*/

		pair<iterator,bool>	insert(const value_type& val)
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
				++_size;
				return (_y);
			}
			else
				throw(ContainerException("out_of_range"));
		}

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