#ifndef _AVL_TREE_H
# define _AVL_TREE_H

# include <algorithm>
# include "utilities.hpp"
# include "pair.hpp"
# include "_avl_tree_node.hpp"
# include "_Rb_iterator.hpp"
# include "iterator_traits.hpp"

namespace ft
{
	template <class T, class Compare = std::less<T>, class Allocator = std::allocator<T> >
	class AVLTree
	{
		public:
			typedef T														value_type;
			typedef Compare													value_compare;
			typedef Allocator												allocator_type;
			typedef size_t													size_type;
			typedef ptrdiff_t												difference_type;
			typedef typename allocator_type::reference						reference;
			typedef typename allocator_type::const_reference				const_reference;
			typedef typename allocator_type::pointer						pointer;
			typedef typename allocator_type::const_pointer					const_pointer;
			typedef RBIterator<value_type>									iterator;
			typedef RBIterator<const value_type>							const_iterator;
			typedef ft::reverse_iterator<iterator>							reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;
			typedef Node<value_type>										tree_node;
			typedef typename Allocator::template rebind<tree_node>::other	node_allocator;
		
		public:
			/****************************************************************************\
			**								Member functions							**
			\****************************************************************************/

			explicit AVLTree(const value_compare &comp, const allocator_type &alloc) : _comp(comp), _node_ptr(new_nil_leaf()), _alloc_pair(alloc), _size(0)
			{
				_root = _node_ptr;
				_node_ptr->height = 0;
			}

			AVLTree(const AVLTree &other) : _comp(other._comp), _node_ptr(new_nil_leaf()), _root(_node_ptr), _alloc_pair(other._alloc_pair), _size(0)
			{
				*this = other;
			}

			AVLTree &operator=(const AVLTree &other)
			{
				if (this != &other)
				{
					clear(_root);
					insert(other.begin(), other.end());
					_comp = other._comp;
					_alloc_pair = other._alloc_pair;
					_size = other._size;
				}
				return (*this);
			}

			~AVLTree()
			{
				clear(_root);
				delete_null_node(_node_ptr);
			}

			allocator_type	get_allocator() const
			{
				return (_alloc_pair);
			}

			/****************************************************************************\
			**									Iterators								**
			\****************************************************************************/

			iterator	begin()
			{
				tree_node	*node;

				if (_root == _node_ptr)
					return (iterator(_node_ptr));
				node = _root;
				while (node && node->left != _node_ptr)
					node = node->left;
				return (iterator(node));			
			}

			const_iterator	begin() const
			{
				tree_node	*node;

				if (_root == _node_ptr)
					return (iterator(_node_ptr));
				node = _root;
				while (node && node->left != _node_ptr)
					node = node->left;
				return (iterator(node));			
			}

			iterator	end()
			{
				return (iterator(get_rightmost_node(_root)));
			}

			const_iterator	end() const
			{
				return (iterator(get_rightmost_node(_root)));
			}

			reverse_iterator	rbegin()
			{
				return (reverse_iterator(end()));
			}

			const_reverse_iterator	rbegin() const
			{
				return (reverse_iterator(end()));
			}

			reverse_iterator	rend()
			{
				return (reverse_iterator(begin()));
			}

			const_reverse_iterator	rend() const
			{
				return (reverse_iterator(begin()));
			}

			/****************************************************************************\
			**									Capacity								**
			\****************************************************************************/

			bool	empty() const
			{
				return (!size());
			}

			size_type	size() const
			{
				return (_size);
			}

			size_type	max_size() const { return std::min<size_type>(_alloc_node.max_size(),
									std::numeric_limits<difference_type>::max()); }

			/****************************************************************************\
			**									Modifiers								**
			\****************************************************************************/

			void	clear(tree_node *node)
			{
				if (node && node->leaf)
				{
					clear(node->left);
					clear(node->right);
					delete_node(node);
				}
				_root = _node_ptr;
			}
			
