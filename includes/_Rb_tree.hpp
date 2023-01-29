#ifndef RB_TREE_H
# define RB_TREE_H

# include "utilities.hpp"
# include "pair.hpp"
# include "rb_iterator.hpp"
# include "iterator.hpp"

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
			typedef node<value_type>										node_tree;
			typedef typename Allocator::template rebind<node_tree>::other	node_allocator;
		
		private:
			value_compare	_comp;
			node_tree		*_node_ptr;
			node_tree		*_root;
			node_allocator	_node_allocator;
			allocator_type	_allocator_type;
			size_type		_size;

		public:
			explicit RBTree(const value_compare &comp, const allocator_type &alloc) : _comp(comp), _node_ptr(newNullNode()), _allocator_type(alloc), _size(0)
			{
				_root = _node_ptr;
				_node_ptr->color = "black";
			}

			RBTree(const RBTree &other) : _comp(other._comp), _node_ptr(newNullNode()), _root(_node_ptr), _allocator_type(other._allocator_type), _size(0)
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
					_allocator_type = other._allocator_type;
					_size = other._size;
				}
				return (*this);
			}

			~RBTree(void)
			{
				clear(_root);
				deleteNullNode(_node_ptr);
			}

			// iterators
			iterator	begin(void)
			{
				node_tree	*node;

				if (_root == _node_ptr)
					return (iterator(_node_ptr));
				node = _root;
				while (node && node->left != _node_ptr)
					node = node->left;
				return (iterator(node));			
			}

			const_iterator	begin(void) const
			{
				node_tree	*node;

				if (_root == _node_ptr)
					return (iterator(_node_ptr));
				node = _root;
				while (node && node->left != _node_ptr)
					node = node->left;
				return (iterator(node));			
			}

			iterator	end(void)
			{
				return (iterator(getMax(_root)));
			}

			const_iterator	end(void) const
			{
				return (iterator(getMax(_root)));
			}

			reverse_iterator	rbegin(void)
			{
				return (reverse_iterator(end()));
			}

			const_reverse_iterator	rbegin(void) const
			{
				return (reverse_iterator(end()));
			}

			reverse_iterator	rend(void)
			{
				return (reverse_iterator(begin()));
			}

			const_reverse_iterator	rend(void) const
			{
				return (reverse_iterator(begin()));
			}

			// capacity
			bool	empty(void) const
			{
				return (_size == 0);
			}

			size_type	size(void) const
			{
				return (_size);
			}

			size_type	max_size(void) const
			{
				return (_node_allocator.max_size());
			}

			// modifiers
			void	clear(node_tree *node)
			{
				if (node && node->leaf)
				{
					clear(node->left);
					clear(node->right);
					deleteTreeNode(node);
				}
				_root = _node_ptr;
			}
			
			ft::pair<iterator, bool>	insert(const value_type &data)
			{
				node_tree	*node = _root;
				node_tree	*parent = _node_ptr;

				if (_root == _node_ptr)
				{
					_root = newTreeNode(data, _node_ptr, 2);
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
				node = newTreeNode(data, parent, 1);
				if (_comp(data, parent->data))
					parent->left = node;
				else
					parent->right = node;
				node->left->parent = node;
				node->right->parent = node;
				rebalanceTree(node);
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

			template <class InputIterator>
			void	insert(InputIterator first, InputIterator last)
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

			void	erase(node_tree *node)
			{
				node_tree	*search = node;
				node_tree	*tmp;
				int			color = node->color;

				if (node->left == _node_ptr)
				{
					tmp = node->right;
					insertNode(node, node->right);
				}
				else if (node->right == _node_ptr)
				{
					tmp = node->left;
					insertNode(node, node->left);
				}
				else
				{
					search = getMin(node->right);
					color = search->color;
					tmp = search->right;
					if (search->parent == node)
						tmp->parent = search;
					else
					{
						insertNode(search, search->right);
						search->right = node->right;
						search->right->parent = search;
					}
					insertNode(node, search);
					search->left = node->left;
					search->left->parent = search;
					search->color = node->color;
				}
				if (color == "black")
					recolorTree(tmp);
				deleteTreeNode(node);
			}

			size_type	erase(const value_type &data)
			{
				node_tree *tmp = search(_root, data);
				if (tmp)
				{
					erase(tmp);
					return (1);
				}
				return (0);
			}

			void	swap(RBTree &x)
			{
				ft::swap_elements(_comp, x._comp);
				ft::swap_elements(_node_ptr, x._node_ptr);
				ft::swap_elements(_root, x._root);
				ft::swap_elements(_node_allocator, x._node_allocator);
				ft::swap_elements(_allocator_type, x._allocator_type);
				ft::swap_elements(_size, x._size);
			}

			// lookup
			iterator	find(const value_type &data)
			{
				node_tree	*result = search(_root, data);
				if (result)
					return (iterator(result));
				return (iterator(getMax(_root)));
			}

			iterator	find(const value_type &data) const
			{
				node_tree	*result = search(_root, data);
				if (result)
					return (iterator(result));
				return (iterator(getMax(_root)));
			}

			node_tree	*lower_bound(const value_type &data)
			{
				node_tree	*node = _root;
				node_tree	*tmp = _node_ptr;

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

			node_tree	*lower_bound(const value_type &data) const
			{
				node_tree	*node = _root;
				node_tree	*tmp = _node_ptr;

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

			node_tree	*upper_bound(const value_type &data)
			{
				node_tree	*node = _root;
				node_tree	*tmp = _node_ptr;

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

			node_tree	*upper_bound(const value_type &data) const
			{
				node_tree	*node = _root;
				node_tree	*tmp = _node_ptr;

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

			// allocator
			allocator_type	get_allocator(void) const
			{
				return (_allocator_type);
			}

		private:
			// node
			node_tree	*newNullNode(void)
			{
				node_tree	*tmp = _node_allocator.allocate(1);
				tmp->color = "black";
				tmp->leaf = 0;
				tmp->parent = NULL;
				tmp->left = _node_ptr;
				tmp->right = _node_ptr;
				return (tmp);
			}

			node_tree	*newTreeNode(const value_type &data, node_tree *parent, int leaf)
			{
				node_tree	*tmp = _node_allocator.allocate(1);
				_allocator_type.construct(&(tmp->data), data);
				tmp->color = "red";
				tmp->leaf = leaf;
				tmp->parent = parent;
				tmp->left = _node_ptr;
				tmp->right = _node_ptr;
				++_size;
				return (tmp);
			}

			void	deleteTreeNode(node_tree *node)
			{
				_allocator_type.destroy(&(node->data));
				_node_allocator.deallocate(node, 1);
				--_size;
			}

			void	deleteNullNode(node_tree *node)
			{
				_node_allocator.deallocate(node, 1);
				--_size;
			}

		public:
			node_tree	*search(node_tree *search, const value_type &data) const
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
			node_tree	*getMin(node_tree *node) const
			{
				while (node->left != _node_ptr)
					node = node->left;
				return (node);
			}

			node_tree	*getMax(node_tree *node) const
			{
				while (node && node->leaf)
					node = node->right;
				return (node);
			}

			void	rotateTreeLeft(node_tree *node)
			{
				node_tree	*tmp = node->right;
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

			void	rotateTreeRight(node_tree *node)
			{
				node_tree	*tmp = node->left;
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

			void	swapNodeColor(node_tree *node)
			{
				if (node->color == "red")
					node->color = "black";
				else
					node->color = "red";
			}

			void	insertNode(node_tree *new_node, node_tree *node)
			{
				if (new_node->parent == _node_ptr)
					_root = node;
				else if (new_node == new_node->parent->left)
					new_node->parent->left = node;
				else
					new_node->parent->right = node;
				node->parent = new_node->parent;
			}

			node_tree	*replaceNode(node_tree *node)
			{
				if (!node || (!node->left && !node->right))
					return (NULL);
				if (node->left && node->right)
				{
					node_tree	*tmp = node->right;
					while (tmp->left != NULL)
						tmp = tmp->left;
					return (tmp);
				}
				if (node->right)
					return (node->left);
				return (node->right);
			}

			void    rebalanceTree(node_tree *node)
        	{
            	while (node != _root && node->parent->color == "red")
            	{
            	    node_tree    *node_grandma = node->parent->parent;
            	    if (node->parent == node_grandma->right)
            	    {
            	        node_tree    *uncleNode = node_grandma->left;
            	        if (uncleNode->color == "red")
            	        {
            	            uncleNode->color = "black";
            	            node->parent->color = "black";
            	            node_grandma->color = "red";
            	            node = node_grandma;
            	        }
            	        else
            	        {
            	            if (node == node->parent->left)
            	            {
            	                node = node->parent;
            	                rotateTreeRight(node);
            	            }
            	            node->parent->color = "black";
            	            node_grandma->color = "red";
            	            rotateTreeLeft(node_grandma);
            	        }
            	    }
            	    else
            	    {
            	        node_tree    *node_uncle = node_grandma->right;
            	        if (node_uncle->color == "red")
            	        {
            	            node_uncle->color = "black";
            	            node->parent->color = "black";
            	            node_grandma->color = "red";
            	            node = node_grandma;
            	        }
            	        else
            	        {
            	            if (node == node->parent->right)
            	            {
            	                node = node->parent;
            	                rotateTreeLeft(node);
            	            }
            	            node->parent->color = "black";
            	            node_grandma->color = "red";
            	            rotateTreeRight(node_grandma);
            	        }
            	    }                
            	}
            	_root->color = "black";
			}

        	void    recolorTree(node_tree *node)
        	{
        	    while (node != _root && node->color == "black")
        	    {
        	        if( node == node->parent->left)
        	        {
        	            node_tree *sibling = node->parent->right;
        	            if (sibling->color == "red")
        	            {
        	                swapNodeColor(sibling);
        	                node->parent->color = "red"; 
        	                rotateTreeLeft(node->parent);
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
        	                    rotateTreeRight(sibling);
        	                    sibling = node->parent->right;
        	                }
        	                sibling->color = node->parent->color;
        	                node->parent->color = "black";
        	                sibling->right->color = "black";
        	                rotateTreeLeft(node->parent);
        	                node = _root;
        	            }
        	        }
        	        else
        	        {
        	            node_tree *sibling = node->parent->left;
        	            if (sibling->color == "red")
        	            {
        	                swapNodeColor(sibling);
        	                node->parent->color = "red";
        	                rotateTreeRight(node->parent);
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
        	                    rotateTreeLeft(sibling);
        	                    sibling = node->parent->left;
        	                }
        	                sibling->color = node->parent->color;
        	                node->parent->color = "black";
        	                sibling->left->color = "black";
        	                rotateTreeRight(node->parent);
        	                node = _root;
        	            }
        	        }
        	    }
        	    node->color = "black";
        	}

		public:
			// utils
			node_tree	*get_root(void) const
			{
				return (_root);
			}
	};
}


#endif