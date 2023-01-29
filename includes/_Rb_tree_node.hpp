#ifndef _RB_TREE_NODE_H
# define _RB_TREE_NODE_H

# include <iostream>

namespace ft {

	template <class T>
	struct node
	{
		typedef T	value_type;
		value_type	data;
		bool		color;
		int			leaf;
		node		*parent;
		node		*left;
		node		*right;

		node(void) : data(NULL), parent(NULL), left(NULL), right(NULL), color(0)
		{

		}

		node(T const &data, node *parent, int leaf) : data(data), parent(parent), left(NULL), right(NULL), color(0), leaf(leaf)
		{

		}

		node(node const &other) : data(other.data), parent(other.parent), left(other.left), right(other.right), color(other.color), leaf(other.leaf)
		{

		}

		node &operator=(node const &other)
		{
			if (this != &other)
			{
				this->data = other.data;
				this->color = other.color;
				this->leaf = other.leaf;
				this->parent = other.parent;
				this->left = other.left;
				this->right = other.right;
			}
			return (*this);
		}

		~node(void)
		{

		}
	};
}

#endif
