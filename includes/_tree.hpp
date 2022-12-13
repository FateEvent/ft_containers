#ifndef _TREE_H
# define _TREE_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "ContainerException.hpp"
# include "_node.hpp"
# include "iterator.hpp"
# include "pair.hpp"
# include <deque>

template<class T>
class _tree {
private:
	Node	*_root;

public:
	_tree() { _root = NULL; }
	~_tree() { _root->suppress_node(); }
	_tree&	operator= (_tree const& base);
	Node	*root() { return _root; }
	void	change_root(Node *current) { _root = current; }

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
			std::cout << '(' << std::endl;
			infix_traversal(current->left(), sep);
			current->treat(sep);
			infix_traversal(current->right(), sep);
			std::cout << ')' << std::endl;
		}
	}

	void	suffix_traversal(Node *current, char sep) {
		if (current)
		{
			suffix_traversal(current->left(), sep);
			suffix_traversal(current->right(), sep);
			current->treat(sep);
		}
	}

	void	level_order_traversal(Node *current, char sep) {
		std::deque<Node *>	deck;

		deck.push_back(current);
		while (!deck.empty())
		{
			current = deck.front();
			deck.pop_front();
			current->treat(sep);
			if (current->left())
				deck.push_back(current->left());
			if (current->right())
				deck.push_back(current->right());
		}
	}

	void	insert(Node *current, T data) {
		Node *newNode = new Node(pair);

		if (current)
		{
			if (_key_comp(pair.first, current->data().first))
			{
				if (current->left())
					insert(current->left(), pair);
				else
					current->change_left(newNode);
			}
			else
			{
				if (current->right())
					insert(current->right(), pair);
				else
					current->change_right(newNode);
			}
		}
		else
			current = newNode;
	}
}

#endif