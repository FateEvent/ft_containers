#ifndef _AVL_TREE_NODE_H
# define _AVL_TREE_NODE_H

# include <iostream>

namespace ft {

	template <class T>
	struct Node
	{
		typedef T	value_type;
		value_type	data;
		size_t		height;
		int			leaf;
		Node		*parent;
		Node		*left;
		Node		*right;

		Node() : data(NULL), parent(NULL), left(NULL), right(NULL), height(1) {}

		Node(T const &data, Node *parent, int leaf) : data(data), parent(parent), left(NULL), right(NULL), height(1), leaf(leaf) {}

		Node(Node const &other) : data(other.data), parent(other.parent), left(other.left), right(other.right), height(other.height), leaf(other.leaf) {}

		Node &operator=(Node const &other)
		{
			if (this != &other)
			{
				this->data = other.data;
				this->height = other.height;
				this->leaf = other.leaf;
				this->parent = other.parent;
				this->left = other.left;
				this->right = other.right;
			}
			return (*this);
		}

		~Node() {}

		friend std::ostream	&operator<< (std::ostream &o, const Node &node) {
			o << "key: " << node.data.first;
			o << ", value: " << node.data.second;
			o << ", height: " << node.height << std::endl;
			return (o);
		}
	};
}

#endif
