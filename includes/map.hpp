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
			void		treat(char sep) { std::cout << data().first << sep << data().second << " height: " << height() << std::endl; }
		};

	public:
		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size() {
				_root = new_node();
			}

		template <class InputIterator>
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
		map(const map& x);

		~map() { delete_node(root()); }
		map&	operator= (map const& base);
		Node	*root() { return _root; }
		void	set_root(Node *current) { _root = current; }

		void	prefix_traversal(Node *current, char sep) {
			if (current)
			{
//				current->treat(sep);
				update_height(current);
				prefix_traversal(current->left(), sep);
				prefix_traversal(current->right(), sep);
			}
		}

		void	infix_traversal(Node *current, char sep) {
			if (current)
			{
				infix_traversal(current->left(), sep);
//				current->treat(sep);
				update_height(current);
				infix_traversal(current->right(), sep);
			}
		}

		void	suffix_traversal(Node *current, void (*f)(Node *)) {
			if (current)
			{
				suffix_traversal(current->left(), f);
				suffix_traversal(current->right(), f);
				f(current);
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
			iterator	it(root());

			it.leftmost();
			return (it);
		}

		iterator	end() { return iterator(root()); }

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
/*
		Node*	deleteNode(Node* root, const value_type& val)
		{
			
			// STEP 1: PERFORM STANDARD BST DELETE
			if (root == NULL)
				return root;
			// If the key to be deleted is smaller
			// than the root's key, then it lies
			// in left subtree
			if (_key_comp(val.first, root()->data().first))
				root->left = deleteNode(root->left, key);
			// If the key to be deleted is greater
			// than the root's key, then it lies
			// in right subtree
			else if(!_key_comp(val.first, root()->data().first))
				root->right() = deleteNode(root->right(), key);
			// if key is same as root's key, then
			// This is the node to be deleted
			else
			{
				// node with only one child or no child
				if( (root->left() == NULL) ||
				(root->right() == NULL) )
				{
				Node *temp = root->left() ? root->left() : root->right();
				// No child case
				if (temp == NULL)
				{
					temp = root;
					root = NULL;
				}
				else // One child case
				*root = *temp; // Copy the contents of
							// the non-empty child
				delete_node(temp);
				}
				else
				{
				// node with two children: Get the inorder
				// successor (smallest in the right subtree)
				Node* temp = minValueNode(root->right);
			
				// Copy the inorder successor's
				// data to this node
				root->key = temp->key;
			
				// Delete the inorder successor
				root->right() = deleteNode(root->right(), temp->key);
				}
			}
			// If the tree had only one node
			// then return
			if (root == NULL)
			return root;
			
			// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
			root->height = 1 + max(height(root->left), height(root->right));
			
			// STEP 3: GET THE BALANCE FACTOR OF
			// THIS NODE (to check whether this
			// node became unbalanced)
			int balance = getBalance(root);
			
			// If this node becomes unbalanced,
			// then there are 4 cases
			
			// Left Left Case
			if (balance > 1 &&
				getBalance(root->left) >= 0)
				return rightRotate(root);
			
			// Left Right Case
			if (balance > 1 &&
				getBalance(root->left) < 0)
			{
				root->left = leftRotate(root->left);
				return rightRotate(root);
			}
			
			// Right Right Case
			if (balance < -1 &&
				getBalance(root->right) <= 0)
				return leftRotate(root);
			// Right Left Case
			if (balance < -1 &&
				getBalance(root->right) > 0)
			{
				root->right = rightRotate(root->right);
				return leftRotate(root);
			}
			
			return root;
		}
*/
		void	delete_node(Node *node)
		{
			_alloc_node.destroy(node);
			_alloc_node.deallocate(node, 1);
		}


		Node	*_avl_tree_search(Node *node, const value_type& pair)
		{
			if (node == NULL)
				return (NULL);
			else if (node->data().first == pair.first)
				return (node);
			else if (_key_comp(node->data().first, pair.first))
			{
				Node *temp = _avl_tree_search(node->left(), pair);
				return (temp);
			}
			else {
				Node *temp = _avl_tree_search(node->right(), pair);
				return (temp);
			}
		}

		static void	update_height(Node *node)
		{
			if (node != NULL) {
				int val = 1;
				if (node->left() != NULL)
					val = node->left()->height() + 1;
				if (node->right() != NULL)
					val = std::max(val, node->right()->height() + 1);
				node->_height = val;
			}
		}

		static int	calculate_height(Node *temp)
		{
			int h = 0;
			if (temp != NULL) {
				int l_height = calculate_height(temp->left());
				int r_height = calculate_height(temp->right());
				int max_height = std::max(l_height, r_height);
				h = max_height + 1;
			}
			return h;
		}

		int	diff(Node *temp)
		{
			int l_height = calculate_height(temp->left());
			int r_height = calculate_height(temp->right());
			int b_factor = l_height - r_height;
			return (b_factor);
		}


		Node	*rr_rotation(Node *parent)
		{
			Node *temp(parent->right());
			parent->set_right(temp->left());
			temp->set_left(parent);
			return (temp);
		}

		Node	*ll_rotation(Node *parent)
		{
			Node *temp(parent->left());
			parent->set_left(temp->right());
			temp->set_right(parent);
			return (temp);
		}

		Node	*lr_rotation(Node *parent)
		{
			Node *temp(parent->left());
			parent->set_left(rr_rotation(temp));
			return (ll_rotation(parent));
		}

		Node	*rl_rotation(Node *parent)
		{
			Node *temp(parent->right());
			parent->set_right(ll_rotation(temp));
			return (rr_rotation(parent));
		}

		Node	*balance(Node *temp)
		{
			int bal_factor = diff(temp);
			if (bal_factor > 1) {
				if (diff(temp->left()) > 0)
					temp = ll_rotation(temp);
				else
					temp = lr_rotation(temp);
			}
			else if (bal_factor < -1) {
				if (diff(temp->right()) > 0)
					temp = rl_rotation(temp);
				else
					temp = rr_rotation(temp);
			}
			return (temp);
		}

		Node	*balance_tree(Node *root)
		{
			if (root == NULL)
				return (NULL);
			root->set_left(balance_tree(root->left()));
			root->set_right(balance_tree(root->right()));
			root = balance(root);
			return (root);
		}

		Node	*_avl_tree_insert(Node *_x, const value_type& val)
		{
			Node	*newNode = new_node(val);
			Node	*_y;

			while (_x != NULL) {
				_y = _x;
				if (_key_comp(val.first, _x->data().first))
					_x = _x->left();
				else if (val.first == _x->data().first)
				{
					delete_node(newNode);
					return (_x);
				}
				else
					_x = _x->right();
			}
			if (_y == NULL)
			{
				_y = newNode;
				newNode->set_parent(_y->parent());
			}
			else if (_key_comp(val.first, _y->data().first))
			{
				_y->set_left(newNode);
				newNode->set_parent(_y);
			}
			else
			{
				_y->set_right(newNode);
				newNode->set_parent(_y);
			}
			set_root(balance_tree(root()));
			suffix_traversal(root(), update_height);
			// find the node (the iterator?) _y by finding the key
			_y = _avl_tree_search(root(), val);
			return (_y);
		}

		pair<iterator, bool>	insert(const value_type& val)
		{
			iterator	it(insert(root(), val));

			if (it->second == val.second)
				return (make_pair(it, true));
			return (make_pair(it, false));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			if (pos >= begin() && pos <= end())
				return (iterator(_avl_tree_insert(pos.base(), val)));
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