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
# include "_tree_node.hpp"
# include <map>

namespace ft
{
	template<class Key, class T, class Compare = std::less<Key>,
	class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map {
	public:
		typedef Key																key_type;
		typedef T																mapped_type;
		typedef pair<const key_type, mapped_type>								value_type;
		typedef Compare															key_compare;
		typedef Allocator														allocator_type;
		typedef	Node<value_type>												tree_node;
		typedef typename allocator_type::reference								reference;
		typedef typename allocator_type::const_reference						const_reference;
		typedef typename allocator_type::pointer								pointer;
		typedef typename allocator_type::const_pointer							const_pointer;
		typedef typename allocator_type::size_type								size_type;
		typedef typename allocator_type::difference_type						difference_type;
		typedef typename Allocator:: template rebind<tree_node>::other			node_allocator;
		typedef typename node_allocator::pointer								node_pointer;
		typedef ft::wrapper_it<ft::map_iterator<tree_node, value_type> >		iterator;
		typedef ft::wrapper_it<ft::map_iterator<tree_node, const value_type> >	const_iterator;
		typedef ft::reverse_iterator<iterator>									reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>							const_reverse_iterator;

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

		friend std::ostream	&operator<< (std::ostream &o, const tree_node &node) {
			o << "key: " << node._data.first;
			o << ", value: " << node._data.second;
			o << ", height: " << node._height << std::endl;
			return (o);
		}

	public:

