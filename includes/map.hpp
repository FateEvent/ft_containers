#ifndef MAP_H
# define MAP_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "pair.hpp"
# include "utilities.hpp"
# include "_Rb_tree_node.hpp"
# include "_Rb_tree.hpp"
# include "_Rb_iterator.hpp"

namespace ft
{
	template<class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map {
	public:
		class value_compare;
		typedef Key													key_type;
		typedef T													mapped_type;
		typedef pair<const key_type, mapped_type>					value_type;
		typedef Compare												key_compare;
		typedef Allocator											allocator_type;
		typedef typename allocator_type::reference					reference;
		typedef typename allocator_type::const_reference			const_reference;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer				const_pointer;
		typedef typename allocator_type::size_type					size_type;
		typedef typename allocator_type::difference_type			difference_type;

		typedef	Node<value_type>									tree_node;
		typedef RBTree<value_type, value_compare, allocator_type>	Rb_tree;
		typedef typename Rb_tree::iterator							iterator;
		typedef typename Rb_tree::const_iterator					const_iterator;
		typedef typename Rb_tree::reverse_iterator					reverse_iterator;
		typedef typename Rb_tree::const_reverse_iterator			const_reverse_iterator;

		class value_compare
			: public std::binary_function<value_type, value_type, bool>
		{
			friend class map;

		protected:
			key_compare	comp;

			value_compare(key_compare c) : comp(c) {};

		public:
			bool operator()(const value_type& lhs, const value_type& rhs) const { return comp(lhs.first, rhs.first); }
		};

	public:
		/****************************************************************************\
		**								Member functions							**
		\****************************************************************************/

		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _tree(comp, alloc), _alloc_pair(alloc), _key_comp(comp) {}

		template <class InputIterator>
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _tree(comp, alloc), _alloc_pair(alloc), _key_comp(comp) {
			
			_tree.insert(first, last);
		}

		map(const map& other)
			: _tree(other._tree), _alloc_pair(other._alloc_pair), _key_comp(other._key_comp) {}

		~map() {}

		map	&operator=(const map &other)
		{
			if (this != &other)
			{
				_tree = other._tree;
				_key_comp = other._key_comp;
				_alloc_pair = other._alloc_pair;
			}
			return (*this);
		}

		allocator_type	get_allocator() const { return _alloc_pair; }

		/****************************************************************************\
		**								Element access								**
		\****************************************************************************/

		mapped_type&	at(const key_type& k)
		{
			tree_node	*found = _tree.search(_tree.root(), ft::make_pair(k, mapped_type()));
			if (found)
				return (found->data.second);
			throw(std::out_of_range("map"));
		}

		const mapped_type&	at(const key_type& k) const
		{
			return (const_cast<mapped_type>(at((k))));
		}

		mapped_type&	operator[] (const key_type& k)
		{
			return ((*(insert(ft::make_pair(k, mapped_type())).first)).second);
		}

		/****************************************************************************\
		**									Iterators								**
		\****************************************************************************/

		iterator	begin()	{ return (_tree.begin()); }

		const_iterator	begin() const { return (_tree.begin()); }

		iterator	end() { return (_tree.end()); }

		const_iterator	end() const { return (_tree.end()); }

		reverse_iterator rbegin() { return (_tree.rbegin()); }
		
		const_reverse_iterator rbegin() const { return (_tree.rbegin()); }

		reverse_iterator rend() { return (_tree.rend()); }
		
		const_reverse_iterator rend() const { return (_tree.rend()); }

		/****************************************************************************\
		**									Capacity								**
		\****************************************************************************/

		bool empty() const { return (_tree.empty()); }

		size_type	size() const { return (_tree.size()); }

		size_type	max_size() const { return (_tree.max_size()); }

		/****************************************************************************\
		**									Modifiers								**
		\****************************************************************************/

		void	clear() {
			_tree.clear(_tree.root());
		}

		pair<iterator, bool>	insert(const value_type& val)
		{
			return (_tree.insert(val));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			return (_tree.insert(pos, val));
		}

		template<class InputIt>
		void	insert(InputIt first, InputIt last,
			typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::value* = 0)
		{
			_tree.insert(first, last);
		}

		void	erase(iterator pos)
		{
			_tree.erase(pos.base());
		}

		void	erase(iterator first, iterator last)
		{
			_tree.erase(first, last);
		}

		size_type	erase(const key_type& key)
		{
			return (_tree.erase(ft::make_pair(key, mapped_type())));
		}

		void	swap(map& other)
		{
			_tree.swap(other._tree);
		}

		/****************************************************************************\
		**									Lookup									**
		\****************************************************************************/

		size_type	count( const key_type& key ) const {
			tree_node	*found(_tree.search(_tree.root(), ft::make_pair(key, mapped_type())));

			if (found)
				return (1);
			return (0);
		}

		iterator	find(const key_type& key) {
			return (_tree.find(ft::make_pair(key, mapped_type())));
		}

		const_iterator	find (const key_type& key) const {
			return (_tree.find(ft::make_pair(key, mapped_type())));
		}

		ft::pair<iterator,iterator>	equal_range(const key_type& key) {
			return (ft::make_pair(lower_bound(key), upper_bound(key)));
		}

		ft::pair<const_iterator,const_iterator>	equal_range(const key_type& key) const {
			return (ft::make_pair(lower_bound(key), upper_bound(key)));
		}

		iterator		lower_bound(const key_type& key) {
			iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (!_key_comp(it->first, key))
					break ;
				it++;
			}
			return (it);
		}

		const_iterator	lower_bound(const key_type& key) const {
			const_iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (!_key_comp(it->first, key))
					break ;
				it++;
			}
			return (it);
		}

		iterator		upper_bound(const key_type& key) {
			iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (_key_comp(key, it->first))
					break ;
				it++;
			}
			return (it);
		}

		const_iterator	upper_bound(const key_type& key) const {
			const_iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (_key_comp(key, it->first))
					break ;
				it++;
			}
			return (it);
		}

		/****************************************************************************\
		**									Observers								**
		\****************************************************************************/

		key_compare	key_comp() const { return _key_comp; };

		value_compare	value_comp() const { return value_compare(_key_comp); }

	private :

		value_type	get_value_type(const key_type &key) const
		{
			return (ft::make_pair(key, mapped_type()));
		}

		Rb_tree			_tree;
		allocator_type	_alloc_pair;
		key_compare		_key_comp;
	};

	template <class Key, class T, class Compare, class Alloc>
	bool	operator==(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator!=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (!(rhs < lhs));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (rhs < lhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		return (!(lhs < rhs));
	}

	template <class Key, class T, class Compare, class Alloc>
	void	swap(map<Key, T, Compare, Alloc> &lhs, map<Key, T, Compare, Alloc> &rhs)
	{
		lhs.swap(rhs);
	}
}

#endif