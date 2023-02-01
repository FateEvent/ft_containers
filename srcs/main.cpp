#include <iostream>
#include <string>
#include <vector>
#include "chrono.hpp"
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

#include <stdlib.h>
// added for testing
#include <list>

// --- Class foo
template <typename T>
class foo {
	public:
		typedef T	value_type;

		foo(void) : value(), _verbose(false) { };
		foo(value_type src, const bool verbose = false) : value(src), _verbose(verbose) { };
		foo(foo const &src, const bool verbose = false) : value(src.value), _verbose(verbose) { };
		~foo(void) { if (this->_verbose) std::cout << "~foo::foo()" << std::endl; };
		void m(void) { std::cout << "foo::m called [" << this->value << "]" << std::endl; };
		void m(void) const { std::cout << "foo::m const called [" << this->value << "]" << std::endl; };
		foo &operator=(value_type src) { this->value = src; return *this; };
		foo &operator=(foo const &src) {
			if (this->_verbose || src._verbose)
				std::cout << "foo::operator=(foo) CALLED" << std::endl;
			this->value = src.value;
			return *this;
		};
		value_type	getValue(void) const { return this->value; };
		void		switchVerbose(void) { this->_verbose = !(this->_verbose); };

		operator value_type(void) const {
			return value_type(this->value);
		}
	private:
		value_type	value;
		bool		_verbose;
};
template <typename T>
std::string	printPair(const T &iterator, bool nl = true, std::ostream &o = std::cout)
{
	o << "key: " << iterator->first << " | value: " << iterator->second;
	if (nl)
		o << std::endl;
	return ("");
}

template <typename T>
std::ostream	&operator<<(std::ostream &o, foo<T> const &bar) {
	o << bar.getValue();
	return o;
}
// --- End of class foo

template <typename T_MAP>
void	printSize(T_MAP const &mp, bool print_content = 1)
{
	std::cout << "size: " << mp.size() << std::endl;
	std::cout << "max_size: " << mp.max_size() << std::endl;
	if (print_content)
	{
		typename T_MAP::const_iterator it = mp.begin(), ite = mp.end();
		std::cout << std::endl << "Content is:" << std::endl;
		for (; it != ite; ++it)
			std::cout << "- " << printPair(it, false) << std::endl;
	}
	std::cout << "###############################################" << std::endl;
}

template <typename T1, typename T2>
void	printReverse(ft::map<T1, T2> &mp)
{
	typename ft::map<T1, T2>::iterator it = mp.end(), ite = mp.begin();

	std::cout << "printReverse:" << std::endl;
	while (it != ite) {
		it--;
		std::cout << "-> " << printPair(it, false) << std::endl;
	}
	std::cout << "_______________________________________________" << std::endl;
}

#define _pair ft::pair
#define T1 int
#define T2 foo<int>
typedef ft::map<T1, T2>::value_type T3;
typedef ft::map<T1, T2>::iterator ft_iterator;
typedef ft::map<T1, T2>::const_iterator ft_const_iterator;

static int iter = 0;

template <typename MAP>
void	ft_bound(MAP &mp, const T1 &param)
{
	ft_iterator ite = mp.end(), it[2];
	_pair<ft_iterator, ft_iterator> ft_range;

	std::cout << "\t-- [" << iter++ << "] --" << std::endl;
	std::cout << "with key [" << param << "]:" << std::endl;
	it[0] = mp.lower_bound(param); it[1] = mp.upper_bound(param);
	ft_range = mp.equal_range(param);
	std::cout << "lower_bound: " << (it[0] == ite ? "end()" : printPair(it[0], false)) << std::endl;
	std::cout << "upper_bound: " << (it[1] == ite ? "end()" : printPair(it[1], false)) << std::endl;
	std::cout << "equal_range: " << (ft_range.first == it[0] && ft_range.second == it[1]) << std::endl;
}

template <typename MAP>
void	ft_const_bound(const MAP &mp, const T1 &param)
{
	ft_const_iterator ite = mp.end(), it[2];
	_pair<ft_const_iterator, ft_const_iterator> ft_range;

	std::cout << "\t-- [" << iter++ << "] (const) --" << std::endl;
	std::cout << "with key [" << param << "]:" << std::endl;
	it[0] = mp.lower_bound(param); it[1] = mp.upper_bound(param);
	ft_range = mp.equal_range(param);
	std::cout << "lower_bound: " << (it[0] == ite ? "end()" : printPair(it[0], false)) << std::endl;
	std::cout << "upper_bound: " << (it[1] == ite ? "end()" : printPair(it[1], false)) << std::endl;
	std::cout << "equal_range: " << (ft_range.first == it[0] && ft_range.second == it[1]) << std::endl;
}



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
	ft::map<int, int> map_int;

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

	for (int i = 0; i < COUNT; ++i)
		map_int.insert(ft::make_pair(rand(), rand()));

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand();
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;
		ft::map<int, int> copy = map_int;


	std::cout << "// --> My tests <-- //" << std::endl << std::endl;

	timeval	start;
	gettimeofday(&start, NULL);

	std::cout << "// --> Vector <-- //" << std::endl << std::endl;

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

	std::list<T3> lst;
	unsigned int lst_size = 10;
	for (unsigned int i = 0; i < lst_size; ++i)
		lst.push_back(T3(i + 1, (i + 1) * 3));
	ft::map<T1, T2> mp(lst.begin(), lst.end());
	printSize(mp);

	ft_const_bound(mp, -10);
	ft_const_bound(mp, 1);
	ft_const_bound(mp, 5);
	ft_const_bound(mp, 10);
	ft_const_bound(mp, 50);

	mp.lower_bound(3)->second = 404;
	mp.upper_bound(7)->second = 842;
	ft_bound(mp, 5);
	ft_bound(mp, 7);

	printSize(mp);

	timeval	end;
	gettimeofday(&end, NULL);

	timerDisplay(&start, &end);
}
