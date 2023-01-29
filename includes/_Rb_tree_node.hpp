#ifndef _TREE_NODE_H
# define _TREE_NODE_H

# include <iostream>

template<class Pair>
struct Node {
public:
	Pair		_data;
	Node		*_left;
	Node		*_right;
	Node		*_parent;
	int			_height;
	std::string	_colour;

	Node() : _data(Pair()), _left(NULL), _right(NULL), _parent(NULL), _height(1), _colour("black") {}
	Node(Pair pair) : _data(pair), _left(NULL), _right(NULL), _parent(NULL), _height(1), _colour("black") {}
	~Node() {}

	Node		&operator= (Node &other) { _left = other._left; _right = other._right; _parent = other._parent; _colour = other._colour; return (*this); }
	void		set_data(Pair &data) { _data = data; }
	void		set_left(Node *left) { _left = left; }
	void		set_right(Node *right) { _right = right; }
	void		set_parent(Node *parent) { _parent = parent; }
	void		set_height(int height) { _height = height; }
	void		set_colour(std::string colour) { _colour = colour; }
	Pair		&data() { return _data; }
	Node		*left() { return _left; }
	Node		*right() { return _right; }
	Node		*parent() { return _parent; }
	int			&height() { return _height; }
	std::string	&colour() { return _colour; }

	bool		is_left_child() { return this == this->parent()->left(); }

	void		move_down(Node *parent) {
		if (this->parent() != NULL) {
			if (this->is_left_child()) {
				this->parent()->set_left(parent);
			} else {
				this->parent()->set_right(parent);
			}
		}
		parent->set_parent(this->parent());
		this->set_parent(parent);
	}

	bool	has_red_child() {
		return (this->left() != NULL and this->left()->colour() == "red") or
			(this->right() != NULL and this->right()->colour() == "red");
	}

	Node	*get_sibling() {
		if (this->parent() == NULL)
				return (NULL);
		if (this->is_left_child())
				return this->parent()->right();
		return (this->parent()->left());
	}

	Node	*get_uncle_node()
	{
		if (this->parent() && this->parent()->parent())
		{
			if (this->parent()->is_left_child())
				return (this->parent()->parent()->right());
			else
				return (this->parent()->parent()->left());
		}
		return (NULL);
	}
};

#endif
