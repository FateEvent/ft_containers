#include <iostream>
#include <string>
#include <vector>
#if 0 //CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
	#include <map.hpp>
	#include <set.hpp>
	#include <stack.hpp>
	#include <vector.hpp>
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};

#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs) 
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

int main(int argc, char** argv) {
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed);

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
	ft::stack<Buffer, std::vector<Buffer> > stack_deq_buffer;
//	ft::map<int, int> map_int;
/*
	for (int i = 0; i < COUNT; i++)
		vector_buffer.push_back(Buffer());

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cerr << vector_buffer.size() <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}
*/
/*	for (int i = 0; i < COUNT; ++i)
		map_int.insert(ft::make_pair(rand(), rand()));

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand();
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;
		ft::map<int, int> copy = map_int;
*/

	// --> My tests <-- //
	std::cout << "// --> My tests <-- //" << std::endl << std::endl;
/*
	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
		std::cout << *it;
	std::cout << std::endl;

	MutantStack<char>::iterator itLetter = iterable_stack.begin();
	MutantStack<char>::iterator itLetter2 = iterable_stack.end();

	ft::vector<char>	newVector(itLetter, itLetter2);
	for (ft::vector<char>::iterator it = newVector.begin(); it != newVector.end(); it++)
		std::cout << "iterator: " << *it << std::endl;
	newVector.push_back('j');
	for (ft::vector<char>::iterator it = newVector.end() - 2; it != newVector.end(); it++)
		std::cout << "last letters: " << *it << std::endl;

	std::cout << "The maximum storage capacity of the container is " << newVector.max_size() << std::endl;

	ft::vector<char>	vecCopy(newVector);

	std::cout << "What vecCopy contains:" << std::endl;
	for (ft::vector<char>::iterator it = vecCopy.begin(); it != vecCopy.end(); it++)
		std::cout << *it << std::endl;

	vecCopy.assign(4, 'a');
	std::cerr << "size and capacity: " << vecCopy.size() << ", " << vecCopy.capacity() << std::endl;
	std::cout << "What vecCopy contains:" << std::endl;
	for (ft::vector<char>::iterator it = vecCopy.begin(); it != vecCopy.end(); it++)
		std::cout << *it << std::endl;

	newVector.reserve(45);
	newVector.assign(vecCopy.begin(), vecCopy.end());
	newVector.insert((newVector.begin() + 2), 'z');
	newVector.push_back('c');
	std::cerr << "size and capacity: " << newVector.size() << ", " << newVector.capacity() << std::endl;
	std::cout << "What newVector contains:" << std::endl;
	for (ft::vector<char>::iterator it = newVector.begin(); it != newVector.end(); it++)
		std::cout << *it << std::endl;
	vecCopy.push_back('c');
	vecCopy.push_back('c');
	vecCopy.push_back('c');
	vecCopy.push_back('c');
	vecCopy.insert(vecCopy.begin() + 2, itLetter, itLetter2);
	std::cerr << "size and capacity: " << vecCopy.size() << ", " << vecCopy.capacity() << std::endl;
	std::cout << "What vecCopy contains:" << std::endl;
	for (ft::vector<char>::iterator it = vecCopy.begin(); it != vecCopy.begin() + vecCopy.size(); it++)
		std::cout << *it << std::endl;

	vecCopy.insert(vecCopy.begin() + 2, 5, 'Z');
	std::cerr << "size and capacity: " << vecCopy.size() << ", " << vecCopy.capacity() << std::endl;
	std::cout << "What the copy contains:" << std::endl;
	for (ft::vector<char>::iterator it = vecCopy.begin(); it != vecCopy.begin() + vecCopy.size(); it++)
		std::cout << *it << std::endl;

	vecCopy.insert(vecCopy.begin() + 2, 'x');
	std::cout << "return: " << *(vecCopy.erase(vecCopy.begin() + 1, vecCopy.begin() + 7)) << std::endl;
	std::cout << "return: " << *(vecCopy.erase(vecCopy.begin() + 1)) << std::endl;
	vecCopy.resize(35);
	std::cout << "What the copy contains now:" << std::endl;
	for (ft::vector<char>::iterator it = vecCopy.begin(); it != vecCopy.begin() + vecCopy.size(); it++)
		std::cout << *it << std::endl;

	vecCopy.swap(newVector);
	std::cout << "What the vector contains now:" << std::endl;
	for (ft::vector<char>::iterator it = newVector.begin(); it != newVector.begin() + newVector.size(); it++)
		std::cout << *it << std::endl;
	std::cout << "What the copy contains now:" << std::endl;
	for (ft::vector<char>::iterator it = vecCopy.begin(); it != vecCopy.begin() + vecCopy.size(); it++)
		std::cout << *it << std::endl;

	ft::vector<char>	newDeck(itLetter, itLetter2);
	std::cerr << "size and capacity: " << newDeck.size() << ", " << newDeck.capacity() << std::endl;
	newDeck.resize(53);
	std::cout << "What the deck contains:" << std::endl;
	for (ft::vector<char>::iterator it = newDeck.begin(); it != newDeck.end(); it++)
		std::cout << *it << std::endl;
	newDeck.pop_front();
	newDeck.push_front('x');
	std::cerr << "size and capacity: " << newDeck.size() << ", " << newDeck.capacity() << std::endl;
	std::cout << "What the deck contains now:" << std::endl;
	for (ft::vector<char>::iterator it = newDeck.begin(); it != newDeck.end(); it++)
		std::cout << *it << std::endl;
*/
/*
	ft::pair<char, int> b = ft::make_pair('b', 5);
	ft::pair<char, int> c = ft::make_pair('c', 3);
	ft::pair<char, int> d = ft::make_pair('d', 15);
	ft::pair<char, int> f = ft::make_pair('f', 6);
	ft::pair<char, int> l = ft::make_pair('l', -1);

	ft::map<char, int> tree;

	tree.insert(tree.begin(), ft::make_pair('a', 1));
	std::cout << (tree.begin())->first << std::endl;
	tree.insert(ft::make_pair('c', 4));
	std::cout << (tree.begin())->first << std::endl;
	tree.print_tree();
	tree.insert(b);
	tree.print_tree();
	std::cout << std::endl;
	tree.insert(c);
	tree.insert(d);
	tree.insert(f);
	tree.insert(l);

	tree.level_order_traversal(tree.protoroot(), tree.print_node);

	std::cout << std::endl;

	tree.delete_tree_node('c');
	tree.print_tree();
	tree.delete_tree_node('d');
	std::cout << tree.root()->height() << std::endl;
	tree.print_tree();
	tree.delete_tree_node('f');
	tree.print_tree();

	std::cout << (tree.find('c'))->first << std::endl;
	std::cout << (tree.find('a'))->first << std::endl;

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
	catch (ContainerException& e){
		std::cout << e.what() << std::endl;
	}
	std::cout << tree['a'] << std::endl;
	std::cout << tree['r'] << std::endl;
*/

	ft::pair<char, int> b = ft::make_pair('b', 5);
	ft::pair<char, int> c = ft::make_pair('c', 3);
	ft::pair<char, int> d = ft::make_pair('d', 15);
	ft::pair<char, int> f = ft::make_pair('f', 6);
	ft::pair<char, int> l = ft::make_pair('l', -1);

	ft::set<char, int> tree;

	tree.insert(tree.begin(), ft::make_pair('a', 1));
	std::cout << (tree.begin())->first << std::endl;
	tree.insert(ft::make_pair('c', 4));
	std::cout << (tree.begin())->first << std::endl;
	tree.print_tree();
	tree.insert(b);
	tree.print_tree();
	std::cout << std::endl;
	tree.insert(c);
	tree.insert(d);
	tree.insert(f);
	tree.insert(l);

	tree.print_tree();
//	tree.level_order_traversal(tree.protoroot(), tree.print_node);

	std::cout << std::endl;

	tree.delete_tree_node('c');
	tree.print_tree();
	tree.delete_tree_node('d');
	std::cout << tree.root()->height() << std::endl;
	tree.print_tree();
	tree.delete_tree_node('f');
	tree.print_tree();

//	std::cout << (tree.find('c'))->first << std::endl;
//	std::cout << (tree.find('a'))->first << std::endl;

	ft::set<char, int>::iterator it = tree.begin();
	std::cout << it->first << std::endl;
	std::cout << &it << std::endl;
	it++;
	it++;
	std::cout << it->first << std::endl;
	it--;
	std::cout << it->first << std::endl;
	it--;
	std::cout << it->first << std::endl;

//	std::cout << tree.at('a') << std::endl;
//	try {
//		std::cout << tree.at('c') << std::endl;
//	}
//	catch (ContainerException& e){
//		std::cout << e.what() << std::endl;
//	}
//	std::cout << tree['a'] << std::endl;
//	std::cout << tree['r'] << std::endl;

}
