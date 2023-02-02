#include <iostream>
#include <string>
#include <stdlib.h>
#include "pair.hpp"
#include <set.hpp>

template <typename T>
std::string	printPair(const T &iterator, bool nl = true, std::ostream &o = std::cout)
{
	o << "value: " << *iterator;
	if (nl)
		o << std::endl;
	return ("");
}

template <typename T_SET>
void	print_tree(T_SET const &set, bool print_content = 1)
{
	std::cout << "size: " << set.size() << std::endl;
	std::cout << "max_size: " << set.max_size() << std::endl;
	if (print_content)
	{
		typename T_SET::const_iterator it = set.begin(), ite = set.end();
		std::cout << std::endl << "Content is:" << std::endl;
		for (; it != ite; ++it)
			std::cout << "- " << *it << std::endl;
	}
	std::cout << "###############################################" << std::endl;
}

#define _pair ft::pair
typedef ft::set<int, std::string>::iterator ft_iterator;
typedef ft::set<int, std::string>::const_iterator ft_const_iterator;

static int iter = 0;

template <typename SET>
void	ft_bound(SET &st, const int &param)
{
	ft_iterator ite = st.end(), it[2];
	_pair<ft_iterator, ft_iterator> ft_range;

	std::cout << "\t-- [" << iter++ << "] --" << std::endl;
	std::cout << "with key [" << param << "]:" << std::endl;
	it[0] = st.lower_bound(param); it[1] = st.upper_bound(param);
	ft_range = st.equal_range(param);
	std::cout << "lower_bound: " << (it[0] == ite ? "end()" : printPair(it[0], false)) << std::endl;
	std::cout << "upper_bound: " << (it[1] == ite ? "end()" : printPair(it[1], false)) << std::endl;
	std::cout << "equal_range: " << (ft_range.first == it[0] && ft_range.second == it[1]) << std::endl;
}

template <typename SET>
void	ft_const_bound(const SET &st, const int &param)
{
	ft_const_iterator ite = st.end(), it[2];
	_pair<ft_const_iterator, ft_const_iterator> ft_range;

	std::cout << "\t-- [" << iter++ << "] (const) --" << std::endl;
	std::cout << "with key [" << param << "]:" << std::endl;
	it[0] = st.lower_bound(param); it[1] = st.upper_bound(param);
	ft_range = st.equal_range(param);
	std::cout << "lower_bound: " << (it[0] == ite ? "end()" : printPair(it[0], false)) << std::endl;
	std::cout << "upper_bound: " << (it[1] == ite ? "end()" : printPair(it[1], false)) << std::endl;
	std::cout << "equal_range: " << (ft_range.first == it[0] && ft_range.second == it[1]) << std::endl;
}

int main() {

	std::cout << "// --> My tests <-- //" << std::endl << std::endl;

	std::cout << "// --> SET <-- //" << std::endl << std::endl;

	ft::set<char> tree;

	tree.insert('a');
	tree.insert('c');
	tree.insert('b');
	tree.insert('c');
	tree.insert('d');
	tree.insert('f');
	std::cout << "tree.begin()" << std::endl; 
	std::cout << *tree.begin() << std::endl; 
	tree.insert(tree.begin(), 'l');
	std::cout << "tree.begin()" << std::endl; 
	std::cout << *tree.begin() << std::endl;

	tree.erase('c');
	tree.erase('d');
	tree.erase('f');

	ft::set<char>::iterator	it = tree.find('c');
	if (it != tree.end())
		std::cout << *it << std::endl;
	
	it = tree.find('a');
	if (it != tree.end())
		std::cout << *it << std::endl;
	
	it = tree.find('a');
	if (it != tree.end())
		std::cout << *it << std::endl;

	ft::set<char> lemontree(tree);

	print_tree(lemontree);

	ft::set<int>	set;

	set.insert(5);
	set.insert(15);
	set.insert(29);
	set.insert(35);
	set.insert(18);
	set.insert(7);
	set.insert(0);
	set.insert(134);
	set.insert(-4);
	set.insert(7);
	set.insert(76);
	set.insert(90);
	set.insert(56);
	ft::set<int> cherrytree(set.begin(), set.end());
	print_tree(cherrytree);

	ft_const_bound(cherrytree, 6);
	ft_const_bound(cherrytree, 1);
	ft_const_bound(cherrytree, 5);
	ft_const_bound(cherrytree, 10);
	ft_const_bound(cherrytree, 50);

	ft_bound(cherrytree, 5);
	ft_bound(cherrytree, 7);

	cherrytree.erase(cherrytree.begin(), cherrytree.end());
	print_tree(cherrytree);
	std::cout << cherrytree.empty() << std::endl;

	set.clear();
	print_tree(set);
	std::cout << set.empty() << std::endl;
}