			ft::pair<iterator, bool>	insert(const value_type &data)
			{
				tree_node	*node = _root;
				tree_node	*parent = _node_ptr;

				if (_root == _node_ptr)
				{
					_root = new_node(data, _node_ptr, 2);
					return (ft::make_pair(iterator(_root), true));
				}
				while (node != _node_ptr)
				{
					parent = node;
					if (_comp(data, node->data))
						node = node->left;
					else if (_comp(node->data, data))
						node = node->right;
					else
						return (ft::make_pair(iterator(node), false));
				}
				node = new_node(data, parent, 1);
				if (_comp(data, parent->data))
					parent->left = node;
				else
					parent->right = node;
				node->left->parent = node;
				node->right->parent = node;
				_root = _avl_tree_rebalancing(_root);
				return (ft::make_pair(iterator(node), true));
			}

			iterator	insert(iterator pos, const value_type &data)
			{
				(void)pos;
				return (insert(data).first);
			}

			iterator	insert(const_iterator pos, const value_type &data)
			{
				(void)pos;
				return (insert(data).first);
			}

			template <class InputIt>
			void	insert(InputIt first, InputIt last,
				typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::value* = 0)
			{
				while (first != last)
					insert(*first++);
			}

			void	erase(iterator first, iterator last)
			{
				while (first != last)
					erase(*first++);
			}

			void	erase(const_iterator first, const_iterator last)
			{
				while (first != last)
					erase(*first++);
			}

			void	erase(tree_node *node)
			{
				_avl_tree_node_deletion(node->data);
			}

			size_type	erase(const value_type &data)
			{
				tree_node *found = search(_root, data);
				if (found)
				{
					erase(found);
					return (1);
				}
				return (0);
			}

			void	swap(AVLTree &x)
			{
				ft::swap_elements(_comp, x._comp);
				ft::swap_elements(_node_ptr, x._node_ptr);
				ft::swap_elements(_root, x._root);
				ft::swap_elements(_alloc_node, x._alloc_node);
				ft::swap_elements(_alloc_pair, x._alloc_pair);
				ft::swap_elements(_size, x._size);
			}

			/****************************************************************************\
			**									Lookup									**
			\****************************************************************************/

			iterator	find(const value_type &data)
			{
				tree_node	*result = search(_root, data);
				if (result)
					return (iterator(result));
				return (iterator(get_rightmost_node(_root)));
			}

			iterator	find(const value_type &data) const
			{
				tree_node	*result = search(_root, data);
				if (result)
					return (iterator(result));
				return (iterator(get_rightmost_node(_root)));
			}

			tree_node	*lower_bound(const value_type &data)
			{
				tree_node	*node = _root;
				tree_node	*tmp = _node_ptr;

				while (node != _node_ptr)
				{
					if (!_comp(node->data, data))
					{
						tmp = node;
						node = node->left;
					}
					else
						node = node->right;
				}
				return (tmp);
			}

			tree_node	*lower_bound(const value_type &data) const
			{
				tree_node	*node = _root;
				tree_node	*tmp = _node_ptr;

				while (node != tmp)
				{
					if (!_comp(node->data, data))
					{
						tmp = node;
						node = node->left;
					}
					else
						node = node->right;
				}
				return (tmp);
			}

			tree_node	*upper_bound(const value_type &data)
			{
				tree_node	*node = _root;
				tree_node	*tmp = _node_ptr;

				while (node != tmp)
				{
					if (!_comp(data, node->data))
					{
						tmp = node;
						node = node->left;
					}
					else
						node = node->right;
				}
				return (tmp);
			}

			tree_node	*upper_bound(const value_type &data) const
			{
				tree_node	*node = _root;
				tree_node	*tmp = _node_ptr;

				while (node != tmp)
				{
					if (!_comp(data, node->data))
					{
						tmp = node;
						node = node->left;
					}
					else
						node = node->right;
				}
				return (tmp);
			}

		private:
			/****************************************************************************\
			**									Node									**
			\****************************************************************************/

			tree_node	*new_nil_leaf()
			{
				tree_node	*tmp = _alloc_node.allocate(1);
				tmp->height = 0;
				tmp->leaf = 0;
				tmp->parent = NULL;
				tmp->left = _node_ptr;
				tmp->right = _node_ptr;
				return (tmp);
			}

