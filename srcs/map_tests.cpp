#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#if 0 //CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
	#include <map.hpp>
	#include <stack.hpp>
	#include <vector.hpp>
#endif

template <typename T>
std::string	print_pair(const T &iterator, bool nl = true, std::ostream &o = std::cout)
{
	o << "key: " << iterator->first << " | value: " << iterator->second;
	if (nl)
		o << std::endl;
	return ("");
}

template <typename T_MAP>
void	print_tree(T_MAP const &mp, bool print_content = 1)
{
	std::cout << "size: " << mp.size() << std::endl;
	std::cout << "max_size: " << mp.max_size() << std::endl;
	if (print_content)
	{
		typename T_MAP::const_iterator it = mp.begin(), ite = mp.end();
		std::cout << std::endl << "Content is:" << std::endl;
		for (; it != ite; ++it)
			std::cout << "- " << print_pair(it, false) << std::endl;
	}
	std::cout << "###############################################" << std::endl;
}

template <typename T1, typename T2>
void	reverse_print(ft::map<T1, T2> &mp)
{
	typename ft::map<T1, T2>::iterator it = mp.end(), ite = mp.begin();

	std::cout << "reverse_print:" << std::endl;
	while (it != ite) {
		it--;
		std::cout << "-> " << print_pair(it, false) << std::endl;
	}
	std::cout << "_______________________________________________" << std::endl;
}

#define _pair ft::pair
typedef ft::map<int, std::string>::iterator ft_iterator;
typedef ft::map<int, std::string>::const_iterator ft_const_iterator;

static int iter = 0;

template <typename MAP>
void	ft_bound(MAP &mp, const int &param)
{
	ft_iterator ite = mp.end(), it[2];
	_pair<ft_iterator, ft_iterator> ft_range;

	std::cout << "\t-- [" << iter++ << "] --" << std::endl;
	std::cout << "with key [" << param << "]:" << std::endl;
	it[0] = mp.lower_bound(param); it[1] = mp.upper_bound(param);
	ft_range = mp.equal_range(param);
	std::cout << "lower_bound: " << (it[0] == ite ? "end()" : print_pair(it[0], false)) << std::endl;
	std::cout << "upper_bound: " << (it[1] == ite ? "end()" : print_pair(it[1], false)) << std::endl;
	std::cout << "equal_range: " << (ft_range.first == it[0] && ft_range.second == it[1]) << std::endl;
}

template <typename MAP, typename T1>
void	ft_const_bound(const MAP &mp, const T1 &param)
{
	ft_const_iterator ite = mp.end(), it[2];
	_pair<ft_const_iterator, ft_const_iterator> ft_range;

	std::cout << "\t-- [" << iter++ << "] (const) --" << std::endl;
	std::cout << "with key [" << param << "]:" << std::endl;
	it[0] = mp.lower_bound(param); it[1] = mp.upper_bound(param);
	ft_range = mp.equal_range(param);
	std::cout << "lower_bound: " << (it[0] == ite ? "end()" : print_pair(it[0], false)) << std::endl;
	std::cout << "upper_bound: " << (it[1] == ite ? "end()" : print_pair(it[1], false)) << std::endl;
	std::cout << "equal_range: " << (ft_range.first == it[0] && ft_range.second == it[1]) << std::endl;
}

int main() {

	std::cout << "// --> My tests <-- //" << std::endl << std::endl;

	std::cout << "// --> Map <-- //" << std::endl << std::endl;

	ft::pair<char, int> b = ft::make_pair('b', 5);
	ft::pair<char, int> c = ft::make_pair('c', 3);
	ft::pair<char, int> d = ft::make_pair('d', 15);
	ft::pair<char, int> f = ft::make_pair('f', 6);
	ft::pair<char, int> l = ft::make_pair('l', -1);

	ft::map<char, int> tree;

	tree.insert(ft::make_pair('a', 1));
	tree.insert(ft::make_pair('c', 4));
	tree.insert(b);
	std::cout << std::endl;
	tree.insert(c);
	tree.insert(d);
	tree.insert(f);
	std::cout << "tree.begin()->first" << std::endl; 
	std::cout << tree.begin()->first << std::endl; 
	tree.insert(tree.begin(), l);
	std::cout << "tree.begin()->first" << std::endl; 
	std::cout << tree.begin()->first << std::endl;

	std::cout << std::endl;

	tree.erase('c');
	tree.erase('d');
	tree.erase('f');

	std::cout << (tree.find('c'))->first << std::endl;
	std::cout << (tree.find('a'))->first << std::endl;

	ft::map<char, int> lemontree(tree);

	ft::map<char, int>::iterator it = tree.begin();
	std::cout << it->first << std::endl;
	std::cout << &it << std::endl;
	it++;
	it++;
	std::cout << it->first << std::endl;
	it--;
	std::cout << it->first << std::endl;
	it--;
	std::cout << it->first << std::endl;

	std::cout << tree.at('a') << std::endl;
	try {
		std::cout << tree.at('c') << std::endl;
	}
	catch (std::out_of_range &e){
		std::cout << e.what() << std::endl;
	}
	std::cout << tree['a'] << std::endl;
	std::cout << tree['r'] << std::endl;

	ft::map<int, std::string> mp;

	mp.insert(ft::make_pair<int, std::string>(1, "American Elm"));
	mp.insert(ft::make_pair<int, std::string>(15, "Yellow Birch"));
	mp.insert(ft::make_pair<int, std::string>(4, "Butternut"));
	mp.insert(ft::make_pair<int, std::string>(71, "Black Cherry"));
	mp.insert(ft::make_pair<int, std::string>(79, "Pin Cherry"));
	mp.insert(ft::make_pair<int, std::string>(44, "American Chestnut"));
	mp.insert(ft::make_pair<int, std::string>(35, "Cucumber Tree"));
	mp.insert(ft::make_pair<int, std::string>(15, "Eastern Cottonwood"));
	mp.insert(ft::make_pair<int, std::string>(12, "Arborvitae"));
	mp.insert(ft::make_pair<int, std::string>(11, "Black Ash"));
	mp.insert(ft::make_pair<int, std::string>(89, "Paper Birch"));
	mp.insert(ft::make_pair<int, std::string>(78, "Basswood"));
	mp.insert(ft::make_pair<int, std::string>(28, "Gray Birch"));
	ft::map<int, std::string> cherrytree(mp.begin(), mp.end());
	print_tree(cherrytree);

	ft_const_bound(cherrytree, -10);
	ft_const_bound(cherrytree, 1);
	ft_const_bound(cherrytree, 5);
	ft_const_bound(cherrytree, 10);
	ft_const_bound(cherrytree, 50);

	cherrytree.lower_bound(3)->second = "Balsam Fir";
	cherrytree.upper_bound(7)->second = "Slippery Elm";
	ft_bound(cherrytree, 5);
	ft_bound(cherrytree, 7);

	print_tree(cherrytree);
}
