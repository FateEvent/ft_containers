#ifndef _TREE_NODE_H
# define _TREE_NODE_H

# include <iostream>

template<class Pair>
struct Node {
public:
	Pair	_data;
	Node		*_left;
	Node		*_right;
	Node		*_parent;
	int			_height;

public:
	Node() : _data(Pair()), _left(NULL), _right(NULL), _parent(NULL), _height(1) {}
	Node(Pair pair) : _data(pair), _left(NULL), _right(NULL), _parent(NULL), _height(1) {}
	~Node() {}

	Node		&operator= (Node &other) { _left = other._left; _right = other._right; _parent = other._parent; _height = other._height; return (*this); }
	void		set_data(Pair &data) { _data = data; }
	void		set_left(Node *left) { _left = left; }
	void		set_right(Node *right) { _right = right; }
	void		set_parent(Node *parent) { _parent = parent; }
	Pair		&data() { return _data; }
	Node		*left() { return _left; }
	Node		*right() { return _right; }
	Node		*parent() { return _parent; }
	int			&height() { return _height; }
};

#endif