			tree_node	*new_node(const value_type &data, tree_node *parent, int leaf)
			{
				tree_node	*tmp = _alloc_node.allocate(1);
				_alloc_pair.construct(&(tmp->data), data);
				tmp->height = 1;
				tmp->leaf = leaf;
				tmp->parent = parent;
				tmp->left = _node_ptr;
				tmp->right = _node_ptr;
				++_size;
				return (tmp);
			}

			void	delete_node(tree_node *node)
			{
				_alloc_pair.destroy(&(node->data));
				_alloc_node.deallocate(node, 1);
				--_size;
			}

			void	delete_null_node(tree_node *node)
			{
				_alloc_node.deallocate(node, 1);
				--_size;
			}

		public:
			tree_node	*search(tree_node *search, const value_type &data) const
			{
				while (search != _node_ptr)
				{
					if (_comp(data, search->data))
						search = search->left;
					else if (_comp(search->data, data))
						search = search->right;
					else
						return (search);
				}
				return (NULL);
			}

			tree_node	*_iterative_avl_tree_search(const value_type& val) const
			{
				tree_node	*current(root());
				tree_node	*prev(root());

				while (current)
				{
					if (val == current->data)
						return (current);
					else if (_comp(val, current->data))
					{
						prev = current;
						current = current->left;
					}
					else
					{
						prev = current;
						current = current->right;
					}
				}
				return (prev);
			}

		private:
			/****************************************************************************\
			**																			**
			**						Red-Black trees utility functions					**
			**																			**
			\****************************************************************************/

			tree_node	*get_leftmost_node(tree_node *node) const
			{
				while (node->left != _node_ptr)
					node = node->left;
				return (node);
			}

			tree_node	*get_rightmost_node(tree_node *node) const
			{
				while (node && node->leaf)
					node = node->right;
				return (node);
			}

			void	_avl_tree_transplant(tree_node *u, tree_node *v)
			{
				if (u == NULL)
					return ;
				else if (u->parent == NULL)
					set_root(v);
				else if (u->parent->left == u)
					u->parent->left = v;
				else
					u->parent->right = v;
				if (v != NULL)
					v->parent = u->parent;
			}

			tree_node	*_avl_tree_successor(tree_node *node)
			{

				if (node->right != NULL)
				{
					node = node->right;
					while(node->left != NULL)
						node = node->left;
					return (node);
				}
				else
				{
					while(node->parent != NULL && node->parent->right == node)
						node = node->parent;
					return (node->parent);
				}
			}

			void	_avl_tree_node_deletion(const value_type& val)
			{
				tree_node	*p(NULL);
				tree_node	*parent(NULL);
				tree_node	*succ(NULL);

				p = search(root(), val);
				if (!p)
					return ;
				--_size;
				if (p->left == NULL && p->right == NULL)	// souci
				{
					parent = p->parent;
					if (parent->left == p)
						parent->left = NULL;
					else
					{
						std::cout << "miao:" << p->data.first << std::endl;
						parent->right = NULL;
					}
					std::cout << "miao:" << p->data.first << std::endl;
					delete_node(p);
					set_root(balance_tree(root()));
					return ;
				}
				if (p->left == NULL)
				{
					parent = p->parent;
					if (parent->left == p)
						parent->left = p->right;
					else
						parent->right = p->right;
					delete_node(p);
					set_root(balance_tree(root()));
					return ;
				}
				if (p->right == NULL)
				{
					parent = p->parent;
					if (parent->left == p)
						parent->left = p->left;
					else
						parent->right = p->left;
					delete_node(p);	
					set_root(balance_tree(root()));
					return ;
				}
				succ = _avl_tree_successor(p);
				if (p->right != succ)
				{
					_avl_tree_transplant(succ, succ->right);
					succ->right = p->right;
					succ->right->parent = succ;
				}
				_avl_tree_transplant(p, succ);
				succ->left = p->left;
				succ->left->parent = succ;
				delete_node(p);
				recomp_height(succ);
				set_root(balance_tree(root()));
				return ;
			}

