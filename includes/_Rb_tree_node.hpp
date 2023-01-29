#ifndef _RB_TREE_NODE_H
# define _RB_TREE_NODE_H

# include <iostream>

namespace ft {

	template <class T>
	struct Node
	{
		typedef T	value_type;
		value_type	data;
		std::string	color;
		int			leaf;
		Node		*parent;
		Node		*left;
		Node		*right;

		Node(void) : data(NULL), parent(NULL), left(NULL), right(NULL), color("red") {}

		Node(T const &data, Node *parent, int leaf) : data(data), parent(parent), left(NULL), right(NULL), color("red"), leaf(leaf) {}

		Node(Node const &other) : data(other.data), parent(other.parent), left(other.left), right(other.right), color(other.color), leaf(other.leaf) {}

		Node &operator=(Node const &other)
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

		~Node(void) {}

		friend std::ostream	&operator<< (std::ostream &o, const Node &node) {
			o << "key: " << node.data.first;
			o << ", value: " << node.data.second;
			o << ", color: " << node.color << std::endl;
			return (o);
		}
	};
}

#endif
