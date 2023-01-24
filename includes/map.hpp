#ifndef MAP_H
# define MAP_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "pair.hpp"
# include "vector.hpp"
# include "map_iterator.hpp"
# include "iterator.hpp"

class map_iterator;

namespace ft
{
	template<class Key, class T, class Compare = std::less<Key>,
	class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map {
		struct Node;
	public:
		typedef Key																					key_type;
		typedef T																					mapped_type;
		typedef pair<const key_type, mapped_type>													value_type;
		typedef Compare																				key_compare;
		typedef Allocator																			allocator_type;
		typedef typename allocator_type::reference													reference;
		typedef typename allocator_type::const_reference											const_reference;
		typedef typename allocator_type::pointer													pointer;
		typedef typename allocator_type::const_pointer												const_pointer;
		typedef typename allocator_type::size_type													size_type;
		typedef typename allocator_type::difference_type											difference_type;
		typedef typename Allocator:: template rebind<Node>::other									node_allocator;
		typedef typename node_allocator::pointer													node_pointer;
		typedef ft::wrapper_it<ft::map_iterator<key_type, mapped_type, Node, value_type> >			iterator;
		typedef ft::wrapper_it<ft::map_iterator<key_type, mapped_type, Node, const value_type> >	const_iterator;
		typedef ft::reverse_iterator<iterator>														reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>												const_reverse_iterator;

		class value_compare
			: public std::binary_function<value_type, value_type, bool>
		{
			friend class map;

		protected:
			key_compare	comp;

			value_compare(key_compare c) : comp(c) {};

		public:
			bool operator()(const value_type& lhs, const value_type& rhs) const { return comp(lhs.first, rhs.first); }
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
			void		set_data(value_type &data) { _data = data; }
			void		set_left(Node *left) { _left = left; }
			void		set_right(Node *right) { _right = right; }
			void		set_parent(Node *parent) { _parent = parent; }
			value_type	&data() { return _data; }
			Node		*left() { return _left; }
			Node		*right() { return _right; }
			Node		*parent() { return _parent; }
			int			&height() { return _height; }

			void update_height() {
	    		_height = 1 + std::max(left() ? left()->_height : 0,
				    right() ? right()->_height : 0);
			}
/*
			void update_n() {
				n = 1 + (left() ? left()->n : 0)
				+ (right() ? right()->n : 0);
			}
*/
			short imbalance() {
				return (right() ? right()->_height : 0)
					- (left() ? left()->_height : 0);
			}
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
		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size() {
			_root = new_node();
		}

		template <class InputIterator>
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size() {
			_root = new_node();
	
			for (; first != last; ++first)
				insert(*first);
		}

		map(const map& x)
			: _root(NULL), _alloc_node(x._alloc_node), _alloc_pair(x._alloc_pair), _key_comp(x._key_comp), _size(x._size)
		{
			_root = new_node();
			const_iterator	first = x.begin();
			const_iterator	last = x.end();

			for (; first != last; ++first)
				insert(*first);
		}

		~map() { delete_node(protoroot()); }
		map&	operator= (map const& rhs) {
			if (this == &rhs)
				return (*this);
			clear();
			const_iterator	first = rhs.begin();
			const_iterator	last = rhs.end();

			for (; first != last; ++first)
				insert(*first);
			return (*this);
		}
		Node	*protoroot() const { return (_root); }
		Node	*protoroot() { return (_root); }
		Node	*dummy() const { return (protoroot()); }
		Node	*dummy() { return (protoroot()); }
		Node	*root() const { return (_root->right()); }
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
				std::cout << ", height: " << node->height() << std::endl;
			}
			else
				std::cout << "NULL pointer" << std::endl;
		}

		void	print_tree()
		{
			std::cout << "-- Here's my AVL tree: --" << std::endl;
			level_order_traversal(root(), print_node);
			std::cout << "-- That's the end of my tree. Thanx for watching. --" << std::endl;
		}

		iterator	begin()	{
			if (root())
			{
				Node	*temp(root());

				iterator it(leftmost(temp));
				return (it);
			}
			iterator it(protoroot());
			return (it);
		}

		const_iterator	begin() const {
			if (root())
			{
				Node	*temp(root());

				const_iterator it(leftmost(temp));
				return (it);
			}
			const_iterator it(protoroot());
			return (it);
		}

		iterator	end() {
			if (root())
			{
				Node	*temp(root());
				
				iterator it(rightmost(temp));
				return (++it);
			}
			iterator it(protoroot());
			return (it);
		}

		const_iterator	end() const {
			if (root())
			{
				Node	*temp(root());
				
				const_iterator it(rightmost(temp));
				return (++it);
			}
			const_iterator it(protoroot());
			return (it);
		}

		reverse_iterator rbegin() { return (reverse_iterator(end())); }
		
		const_reverse_iterator rbegin() const { return (const_iterator(end())); }

