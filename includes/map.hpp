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
			void		set_data(value_type &data) { _data = data; }
			void		set_left(Node *left) { _left = left; }
			void		set_right(Node *right) { _right = right; }
			void		set_parent(Node *parent) { _parent = parent; }
			value_type	&data() { return _data; }
			Node		*left() { return _left; }
			Node		*right() { return _right; }
			Node		*parent() { return _parent; }
			int			&height() { return _height; }
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
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
		map(const map& x);

		~map() { delete_node(protoroot()); }
		map&	operator= (map const& base);
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
			return (ptr);
		}

		void	delete_node(Node *node)
		{
			_alloc_node.destroy(node);
			_alloc_node.deallocate(node, 1);
		}

		/********************************************************************************\
		** 																				**
		** 						Utility functions for AVL trees							**
		** 																				**
		\********************************************************************************/

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
			_y = _recursive_avl_tree_search(root(), val.first);
			return (_y);
		}

		void	delete_tree_node(const key_type& k)
		{
//			return (iterator(_avl_tree_node_deletion(k)));
			_avl_tree_node_deletion(k);
		}

		pair<iterator, bool>	insert(const value_type& val)
		{
			Node *temp = _avl_tree_insert(root(), val);
			if (!root())
			{
				set_root(temp);
				temp->set_parent(NULL);
			}
			iterator it(temp);

			if (it->second == val.second)
				return (make_pair(it, true));
			return (make_pair(it, false));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			if (pos >= begin() && pos <= end())
			{
				Node *temp = _avl_tree_insert(pos.base(), val);
				if (!root())
					set_root(temp);
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
	};
}

#endif