			tree_node	*_avl_tree_rr_rotation(tree_node *root)
			{
				tree_node *temp = root->right;
				root->right = temp->left;
				if (temp->left != NULL)
					temp->left->parent = root;
				temp->left = root;
				temp->parent = root->parent;
				root->parent = temp;
				if (temp->parent != NULL
					&& _comp(root->data, temp->parent->data)) {
					temp->parent->left = temp;
				}
				else {
					if (temp->parent != NULL)
						temp->parent->right = temp;
				}
				root = temp;
				recomp_height(root->left);
				recomp_height(root->right);
				return (root);
			}
			
			tree_node	*_avl_tree_rl_rotation(tree_node *root)
			{
				root->right = _avl_tree_ll_rotation(root->right);
				return (_avl_tree_rr_rotation(root));
			}

			tree_node	*_avl_tree_lr_rotation(tree_node *root)
			{
				root->left = _avl_tree_rr_rotation(root->left);
				return (_avl_tree_ll_rotation(root));
			}

			tree_node	*_avl_tree_ll_rotation(tree_node *root)
			{
				tree_node *temp = root->left;
				root->left = temp->right;
				if (temp->right != NULL)
					temp->right->parent = root;
				temp->right = root;
				temp->parent = root->parent;
				root->parent = temp;
				if (temp->parent != NULL
					&& _comp(root->data, temp->parent->data)) {
					temp->parent->left = temp;
				}
				else {
					if (temp->parent != NULL)
						temp->parent->right = temp;
				}
				root = temp;
				recomp_height(root->left);
				recomp_height(root->right);
				return (root);
			}

			static int	calculate_height(tree_node *temp)
			{
				int h = 0;
				if (temp != NULL) {
					int l_height = calculate_height(temp->left);
					int r_height = calculate_height(temp->right);
					int max_height = std::max(l_height, r_height);
					h = max_height + 1;
				}
				return h;
			}

			int	diff(tree_node *temp)
			{
				int l_height = calculate_height(temp->left);
				int r_height = calculate_height(temp->right);
				int balance_factor = l_height - r_height;
				return (balance_factor);
			}


			void	_avl_tree_node_insertion(tree_node *new_node, tree_node *node)
			{
				if (new_node->parent == _node_ptr)
					_root = node;
				else if (new_node == new_node->parent->left)
					new_node->parent->left = node;
				else
					new_node->parent->right = node;
				node->parent = new_node->parent;
			}

			tree_node	*replace_node(tree_node *node)
			{
				if (!node || (!node->left && !node->right))
					return (NULL);
				if (node->left && node->right)
				{
					tree_node	*tmp = node->right;
					while (tmp->left != NULL)
						tmp = tmp->left;
					return (tmp);
				}
				if (node->right)
					return (node->left);
				return (node->right);
			}

			tree_node	*balance(tree_node *temp)
			{
				int balance_factor = diff(temp);
				if (balance_factor > 1) {
					if (diff(temp->left) > 0)
						temp = _avl_tree_ll_rotation(temp);
					else
						temp = _avl_tree_lr_rotation(temp);
				}
				else if (balance_factor < -1) {
					if (diff(temp->right) > 0)
						temp = _avl_tree_rl_rotation(temp);
					else
						temp = _avl_tree_rr_rotation(temp);
				}
				return (temp);
			}

			tree_node	*_avl_tree_rebalancing(tree_node *root)
			{
				if (root == _node_ptr)
					return (NULL);
				root->left = _avl_tree_rebalancing(root->left);
				root->right = _avl_tree_rebalancing(root->right);
				root = balance(root);
				return (root);
			}

			static void	recomp_height(tree_node *_x)
			{
				while (_x != NULL)
				{
					_x->_height = calculate_height(_x);
					_x = _x->parent;
				}
			}

		public:
			/****************************************************************************\
			**									Getters									**
			\****************************************************************************/

			tree_node	*root() const
			{
				return (_root);
			}

		private:
			value_compare	_comp;
			tree_node		*_node_ptr;
			tree_node		*_root;
			node_allocator	_alloc_node;
			allocator_type	_alloc_pair;
			size_type		_size;
	};
}


#endif