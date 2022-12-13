#ifndef _NODE_H
# define _NODE_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "ContainerException.hpp"
# include "iterator.hpp"
# include "pair.hpp"

template <class T>
struct _node {
private:
	T				_data;
	struct _node	*_left;
	struct _node	*_right;
	int				_balance;

public:
	_node(T c) : _data(c), _left(NULL), _right(NULL), _balance() {}
	~_node() {}

	_node	&operator= (_node &other) { _left = other._left; _right = other._right; _balance = other._balance; return (*this); }
	void	change_left(_node *left) { _left = left; }
	void	change_right(_node *right) { _right = right; }
	void	change_data(T &data) { _data = data; }
	_node	*left() { return _left; }
	_node	*right() { return _right; }
	T    data() { return _data; }
	void	treat(char sep) { std::cout << data().first << sep << data().second << sep << std::endl; }

	void	suppress_node()
	{
		std::cout << "seggy" << std::endl;
		if (left())
			left()->suppress_node();
		std::cout << "seggy" << std::endl;
		if (right())
			right()->suppress_node();
		std::cout << "seggy" << std::endl;
		delete (this);
	};
};

#endif