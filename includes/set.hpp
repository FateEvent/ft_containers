#ifndef SET
# define SET

# include <iostream>
# include <string>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "ContainerException.hpp"
# include "pair.hpp"
# include "vector.hpp"
# include "set_iterator.hpp"
#include <set>

class set_iterator;

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>,
	class Allocator = std::allocator<ft::pair<const Key, T> > >
	class set {
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

		typedef ft::set_iterator<key_type, mapped_type, Node>		iterator;
//		typedef implementation-defined								const_iterator;
//		typedef std::reverse_iterator<iterator>						reverse_iterator;
//		typedef std::reverse_iterator<const_iterator>				const_reverse_iterator;

		class value_compare
			: public std::binary_function<value_type, value_type, bool>
		{
			friend class set;

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
			std::string	_colour;

		public:
			Node() : _data(value_type()), _left(NULL), _right(NULL), _parent(NULL), _height(1), _colour("black") {}
			Node(value_type pair) : _data(pair), _left(NULL), _right(NULL), _parent(NULL), _height(1), _colour("black") {}
			~Node() {}

			Node		&operator= (Node &other) { _left = other._left; _right = other._right; _parent = other._parent; _colour = other._colour; return (*this); }
			void		set_data(value_type &data) { _data = data; }
			void		set_left(Node *left) { _left = left; }
			void		set_right(Node *right) { _right = right; }
			void		set_parent(Node *parent) { _parent = parent; }
			void		set_height(int height) { _height = height; }
			void		set_colour(std::string colour) { _colour = colour; }
			value_type	&data() { return _data; }
			Node		*left() { return _left; }
			Node		*right() { return _right; }
			Node		*parent() { return _parent; }
			int			&height() { return _height; }
			std::string	&colour() { return _colour; }
		};
/*
		std::ostream	&operator<< (std::ostream &o, const Node &node) {
			o << "key: " << node.data().first;
			o << ", value: " << node.data().second;
			o << ", height: " << node.height() << std::endl;
			return (o);
		}
*/
	public:
		explicit set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size(), _black_height() {
				_root = new_node();
		}

		template <class InputIterator>
		set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
		set(const set& x);

		~set() { delete_node(protoroot()); }
		set&	operator= (set const& base);
		Node	*protoroot() { return (_root); }
		Node	*root() { return (_root->right()); }
		void	set_root(Node *current) { _root->set_right(current); }

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

		void	suffix_traversal(Node *current, void (*f)(Node *)) {
			if (current)
			{
				suffix_traversal(current->left(), f);
				suffix_traversal(current->right(), f);
				f(current);
			}
		}

		void	level_order_traversal(Node *current, void (*f)(Node *)) {
			vector<Node *>	deck;

			deck.push_back(current);
			while (!deck.empty())
			{
				current = deck.front();
				deck.pop_front();
				f(current);
				if (current->left())
					deck.push_back(current->left());
				if (current->right())
					deck.push_back(current->right());
			}
		}

		static void	print_node(Node *node)
		{
			if (node)
			{
				std::cout << "key: " << node->data().first;
				std::cout << ", value: " << node->data().second;
				std::cout << ", colour: " << node->colour();
				std::cout << ", height: " << node->height() << std::endl;
			}
			else
				std::cout << "NULL pointer" << std::endl;
		}

		void	print_tree()
		{
			std::cout << "-- Here's my Red-Black tree: --" << std::endl;
			level_order_traversal(root(), print_node);
			std::cout << "-- Here my tree ends. Tschüss. --" << std::endl;
		}

		iterator	begin()	{
			if (root())
			{
				iterator it(root());

				it.leftmost();
				return (it);
			}
			iterator it(protoroot());
			return (it);
		}
//		const_iterator	begin() const;

		iterator	end() { return iterator(root() ? root() : protoroot()); }
//		const_iterator	end() const;

		iterator	find(const key_type& k)
		{
			Node *found = _recursive_avl_tree_search(root(), k);
			if (found)
				return (iterator(found));
			return (end());
		}
//		const_iterator	find (const key_type& k) const;

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
			ptr->_colour = "red";
			return (ptr);
		}

		void	delete_node(Node *node)
		{
			_alloc_node.destroy(node);
			_alloc_node.deallocate(node, 1);
		}

		/****************************************************************************\
		** 																			**
		** 					Utility functions for Red-Black trees					**
		** 																			**
		\****************************************************************************/

		Node	*_iterative_avl_tree_search(const key_type& k)
		{
			Node	*current(root());
			Node	*prev(root());

			while (current)
			{
				if (k == current->data().first)
					return (current);
				else if (_key_comp(k, current->data().first))
				{
					prev = current;
					current = current->left();
				}
				else
				{
					prev = current;
					current = current->right();
				}
			}
			return (prev);
		}

		Node	*_recursive_avl_tree_search(Node *node, const key_type& k)
		{
			if (node == NULL)
				return (NULL);
			else if (node->data().first == k)
				return (node);
			else if (_key_comp(k, node->data().first))
			{
				Node *temp = _recursive_avl_tree_search(node->left(), k);
				return (temp);
			}
			else
			{
				Node *temp = _recursive_avl_tree_search(node->right(), k);
				return (temp);
			}
		}

		void	transplant(Node *u, Node *v)
		{
			if (u == NULL)
				return ;
			else if (u->parent() == NULL)
				set_root(v);
			else if (u->parent()->left() == u)
				u->parent()->set_left(v);
			else
				u->parent()->set_right(v);
			if (v != NULL)
				v->set_parent(u->parent());
		}

		Node	*successor(Node *node)
		{
			if (node->right() != NULL)
			{
				node = node->right();
				while(node->left() != NULL)
					node = node->left();
				return (node);
			}
			else
			{
				while(node->parent() != NULL && node->parent()->right() == node)
					node = node->parent();
				return (node->parent());
			}
		}

		void	_avl_tree_node_deletion(const key_type& k)
		{
			Node	*p(NULL);
			Node	*parent(NULL);
			Node	*succ(NULL);

			p = _recursive_avl_tree_search(root(), k);
			if (!p)
				return ;
			--_size;
			if (p->left() == NULL && p->right() == NULL)
			{
				parent = p->parent();

				if (parent->left() == p)
					parent->set_left(NULL);
				else
					parent->set_right(NULL);
				delete_node(p);
				set_root(balance_tree(root()));
				return ;
			}
			if (p->left() == NULL)
			{
				parent = p->parent();
				if (parent->left() == p)
					parent->set_left(p->right());
				else
					parent->set_right(p->right());
				delete_node(p);
				set_root(balance_tree(root()));
				return ;
			}
			if (p->right() == NULL)
			{
				parent = p->parent();
				if (parent->left() == p)
					parent->set_left(p->left());
				else
					parent->set_right(p->left());
				delete_node(p);	
				set_root(balance_tree(root()));
				return ;
			}
			succ = successor(p);
			if (p->right() != succ)
			{
				transplant(succ, succ->right());
				succ->set_right(p->right());
				succ->right()->set_parent(succ);
			}
			transplant(p, succ);
			succ->set_left(p->left());
			succ->left()->set_parent(succ);
			delete_node(p);
			recomp_height(succ);
			set_root(balance_tree(root()));
			return ;
		}

		Node	*_avl_tree_rr_rotation(Node *root)
		{
			Node *temp = root->right();
			root->set_right(temp->left());
			if (temp->left() != NULL)
				temp->left()->set_parent(root);
			temp->set_left(root);
			temp->set_parent(root->parent());
			root->set_parent(temp);
			if (temp->parent() != NULL
				&& _key_comp(root->data().first, temp->parent()->data().first)) {
				temp->parent()->set_left(temp);
			}
			else {
				if (temp->parent() != NULL)
					temp->parent()->set_right(temp);
			}
			root = temp;
			recomp_height(root->left());
			recomp_height(root->right());
			return (root);
		}
		
		Node	*_avl_tree_rl_rotation(Node *root)
		{
			root->set_right(_avl_tree_ll_rotation(root->right()));
			return (_avl_tree_rr_rotation(root));
		}

		Node	*_avl_tree_lr_rotation(Node *root)
		{
			root->set_left(_avl_tree_rr_rotation(root->left()));
			return (_avl_tree_ll_rotation(root));
		}

		Node	*_avl_tree_ll_rotation(Node *root)
		{
			Node *temp = root->left();
			root->set_left(temp->right());
			if (temp->right() != NULL)
				temp->right()->set_parent(root);
			temp->set_right(root);
			temp->set_parent(root->parent());
			root->set_parent(temp);
			if (temp->parent() != NULL
				&& _key_comp(root->data().first, temp->parent()->data().first)) {
				temp->parent()->set_left(temp);
			}
			else {
				if (temp->parent() != NULL)
					temp->parent()->set_right(temp);
			}
			root = temp;
			recomp_height(root->left());
			recomp_height(root->right());
			return (root);
		}

		int	diff(Node *temp)
		{
			int l_height = calculate_height(temp->left());
			int r_height = calculate_height(temp->right());
			int b_factor = l_height - r_height;
			return (b_factor);
		}

		Node	*balance(Node *temp)
		{
			int bal_factor = diff(temp);
			if (bal_factor > 1) {
				if (diff(temp->left()) > 0)
					temp = _avl_tree_ll_rotation(temp);
				else
					temp = _avl_tree_lr_rotation(temp);
			}
			else if (bal_factor < -1) {
				if (diff(temp->right()) > 0)
					temp = _avl_tree_rl_rotation(temp);
				else
					temp = _avl_tree_rr_rotation(temp);
			}
			return (temp);
		}
