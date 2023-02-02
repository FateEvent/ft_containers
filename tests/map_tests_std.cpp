#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>

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

#define _pair std::pair
typedef std::map<int, std::string>::iterator ft_iterator;
typedef std::map<int, std::string>::const_iterator ft_const_iterator;

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

	std::pair<char, int> b = std::make_pair('b', 5);
	std::pair<char, int> c = std::make_pair('c', 3);
	std::pair<char, int> d = std::make_pair('d', 15);
	std::pair<char, int> f = std::make_pair('f', 6);
	std::pair<char, int> l = std::make_pair('l', -1);

	std::map<char, int> tree;

	tree.insert(std::make_pair('a', 1));
	tree.insert(std::make_pair('c', 4));
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

	tree.erase('c');
	tree.erase('d');
	tree.erase('f');

	std::map<char, int>::iterator	it = tree.find('c');
	if (it != tree.end())
		std::cout << it->first << std::endl;
	
	it = tree.find('a');
	if (it != tree.end())
		std::cout << it->first << std::endl;
	
	it = tree.find('a');
	if (it != tree.end())
		std::cout << it->first << std::endl;

	std::map<char, int> lemontree(tree);

	print_tree(lemontree);

	std::cout << tree.at('a') << std::endl;
	try {
		std::cout << tree.at('c') << std::endl;
	}
	catch (std::out_of_range &e){
		std::cout << e.what() << std::endl;
	}
	std::cout << tree['a'] << std::endl;
	std::cout << tree['r'] << std::endl;

	std::map<int, std::string>	mp;

	mp.insert(std::make_pair<int, std::string>(1, "American Elm"));
	mp.insert(std::make_pair<int, std::string>(15, "Yellow Birch"));
	mp.insert(std::make_pair<int, std::string>(4, "Butternut"));
	mp.insert(std::make_pair<int, std::string>(71, "Black Cherry"));
	mp.insert(std::make_pair<int, std::string>(79, "Pin Cherry"));
	mp.insert(std::make_pair<int, std::string>(44, "American Chestnut"));
	mp.insert(std::make_pair<int, std::string>(35, "Cucumber Tree"));
	mp.insert(std::make_pair<int, std::string>(15, "Eastern Cottonwood"));
	mp.insert(std::make_pair<int, std::string>(12, "Arborvitae"));
	mp.insert(std::make_pair<int, std::string>(11, "Black Ash"));
	mp.insert(std::make_pair<int, std::string>(89, "Paper Birch"));
	mp.insert(std::make_pair<int, std::string>(78, "Basswood"));
	mp.insert(std::make_pair<int, std::string>(28, "Gray Birch"));
	std::map<int, std::string> cherrytree(mp.begin(), mp.end());
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

	cherrytree.erase(cherrytree.begin(), cherrytree.end());
	print_tree(cherrytree);
	std::cout << cherrytree.empty() << std::endl;

	mp.clear();
	print_tree(mp);
	std::cout << mp.empty() << std::endl;
}