		reverse_iterator rend() { return (reverse_iterator(begin())); }
		
		const_reverse_iterator rend() const { return (const_iterator(begin())); } 

		iterator	find(const key_type& key) {
			Node *found = _recursive_avl_tree_search(root(), key);
			if (found)
				return (iterator(found));
			return (end());
		}
		const_iterator	find (const key_type& key) const {
			Node *found = _recursive_avl_tree_search(root(), key);
			if (found)
				return (const_iterator(found));
			return (end());
		}

		iterator		upper_bound(const key_type& key) {
			iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (_key_comp(key, it->first))
					break ;
				it++;
			}
			return (it);
		}

		const_iterator	upper_bound(const key_type& key) const {
			const_iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (_key_comp(key, it->first))
					break ;
				it++;
			}
			return (it);
		}

		iterator		lower_bound(const key_type& key) {
			iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (!_key_comp(it->first, key))
					break ;
				it++;
			}
			return (it);
		}

		const_iterator	lower_bound(const key_type& key) const {
			const_iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (!_key_comp(it->first, key))
					break ;
				it++;
			}
			return (it);
		}

		ft::pair<iterator,iterator>	equal_range(const key_type& key) {
			return (ft::make_pair(lower_bound(key), upper_bound(key)));
		}

		ft::pair<const_iterator,const_iterator>	equal_range(const key_type& key) const {
			const_iterator	it1(lower_bound(key));
			const_iterator	it2(upper_bound(key));

			return (ft::make_pair(it1, it2));
		}

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

		/****************************************************************************\
		**																			**
		**						Utility functions for AVL trees						**
		**																			**
		\****************************************************************************/

		Node	*leftmost(Node *_x) const
		{
			while (_x->left())
				_x = _x->left();
			return (_x);
		}

		Node	*rightmost(Node *_x) const
		{
			while (_x->right())
				_x = _x->right();
			return (_x);
		}

		Node	*_iterative_avl_tree_search(const key_type& k) const
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

		Node	*_recursive_avl_tree_search(Node *node, const key_type& k) const
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

		void	_avl_tree_transplant(Node *u, Node *v)
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

		Node	*_avl_tree_successor(Node *node)
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

		iterator erase_node(iterator it) {
			iterator itn(it);
			++itn;
			node *ptr = it.ptr;
			node *q;
			if (!ptr->left_child || !ptr->right_child) {
				q = ptr;
			} else {
				q = itn.ptr;
			}
			node *s;
			if (q->left_child) {
				s = q->left_child;
				q->left_child = nullptr;
			} else {
				s = q->right_child;
				q->right_child = nullptr;
			}
			if (s) {
				s->parent = q->parent;
			}
			if (q == q->parent->left_child) {
				q->parent->left_child = s;
			} else {
				q->parent->right_child = s;
			}
			node *q_parent = q->parent;
			if (q != ptr) {
				q->parent = ptr->parent;
				if (q->parent->left_child == ptr) {
				q->parent->left_child = q;
				} else {
				q->parent->right_child = q;
				}
				q->left_child = ptr->left_child;
				if (q->left_child)
				q->left_child->parent = q;
				q->right_child = ptr->right_child;
				if (q->right_child)
				q->right_child->parent = q;
				q->n = ptr->n;
				q->depth = ptr->depth;
				ptr->left_child = nullptr;
				ptr->right_child = nullptr;
			}
			if (q_parent == ptr) {
				q_parent = q;
			}
			node *parent;
			for (parent = q_parent; parent; parent = parent->parent) {
				--parent->n;
			}
			for (parent = q_parent; parent; parent = parent->parent) {
				parent->update_depth();
				if (parent == root)
				break;
				if (parent->imbalance() < -1) {
				// check for double-rotation case
				if (parent->left_child->imbalance() > 0) {
					rotate_left(parent->left_child);
				}
				rotate_right(parent);
				break;
				} else if (parent->imbalance() > 1) {
				// check for double-rotation case
				if (parent->right_child->imbalance() < 0) {
					rotate_right(parent->right_child);
				}
				rotate_left(parent);
				break;
				}
			}
			alloc.destroy(ptr);
			alloc.deallocate(ptr, 1);
			return itn;
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
				std::cout << "key " << k << std::endl;
				parent = p->parent();
				if (parent->left() == p)
					parent->set_left(NULL);
				else
					parent->set_right(NULL);
				delete_node(p);

				set_root(balance_tree(root()));

				suffix_traversal(root(), print_node);
				return ;
			}
			if (p->left() == NULL)
			{
				std::cout << "key " << k << std::endl;
				parent = p->parent();
				if (parent->left() == p)
					parent->set_left(p->right());
				else
					parent->set_right(p->right());
				delete_node(p);

				set_root(balance_tree(root()));

				suffix_traversal(root(), print_node);
				return ;
			}
			if (p->right() == NULL)
			{
				std::cout << "key " << k << std::endl;
				parent = p->parent();
				if (parent->left() == p)
					parent->set_left(p->left());
				else
					parent->set_right(p->left());
				delete_node(p);

				set_root(balance_tree(root()));

				suffix_traversal(root(), print_node);
				return ;
			}
			succ = _avl_tree_successor(p);
			if (p->right() != succ)
			{
				_avl_tree_transplant(succ, succ->right());
				succ->set_right(p->right());
				succ->right()->set_parent(succ);
			}
			std::cout << "key " << k << std::endl;
			_avl_tree_transplant(p, succ);
			succ->set_left(p->left());
			succ->left()->set_parent(succ);
			delete_node(p);
			recomp_height(succ);

			set_root(balance_tree(root()));

			suffix_traversal(root(), print_node);
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

		Node	*balance_tree(Node *root)
		{
			if (root == NULL)
				return (NULL);
			root->set_left(balance_tree(root->left()));
			root->set_right(balance_tree(root->right()));
			root = balance(root);
			return (root);
		}

		static void	recomp_height(Node *_x)
		{
			while (_x != NULL)
			{
				_x->_height = calculate_height(_x);
				_x = _x->parent();
			}
		}

		Node	*_avl_tree_insert(Node *_x, const value_type& val)
		{
			Node	*newNode = new_node(val);
			Node	*_y;

			if (!_x)
			{
				_x = newNode;
				set_root(_x);
				_x->set_parent(NULL);
				++_size;
				return (_x);
			}
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
			set_root(balance_tree(root()));
			suffix_traversal(root(), recomp_height);
			_y = _recursive_avl_tree_search(root(), val.first);
			return (_y);
		}

		void erase( iterator pos )
		{
			erase(pos->first);
		}

		void erase( iterator first, iterator last )
		{
			for (; first != last; ++first)
				erase(first);
		}

		size_type	erase( const key_type& key )
		{
			Node *p = _recursive_avl_tree_search(root(), key);
			if (!p)
				return (0);
			_avl_tree_node_deletion(key);
			return (1);
		}

		void	clear() {
			erase(begin(), end());
		}

		pair<iterator, bool>	insert(const value_type& val)
		{
			Node	*newNode = new_node(val);
			Node	*_x(root());
			Node	*_y;

			if (!_x)
			{
				_x = newNode;
				set_root(_x);
				_x->set_parent(NULL);
				++_size;
				return (ft::make_pair(iterator(_x), true));
			}
			while (_x != NULL) {
				_y = _x;
				if (_key_comp(val.first, _x->data().first))
					_x = _x->left();
				else if (val.first == _x->data().first)
				{
					delete_node(newNode);
					return (ft::make_pair(iterator(_x), false));
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
			set_root(balance_tree(root()));
			suffix_traversal(root(), recomp_height);
			_y = _recursive_avl_tree_search(root(), val.first);
			return (ft::make_pair(iterator(_y), true));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			(void)pos;
			Node *temp = _avl_tree_insert(root(), val);

			return (iterator(temp));
		}

		template<class InputIt>
		void	insert(InputIt first, InputIt last,
			typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::value* = 0)
		{
			for (; first != last; ++first)
				insert(*first);
		}

		mapped_type&	operator[] (const key_type& k)
		{
			return ((*(insert(ft::make_pair(k, mapped_type())).first)).second);
		}

		mapped_type&	at(const key_type& k)
		{
			Node *found = _recursive_avl_tree_search(root(), k);
			if (found)
				return (found->data().second);
			throw(std::out_of_range("map"));
		}

		const mapped_type&	at(const key_type& k) const
		{
			return (const_cast<mapped_type>(at((k))));
		}

		allocator_type	get_allocator() const { return _alloc_pair; }

		key_compare	key_comp() const { return _key_comp; };

		value_compare	value_comp() const { return value_compare(_key_comp); }

		bool empty() const { return !size(); }

		size_type	size() const { return _size; }

		size_type	max_size() const { return std::min<size_type>(_alloc_node.max_size(),
										std::numeric_limits<difference_type>::max()); }
		
		void	swap(map& other)
		{
			std::swap(_root, other._root);
			std::swap(_size, other._size);
			std::swap(_key_comp, other._key_comp);
			std::swap(_alloc_node, other._alloc_node);
			std::swap(_alloc_pair, other._alloc_pair);
		}

	private :
		Node			*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_key_comp;
		size_type		_size;
	};

	template <class Key, class T, class Compare, class Alloc>
	bool	operator==(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator!=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (!(rhs < lhs));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (rhs < lhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (!(lhs < rhs));
	}

	template <class Key, class T, class Compare, class Alloc>
	void	swap(map<Key, T, Compare, Alloc> &lhs, map<Key, T, Compare, Alloc> &rhs)
	{
		lhs.swap(rhs);
	}
}

#endif