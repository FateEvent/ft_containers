#ifndef _RB_TREE_H
# define _RB_TREE_H

# include "utilities.hpp"
# include "pair.hpp"
# include "_Rb_tree_node.hpp"
# include "_Rb_iterator.hpp"
# include "iterator_traits.hpp"

namespace ft
{
	template <class T, class Compare = std::less<T>, class Allocator = std::allocator<T> >
	class RBTree
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

			explicit RBTree(const value_compare &comp, const allocator_type &alloc) : _comp(comp), _node_ptr(new_nil_leaf()), _alloc_pair(alloc), _size(0)
			{
				_root = _node_ptr;
				_node_ptr->color = "black";
			}

			RBTree(const RBTree &other) : _comp(other._comp), _node_ptr(new_nil_leaf()), _root(_node_ptr), _alloc_pair(other._alloc_pair), _size(0)
			{
				*this = other;
			}

			RBTree &operator=(const RBTree &other)
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

			~RBTree()
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
					_root->color = "black";
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
				_Rb_tree_rebalancing(node);
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
				tree_node	*search = node;
				tree_node	*tmp;
				std::string	color = node->color;

				if (node->left == _node_ptr)
				{
					tmp = node->right;
					_Rb_tree_node_insertion(node, node->right);
				}
				else if (node->right == _node_ptr)
				{
					tmp = node->left;
					_Rb_tree_node_insertion(node, node->left);
				}
				else
				{
					search = get_leftmost_node(node->right);
					color = search->color;
					tmp = search->right;
					if (search->parent == node)
						tmp->parent = search;
					else
					{
						_Rb_tree_node_insertion(search, search->right);
						search->right = node->right;
						search->right->parent = search;
					}
					_Rb_tree_node_insertion(node, search);
					search->left = node->left;
					search->left->parent = search;
					search->color = node->color;
				}
				if (color == "black")
					_Rb_tree_recolouring(tmp);
				delete_node(node);
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

			void	swap(RBTree &x)
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
			**										Node								**
			\****************************************************************************/

			tree_node	*new_nil_leaf()
			{
				tree_node	*tmp = _alloc_node.allocate(1);
				tmp->color = "black";
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
				tmp->color = "red";
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

			void	_Rb_tree_left_rotation(tree_node *node)
			{
				tree_node	*tmp = node->right;
				node->right = tmp->left;
				if (tmp->left != _node_ptr)
					tmp->left->parent = node;
				tmp->parent = node->parent;
				if (node->parent == _node_ptr)
					_root = tmp;
				else if (node->parent && node == node->parent->left)
					node->parent->left = tmp;
				else
					node->parent->right = tmp;
				tmp->left = node;
				node->parent = tmp;
			}

			void	_Rb_tree_right_rotation(tree_node *node)
			{
				tree_node	*tmp = node->left;
				node->left = tmp->right;
				if (tmp->right != _node_ptr)
					tmp->right->parent = node;
				tmp->parent = node->parent;
				if (node->parent == _node_ptr)
					_root = tmp;
				else if (node->parent && node == node->parent->right)
					node->parent->right = tmp;
				else
					node->parent->left = tmp;
				tmp->right = node;
				node->parent = tmp;
			}

			void	_Rb_tree_swap_node_colour(tree_node *node)
			{
				if (node->color == "red")
					node->color = "black";
				else
					node->color = "red";
			}

			void	_Rb_tree_node_insertion(tree_node *new_node, tree_node *node)
			{
				if (new_node->parent == _node_ptr)
					_root = node;
				else if (new_node == new_node->parent->left)
					new_node->parent->left = node;
				else
					new_node->parent->right = node;
				node->parent = new_node->parent;
			}

			void	_Rb_tree_rebalancing(tree_node *node)
			{
				while (node != _root && node->parent->color == "red")
				{
					tree_node	*node_grandpa = node->parent->parent;
					if (node->parent == node_grandpa->right)
					{
						tree_node	*uncleNode = node_grandpa->left;
						if (uncleNode->color == "red")
						{
							uncleNode->color = "black";
							node->parent->color = "black";
							node_grandpa->color = "red";
							node = node_grandpa;
						}
						else
						{
							if (node == node->parent->left)
							{
								node = node->parent;
								_Rb_tree_right_rotation(node);
							}
							node->parent->color = "black";
							node_grandpa->color = "red";
							_Rb_tree_left_rotation(node_grandpa);
						}
					}
					else
					{
						tree_node	*node_uncle = node_grandpa->right;
						if (node_uncle->color == "red")
						{
							node_uncle->color = "black";
							node->parent->color = "black";
							node_grandpa->color = "red";
							node = node_grandpa;
						}
						else
						{
							if (node == node->parent->right)
							{
								node = node->parent;
								_Rb_tree_left_rotation(node);
							}
							node->parent->color = "black";
							node_grandpa->color = "red";
							_Rb_tree_right_rotation(node_grandpa);
						}
					}                
				}
				_root->color = "black";
			}

			void	_Rb_tree_recolouring(tree_node *node)
			{
				while (node != _root && node->color == "black")
				{
					if( node == node->parent->left)
					{
						tree_node *sibling = node->parent->right;
						if (sibling->color == "red")
						{
							_Rb_tree_swap_node_colour(sibling);
							node->parent->color = "red"; 
							_Rb_tree_left_rotation(node->parent);
							sibling = node->parent->right;
						}
						if (sibling->left->color == "black" && sibling->right->color == "black")
						{
							sibling->color = "red";
							node = node->parent;
						}
						else
						{
							if (sibling->right->color == "black")
							{
								sibling->left->color = "black";
								sibling->color = "red";
								_Rb_tree_right_rotation(sibling);
								sibling = node->parent->right;
							}
							sibling->color = node->parent->color;
							node->parent->color = "black";
							sibling->right->color = "black";
							_Rb_tree_left_rotation(node->parent);
							node = _root;
						}
					}
					else
					{
						tree_node *sibling = node->parent->left;
						if (sibling->color == "red")
						{
							_Rb_tree_swap_node_colour(sibling);
							node->parent->color = "red";
							_Rb_tree_right_rotation(node->parent);
							sibling = node->parent->left;
						}
						if (sibling->right->color == "black" && sibling->left->color == "black")
						{
							sibling->color = "red";
							node = node->parent;
						}
						else
						{
							if (sibling->left->color == "black")
							{
								sibling->right->color = "black";
								sibling->color = "red";
								_Rb_tree_left_rotation(sibling);
								sibling = node->parent->left;
							}
							sibling->color = node->parent->color;
							node->parent->color = "black";
							sibling->left->color = "black";
							_Rb_tree_right_rotation(node->parent);
							node = _root;
						}
					}
				}
				node->color = "black";
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