/*
		void	_Rb_right_rotation(Node	*_x)
		{
			Node	*_y(NULL);
			_y = _x->left();
			// Turn _y's left sub-tree into _x's left sub-tree
			_x->set_left(_y->right());
			if ( _y->right() != NULL )
				_y->right()->set_parent(_x);
			// _y's new parent was _x's parent
			_y->set_parent(_x->parent());
			// Set the parent to point to _y instead of _x
			// First see whether we're at the root
			if ( _x->parent() == NULL )
				set_root(_y);
			else
				if ( _x == _x->parent()->right())
					// _x was on the right of its parent
					_x->parent()->set_right(_y);
				else
					// _x must have been on the left
					_x->parent()->set_left(_y);
			// Finall_y, put _x on _y's right
			_y->set_right(_x);
			_x->set_parent(_y);
		}

		void	_Rb_left_rotation(Node	*_x)
		{
			Node	*_y(NULL);
			_y = _x->right();
			// Turn _y's left sub-tree into _x's right sub-tree
			_x->set_right(_y->left());
			if ( _y->left() != NULL )
				_y->left()->set_parent(_x);
			// _y's new parent was _x's parent
			_y->set_parent(_x->parent());
			// Set the parent to point to _y instead of _x
			// First see whether we're at the root
			if ( _x->parent() == NULL )
				set_root(_y);
			else
				if ( _x == _x->parent()->left())
					// _x was on the left of its parent
					_x->parent()->set_left(_y);
				else
					// _x must have been on the right
					_x->parent()->set_right(_y);
			// Finall_y, put _x on _y's left
			_y->set_left(_x);
			_x->set_parent(_y);
		}

		void	recolouring(Node *_x)
		{
			Node	*_y(NULL);
			_x->set_colour("red");
			while (_x->parent() && _x->parent() != root() && _x != root() && _x->parent()->colour() == "red") {
				if ( _x->parent() == _x->parent()->parent()->left()) {
					// If _x's parent() is a left, _x is _x's right 'uncle'
					_y = _x->parent()->parent()->right();
					if ( _y->colour() == "red" ) {
						// case 1 - change the colours
						_x->parent()->set_colour("black");
						_y->set_colour("black");
						_x->parent()->parent()->set_colour("red");
						// Move _x up the tree
						_x = _x->parent()->parent();
					}
					else {
						// _y is a black node
						if ( _x == _x->parent()->right()) {
							// and _x is to the right 
							// case 2 - move _x up and rotate
							_x = _x->parent();
							_Rb_left_rotation(_x);
						}
						// case 3
						_x->parent()->set_colour("black");
						_x->parent()->parent()->set_colour("red");
						_Rb_right_rotation(_x->parent()->parent());
					}
				}
				else {
					// repeat the "if" part with right and left
					exchanged
					_y = _x->parent()->parent()->left();
					if ( _y->colour() == "red" ) {
						// case 1 - change the colours
						_x->parent()->set_colour("black");
						_y->set_colour("black");
						_x->parent()->parent()->set_colour("red");
						// Move _x up the tree
						_x = _x->parent()->parent();
					}
					else {
						// _x is a black node
						if ( _x == _x->parent()->left() ) {
							// and _x is to the right 
							// case 2 - move _x up and rotate
							_x = _x->parent();
							_Rb_right_rotation(_x);
							}
						// case 3
						_x->parent()->set_colour("black");
						_x->parent()->parent()->set_colour("red");
						_Rb_left_rotation(_x->parent()->parent());
					}
				}
			}
			// Colour the root black
			root()->set_colour("black");
		}

		Node	*_Rb_tree_recolouring(Node *_x)
		{
			Node	*uncle(NULL), *parent(NULL), *gp(NULL);

			while (_x != root() && _x->parent()->colour() == "red")
			{
				uncle = get_uncle_node(_x);
				parent = _x->parent();
				gp = parent->parent();
				if (!uncle || !gp)
					return (NULL);
				if (uncle->colour() == "black")
					break ;
				_x->parent()->set_colour("black");
				uncle->set_colour("black");
				gp->set_colour("red");
				_x = gp;
			}
			if (uncle->colour() == "red")
			{
				if (parent == gp->left() && _x == parent->left())
				{
					parent = _avl_tree_ll_rotation(parent);
					parent->set_colour("black");
					parent->left()->set_colour("red");
				}
				else if (parent == gp->left() && _x == parent->right())
				{
					parent = _avl_tree_lr_rotation(parent);
					parent->set_colour("black");
					parent->right()->set_colour("red");
				}
				else if (parent == gp->right() && _x == parent->right())
				{
					parent = _avl_tree_rr_rotation(parent);
					parent->set_colour("black");
					parent->right()->set_colour("red");
				}
				else if (parent == gp->right() && _x == parent->left())
				{
					parent->set_right(_avl_tree_rl_rotation(parent));
					parent->set_colour("black");
					parent->left()->set_colour("red");
				}
			}
			return (_x);
		}
*/
		Node	*balance_tree(Node *root)
		{
			if (root == NULL)
				return (NULL);
			root->set_left(balance_tree(root->left()));
			root->set_right(balance_tree(root->right()));
			root = _Rb_tree_recolouring(root);
			return (root);
		}

		void	swapColours(Node *_x, Node *_y) {
			std::string temp = _x->colour();
			_x->set_colour(_y->colour());
			_y->set_colour(temp);
		}

		bool	isOnLeft(Node *node) { return node == node->parent()->left(); }

		// returns pointer to sibling
		Node	*sibling(Node *node) {
				// sibling null if no parent
				if (node->parent() == NULL)
						return (NULL);
				if (isOnLeft(node))
						return node->parent()->right();
				return (node->parent()->left());
		}

		void	moveDown(Node *node, Node *parent) {
			if (node->parent() != NULL) {
				if (isOnLeft(node)) {
					node->parent()->set_left(parent);
				} else {
					node->parent()->set_right(parent);
				}
			}
			parent->set_parent(node->parent());
			node->set_parent(parent);
		}

		bool	hasRedChild(Node *node) {
			return (node->left() != NULL and node->left()->colour() == "red") or
			(node->right() != NULL and node->right()->colour() == "red");
		}

		Node	*get_uncle_node(Node *node)
		{
			if (node->parent() && node->parent()->parent())
			{
				if (node->parent() == node->parent()->parent()->left())
					return (node->parent()->parent()->right());
				else
					return (node->parent()->parent()->left());
			}
			return (NULL);
		}

		void	leftRotate(Node *_x) {
			// new parent will be node's right child
			Node *nParent = _x->right();
			// update root if current node is root
			if (_x == root())
				set_root(nParent);
			moveDown(_x, nParent);
			// connect _x with new parent's left element
			_x->set_right(nParent->left());
			// connect new parent's left element with node
			// if it is not null
			if (nParent->left() != NULL)
				nParent->left()->set_parent(_x);
			// connect new parent with _x
			nParent->set_left(_x);
		}

		void	rightRotate(Node *_x) {
			// new parent will be node's left child
			Node *nParent = _x->left();
			// update root if current node is root
			if (_x == root())
				set_root(nParent);
			moveDown(_x, nParent);
			// connect _x with new parent's right element
			_x->set_left(nParent->right());
			// connect new parent's right element with node
			// if it is not null
			if (nParent->right() != NULL)
				nParent->right()->set_parent(_x);
			// connect new parent with _x
			nParent->set_right(_x);
		}

		void	fixRedRed(Node *x) {
			// if x is root colour() it black and return
			if (x == root()) {
				x->set_colour("black");
				return ;
			}
			// initialize parent, grandparent, uncle
			Node	*parent = x->parent(), *grandparent = parent->parent(), *uncle = get_uncle_node(x);
			if (parent->colour() != "black") {
				if (uncle != NULL && uncle->colour() == "red") {
					// uncle "red", perform recolour()ing and recurse
					parent->set_colour("black");
					uncle->set_colour("black");
					grandparent->set_colour("red");
					fixRedRed(grandparent);
				} else {
					// Else perform LR, LL, RL, RR
					if (isOnLeft(parent)) {
						if (isOnLeft(x)) {
							// for left right
							swapColours(parent, grandparent);
						} else {
							leftRotate(parent);
							swapColours(x, grandparent);
						}
						// for left left and left right
						rightRotate(grandparent);
					} else {
						if (isOnLeft(x)) {
							// for right left
							rightRotate(parent);
							swapColours(x, grandparent);
						} else {
							swapColours(parent, grandparent);
						}
						// for right right and right left
						leftRotate(grandparent);
					}
				}
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

		static void	recomp_height(Node *_x)
		{
			while (_x != NULL)
			{
				_x->_height = calculate_height(_x);
				_x = _x->parent();
			}
		}



		Node	*_Rb_tree_insert(Node *_x, const value_type& val)
		{
			Node	*newNode = new_node(val);
//			Node	*_y;

			if (!_x)
			{
				_x = newNode;
				set_root(_x);
				_x->set_parent(NULL);
				_x->set_colour("black");
				++_size;
				++_black_height;
				return (_x);
			}
			else	// ajouté
			{
				Node *temp = _iterative_avl_tree_search(val.first);
				if (temp->data() == val)
				{
					delete_node(newNode);
					return (temp);
				}
				newNode->set_parent(temp);
				if (_key_comp(val.first, temp->data().first))
					temp->set_left(newNode);
				else
					temp->set_right(newNode);
				// fix red red voilaton if exists
				fixRedRed(newNode);
				// j'enlèverai après
				return (NULL);
			}
/*
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
			++_size;
			recolouring(_y);
//			set_root(balance_tree(root()));
			_y = _recursive_avl_tree_search(root(), val.first);
			return (_y);
*/
		}

		void	delete_tree_node(const key_type& k)
		{
//			return (iterator(_avl_tree_node_deletion(k)));
			_avl_tree_node_deletion(k);
		}

		pair<iterator, bool>	insert(const value_type& val)
		{
			Node *temp = _Rb_tree_insert(root(), val);
			iterator it(temp);

			if (it->second == val.second)
				return (make_pair(it, true));
			return (make_pair(it, false));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			if (pos >= begin() && pos <= end())
			{
				Node *temp = _Rb_tree_insert(pos.base(), val);
				return (iterator(temp));
			}
			else
				throw(ContainerException("out_of_range"));
		}

		template<class InputIterator> void	insert(InputIterator first, InputIterator last)
		{
			for (; first < last; ++first)
				insert(*first);
		}

		mapped_type&	operator[] (const key_type& k)
		{
			Node *found = _recursive_avl_tree_search(root(), k);
			if (found)
				return (found->data().second);
			iterator it = insert(root(), ft::make_pair(k, mapped_type()));
			return (it->second);
		}

		mapped_type&	at(const key_type& k)
		{
			Node *found = _recursive_avl_tree_search(root(), k);
			if (found)
				return (found->data().second);
			throw(ContainerException("out_of_range"));
		}

		const mapped_type&	at(const key_type& k) const
		{
			return (const_cast<mapped_type>(at((k))));
		}

	private :
		Node			*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_key_comp;
		size_type		_size;
		size_type		_black_height;
	};
}

#endif