		/************************************************************************\
		**							Member functions							**
		\************************************************************************/

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
			: _root(NULL), _alloc_node(x._alloc_node), _alloc_pair(x._alloc_pair), _key_comp(x._key_comp), _size(x._size) {
			_root = new_node();
			const_iterator	first = x.begin();
			const_iterator	last = x.end();

			for (; first != last; ++first)
				insert(first.first);
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

		allocator_type	get_allocator() const { return _alloc_pair; }

		tree_node	*protoroot() const { return (_root); }
		tree_node	*protoroot() { return (_root); }
		tree_node	*dummy() const { return (protoroot()); }
		tree_node	*dummy() { return (protoroot()); }
		tree_node	*root() const { return (_root->_right); }
		tree_node	*root() { return (_root->_right); }
		void		set_root(tree_node *current) { _root->_right = current; }

		void	prefix_traversal(tree_node *current, char sep) {
			if (current)
			{
				current->treat(sep);
				prefix_traversal(current->_left, sep);
				prefix_traversal(current->_right, sep);
			}
		}

		void	infix_traversal(tree_node *current, char sep) {
			if (current)
			{
				infix_traversal(current->_left, sep);
				current->treat(sep);
				infix_traversal(current->_right, sep);
			}
		}

		void	suffix_traversal(tree_node *current, void (*f)(tree_node *)) {
			if (current)
			{
				suffix_traversal(current->_left, f);
				suffix_traversal(current->_right, f);
				f(current);
			}
		}

		void	level_order_traversal(tree_node *current, void (*f)(tree_node *)) {
			vector<tree_node *>	deck;

			deck.push_back(current);
			while (!deck.empty())
			{
				current = deck.front();
				deck.pop_front();
				f(current);
				if (current->_left)
					deck.push_back(current->_left);
				if (current->_right)
					deck.push_back(current->_right);
			}
		}

		static void	print_node(tree_node *node)
		{
			if (node)
			{
				std::cout << "key: " << node->_data.first;
				std::cout << ", value: " << node->_data.second;
				std::cout << ", height: " << node->_height << std::endl;
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

		/************************************************************************\
		**								Element access							**
		\************************************************************************/

		mapped_type&	at(const key_type& k)
		{
			tree_node *found = _recursive_avl_tree_search(root(), k);
			if (found)
				return (found->_data.second);
			throw(std::out_of_range("map"));
		}

		const mapped_type&	at(const key_type& k) const
		{
			return (const_cast<mapped_type>(at((k))));
		}

		mapped_type&	operator[] (const key_type& k)
		{
			return ((*(insert(ft::make_pair(k, mapped_type())).first)).second);
		}

		/************************************************************************\
		**								Iterators								**
		\************************************************************************/

		iterator	begin()	{
			if (root())
			{
				tree_node	*temp(root());

				iterator it(leftmost(temp));
				return (it);
			}
			iterator it(protoroot());
			return (it);
		}

		const_iterator	begin() const {
			if (root())
			{
				tree_node	*temp(root());

				const_iterator it(leftmost(temp));
				return (it);
			}
			const_iterator it(protoroot());
			return (it);
		}

		iterator	end() {
			if (root())
			{
				tree_node	*temp(root());
				
				iterator it(rightmost(temp));
				return (++it);
			}
			iterator it(protoroot());
			return (it);
		}

		const_iterator	end() const {
			if (root())
			{
				tree_node	*temp(root());
				
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

		/************************************************************************\
		**								Capacity								**
		\************************************************************************/

		bool empty() const { return (!size()); }

		size_type	size() const { return _size; }

		size_type	max_size() const { return std::min<size_type>(_alloc_node.max_size(),
									std::numeric_limits<difference_type>::max()); }

		/************************************************************************\
		**								Modifiers								**
		\************************************************************************/

		void	clear() {
			erase(begin(), end());
		}

		pair<iterator, bool>	insert(const value_type& val)
		{
			tree_node	*newNode = new_node(val);
			tree_node	*_x(root());
			tree_node	*_y;

			if (!_x)
			{
				_x = newNode;
				set_root(_x);
				_x->_parent = NULL;
				++_size;
				return (ft::make_pair(iterator(_x), true));
			}
			while (_x != NULL) {
				_y = _x;
				if (_key_comp(val.first, _x->_data.first))
					_x = _x->_left;
				else if (val.first == _x->_data.first)
				{
					delete_node(newNode);
					return (ft::make_pair(iterator(_x), false));
				}
				else
					_x = _x->_right;
			}
			if (_y == NULL)
			{
				_y = newNode;
				newNode->_parent = _y->_parent;
			}
			else if (_key_comp(val.first, _y->_data.first))
			{
				_y->_left = newNode;
				newNode->_parent = _y;
			}
			else
			{
				_y->_right = newNode;
				newNode->_parent = _y;
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
			tree_node *temp = _avl_tree_insert(root(), val);

			return (iterator(temp));
		}

		template<class InputIt>
		void	insert(InputIt first, InputIt last,
			typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::value* = 0)
		{
			for (; first != last; ++first)
				insert(*first);
		}

		void	erase(iterator pos)
		{
			std::cout << "function 3" << std::endl;
			erase(pos->first);
		}

		void	erase(iterator first, iterator last)
		{
			std::cout << "function 2" << std::endl;
			for (; first != last; ++first)
				erase(first->first);
		}

		void	erase(const_iterator first, const_iterator last)
		{
			std::cout << "function 2b" << std::endl;
			for (; first != last; ++first)
				erase(first->first);
		}

		size_type	erase(const key_type& key)
		{
			std::cout << "function 1" << std::endl;
			tree_node *p = _recursive_avl_tree_search(root(), key);
			if (!p)
				return (0);
			_avl_tree_node_deletion(key);
			return (1);
		}

		void	swap(map& other)
		{
			std::swap(_root, other._root);
			std::swap(_size, other._size);
			std::swap(_key_comp, other._key_comp);
			std::swap(_alloc_node, other._alloc_node);
			std::swap(_alloc_pair, other._alloc_pair);
		}

		/************************************************************************\
		**								Lookup									**
		\************************************************************************/

		size_type	count( const key_type& key ) const {
			tree_node	*found(_recursive_avl_tree_search(root(), key));

			if (found)
				return (1);
			return (0);
		}

		iterator	find(const key_type& key) {
			tree_node *found = _recursive_avl_tree_search(root(), key);
			if (found)
				return (iterator(found));
			return (end());
		}

		const_iterator	find (const key_type& key) const {
			tree_node *found = _recursive_avl_tree_search(root(), key);
			if (found)
				return (const_iterator(found));
			return (end());
		}

		ft::pair<iterator,iterator>	equal_range(const key_type& key) {
			return (ft::make_pair(lower_bound(key), upper_bound(key)));
		}

		ft::pair<const_iterator,const_iterator>	equal_range(const key_type& key) const {
			return (ft::make_pair(lower_bound(key), upper_bound(key)));
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

		/****************************************************************************\
		**																			**
		**						Utility functions for AVL trees						**
		**																			**
		\****************************************************************************/

		tree_node	*new_node(const value_type& pair = value_type(), tree_node *parent = NULL)
		{
			tree_node *ptr = _alloc_node.allocate(1);
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

		void	delete_node(tree_node *node)
		{
			_alloc_node.destroy(node);
			_alloc_node.deallocate(node, 1);
		}

		tree_node	*leftmost(tree_node *_x) const
		{
			while (_x->_left)
				_x = _x->_left;
			return (_x);
		}

		tree_node	*rightmost(tree_node *_x) const
		{
			while (_x->_right)
				_x = _x->_right;
			return (_x);
		}

		tree_node	*_iterative_avl_tree_search(const key_type& k) const
		{
			tree_node	*current(root());
			tree_node	*prev(root());

			while (current)
			{
				if (k == current->_data.first)
					return (current);
				else if (_key_comp(k, current->_data.first))
				{
					prev = current;
					current = current->_left;
				}
				else
				{
					prev = current;
					current = current->_right;
				}
			}
			return (prev);
		}

		tree_node	*_recursive_avl_tree_search(tree_node *node, const key_type& k) const
		{
			if (node == NULL)
				return (NULL);
			else if (node->_data.first == k)
				return (node);
			else if (_key_comp(k, node->_data.first))
			{
				tree_node *temp = _recursive_avl_tree_search(node->_left, k);
				return (temp);
			}
			else
			{
				tree_node *temp = _recursive_avl_tree_search(node->_right, k);
				return (temp);
			}
		}

		void	_avl_tree_transplant(tree_node *u, tree_node *v)
		{
			if (u->_parent == NULL)
				set_root(v);
			else if (u->_parent->_left == u)
				u->_parent->_left = v;
			else
				u->_parent->_right = v;
			v->_parent = u->_parent;
		}

		tree_node	*_avl_tree_successor(tree_node *node)
		{
			if (!node || (!node->_left && !node->_right))
				return (NULL);
			if (node->_left)
			{
				node = node->_right;
				while (node->_left)
					node = node->_left;
				return (node);
			}
			while (node->_parent && node->_parent->_right == node)
				node = node->_parent;
			return (node->_parent);
		}

		void	_avl_tree_node_deletion(const key_type& k)
		{
			tree_node	*p(NULL);
			tree_node	*parent(NULL);
			tree_node	*succ(NULL);

			p = _recursive_avl_tree_search(root(), k);
			if (!p)
				return ;
			--_size;
			if (p->_left == NULL && p->_right == NULL)
			{
				std::cout << "key " << k << std::endl;
				parent = p->_parent;
				if (parent->_left == p)
					parent->_left = NULL;
				else
					parent->_right = NULL;
				delete_node(p);

				recomp_height(parent);
				set_root(balance_tree(root()));

				level_order_traversal(root(), print_node);
				return ;
			}
			else if (p->_left == NULL)
			{
				std::cout << "key destra " << k << std::endl;
				parent = p->_parent;
				if (parent->_left == p)
					parent->_left = p->_right;
				else
					parent->_right = p->_right;
				delete_node(p);

				recomp_height(parent);
				set_root(balance_tree(root()));

				level_order_traversal(root(), print_node);
				return ;
			}
			else if (p->_right == NULL)
			{
				std::cout << "key sinistra " << k << std::endl;
				parent = p->_parent;
				if (parent->_left == p)
					parent->_left = p->_left;
				else
					parent->_right = p->_left;
				delete_node(p);

				recomp_height(parent);
				set_root(balance_tree(root()));

				level_order_traversal(root(), print_node);
				return ;
			}
			succ = _avl_tree_successor(p);
			if (p->_right != succ)
			{
				_avl_tree_transplant(succ, succ->_right);
				succ->_right = p->_right;
				succ->_right->_parent = succ;
			}
			std::cout << "key sinistra et destra " << k << std::endl;
			_avl_tree_transplant(p, succ);

			succ->_left = p->_left;
			succ->_left->_parent = succ;
			delete_node(p);

			recomp_height(succ);
			set_root(balance_tree(root()));

			level_order_traversal(root(), print_node);
			return ;
		}

		tree_node	*_avl_tree_rr_rotation(tree_node *root)
		{
			tree_node *temp = root->_right;
			root->_right = temp->_left;
			if (temp->_left != NULL)
				temp->_left->_parent = root;
			temp->_left = root;
			temp->_parent = root->_parent;
			root->_parent = temp;
			if (temp->_parent != NULL
				&& _key_comp(root->_data.first, temp->_parent->_data.first)) {
				temp->_parent->_left = temp;
			}
			else {
				if (temp->_parent != NULL)
					temp->_parent->_right = temp;
			}
			root = temp;
			recomp_height(root->_left);
			recomp_height(root->_right);
			return (root);
		}
		
		tree_node	*_avl_tree_rl_rotation(tree_node *root)
		{
			root->_right = _avl_tree_ll_rotation(root->_right);
			return (_avl_tree_rr_rotation(root));
		}

		tree_node	*_avl_tree_lr_rotation(tree_node *root)
		{
			root->_left = _avl_tree_rr_rotation(root->_left);
			return (_avl_tree_ll_rotation(root));
		}

		tree_node	*_avl_tree_ll_rotation(tree_node *root)
		{
			tree_node *temp = root->_left;
			root->_left = temp->_right;
			if (temp->_right != NULL)
				temp->_right->_parent = root;
			temp->_right = root;
			temp->_parent = root->_parent;
			root->_parent = temp;
			if (temp->_parent != NULL
				&& _key_comp(root->_data.first, temp->_parent->_data.first)) {
				temp->_parent->_left = temp;
			}
			else {
				if (temp->_parent != NULL)
					temp->_parent->_right = temp;
			}
			root = temp;
			recomp_height(root->_left);
			recomp_height(root->_right);
			return (root);
		}

		static int	calculate_height(tree_node *temp)
		{
			int h = 0;
			if (temp != NULL) {
				int l_height = calculate_height(temp->_left);
				int r_height = calculate_height(temp->_right);
				int max_height = std::max(l_height, r_height);
				h = max_height + 1;
			}
			return h;
		}

		int	diff(tree_node *temp)
		{
			int l_height = calculate_height(temp->_left);
			int r_height = calculate_height(temp->_right);
			int b_factor = l_height - r_height;
			return (b_factor);
		}

		tree_node	*balance(tree_node *temp)
		{
			int bal_factor = diff(temp);
			if (bal_factor > 1) {
				if (diff(temp->_left) > 0)
					temp = _avl_tree_ll_rotation(temp);
				else
					temp = _avl_tree_lr_rotation(temp);
			}
			else if (bal_factor < -1) {
				if (diff(temp->_right) > 0)
					temp = _avl_tree_rl_rotation(temp);
				else
					temp = _avl_tree_rr_rotation(temp);
			}

			return (temp);
		}

		tree_node	*balance_tree(tree_node *root)
		{
			if (root == NULL)
				return (NULL);
			root->_left = balance_tree(root->_left);
			root->_right = balance_tree(root->_right);
			root = balance(root);
			return (root);
		}

		static void	recomp_height(tree_node *_x)
		{
			while (_x != NULL)
			{
				_x->_height = calculate_height(_x);
				_x = _x->_parent;
			}
		}

		tree_node	*_avl_tree_insert(tree_node *_x, const value_type& val)
		{
			tree_node	*newNode = new_node(val);
			tree_node	*_y;

			if (!_x)
			{
				_x = newNode;
				set_root(_x);
				_x->_parent = NULL;
				++_size;
				return (_x);
			}
			while (_x != NULL) {
				_y = _x;
				if (_key_comp(val.first, _x->_data.first))
					_x = _x->_left;
				else if (val.first == _x->_data.first)
				{
					delete_node(newNode);
					return (_x);
				}
				else
					_x = _x->_right;
			}
			if (_y == NULL)
			{
				_y = newNode;
				newNode->_parent = _y->_parent;
			}
			else if (_key_comp(val.first, _y->_data.first))
			{
				_y->_left = newNode;
				newNode->_parent = _y;
			}
			else
			{
				_y->_right = newNode;
				newNode->_parent = _y;
			}
			++_size;
			set_root(balance_tree(root()));
			suffix_traversal(root(), recomp_height);
			_y = _recursive_avl_tree_search(root(), val.first);
			return (_y);
		}

		/************************************************************************\
		**								observers								**
		\************************************************************************/

		key_compare	key_comp() const { return _key_comp; };

		value_compare	value_comp() const { return value_compare(_key_comp); }


	private :
		tree_node		*_root;
		tree_node		*_end;
		tree_node		*_rend;
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