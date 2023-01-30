#ifndef RB_ITERATOR_H
# define RB_ITERATOR_H

# include <memory>
# include <iterator>
# include "_Rb_tree_node.hpp"

namespace ft
{
	template <class I>
	class RBIterator
	{
	public:
		typedef I								value_type;
		typedef std::bidirectional_iterator_tag	iterator_category;
		typedef Node<value_type>				tree_node;
		typedef Node<const value_type>			const_tree_node;
		typedef ptrdiff_t						difference_type;
		typedef I&								reference;
		typedef const I&						const_reference;
		typedef I*								pointer;
		typedef const I*						const_pointer;
			
		typedef RBIterator<I>					iterator;
		typedef RBIterator<const I>				const_iterator;

	private:
		tree_node	*node;
		
	public:
		RBIterator() : node(NULL) {}

		RBIterator(tree_node *current) : node(current) {}

		RBIterator(RBIterator const &other) : node(other.base()) {}

		RBIterator	&operator=(RBIterator const &other)
		{
			if (this != &other)
				node = other.base();
			return (*this);
		}

		operator	const_iterator() const
		{
			return (const_iterator(reinterpret_cast<const_tree_node *>(node)));
		}

		tree_node	*base() const
		{
			return (node);
		}

		reference	operator*()
		{
			return (node->data);
		}

		const_reference	operator*() const
		{
			return (node->data);
		}

		pointer	operator->() const
		{
			return &(node->data);
		}

		RBIterator	&operator++()
		{
			if (!node->leaf)
				return (*this);
			if (node && node->right && node->right->leaf)
			{
				node = node->right;
				while (node && node->left && node->left->leaf)
					node = node->left;
			}
			else
			{
				tree_node	*tmp = node;
				node = node->parent;
				while (node && node->leaf && node->right == tmp)
				{
					tmp = node;
					node = node->parent;
				}
			}
			return (*this);
		}

		RBIterator	operator++(int)
		{
			RBIterator	tmp(*this);
			operator++();
			return (tmp);
		}

		RBIterator	&operator--()
		{
			if (!node->leaf)
			{
				while (node->parent->leaf)
					node = node->parent;
				while (node->right->leaf)
					node = node->right;
				return (*this);
			}
			else if (node->left && node->left->leaf)
			{
				node = node->left;
				while (node->right && node->right->leaf)
					node = node->right;
			}
			else
			{
				tree_node	*tmp = node;
				node = node->parent;
				while (node && node->leaf && node->left == tmp)
				{
					tmp = node;
					node = node->parent;
				}
			}
			return (*this);
		}

		RBIterator	operator--(int)
		{
			RBIterator	tmp(*this);
			operator--();
			return (tmp);
		}

		bool	operator==(RBIterator const &other)
		{
			return (node == other.node);
		}

		bool	operator!=(RBIterator const &other)
		{
			return (node != other.node);
		}

		bool	operator>(RBIterator const &other)
		{
			return (node > other.node);
		}

		bool	operator>=(RBIterator const &other)
		{
			return (node >= other.node);
		}

		bool	operator<(RBIterator const &other)
		{
			return (node < other.node);
		}

		bool	operator<=(RBIterator const &other)
		{
			return (node <= other.node);
		}
	};
}

#endif