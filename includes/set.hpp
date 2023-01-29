#ifndef SET_H
# define SET_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "utilities.hpp"
# include "_Rb_tree_node.hpp"
# include "_Rb_tree.hpp"
# include "_Rb_iterator.hpp"

namespace ft
{
	template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key> >
	class set {
	public:
		typedef Key													key_type;
		typedef Key													value_type;
		typedef Compare												key_compare;
		typedef Compare												value_compare;
		typedef Allocator											allocator_type;
		typedef value_type&											reference;
		typedef const value_type&									const_reference;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer				const_pointer;
		typedef typename allocator_type::size_type					size_type;
		typedef typename allocator_type::difference_type			difference_type;

		typedef	Node<value_type>									tree_node;
		typedef RBTree<value_type, value_compare, allocator_type>	rb_tree;
		typedef typename rb_tree::const_iterator					iterator;
		typedef typename rb_tree::const_iterator					const_iterator;
		typedef ft::reverse_iterator<iterator>						reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;

	public:
		/****************************************************************************\
		**								Member functions							**
		\****************************************************************************/

		explicit set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _tree(comp, alloc), _alloc(alloc), _key_comp(comp) {}

		template <class InputIterator>
		set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _tree(comp, alloc), _alloc(alloc), _key_comp(comp) {
			
			_tree.insert(first, last);
		}

		set(const set& other)
			: _tree(other._tree), _alloc(other._alloc), _key_comp(other._key_comp) {}

		~set() {}

		set	&operator=(const set &other)
		{
			if (this != &other)
			{
				_tree = other._tree;
				_key_comp = other._key_comp;
				_alloc = other._alloc;
			}
			return (*this);
		}

		allocator_type	get_allocator() const { return _alloc; }

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
			_tree.erase(*pos);
		}

		void	erase(iterator first, iterator last)
		{
			_tree.erase(first, last);
		}

		size_type	erase(const key_type& key)
		{
			return (_tree.erase(key));
		}

		void	swap(set& other)
		{
			_tree.swap(other._tree);
		}

		/****************************************************************************\
		**									Lookup									**
		\****************************************************************************/

		size_type	count( const key_type& key ) const {
			tree_node	*found(_tree.search(_tree.root(), key));

			if (found)
				return (1);
			return (0);
		}

		iterator	find(const key_type& key) {
			return (_tree.find(key));
		}

		const_iterator	find (const key_type& key) const {
			return (_tree.find(key));
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
				if (!_key_comp(*it, key))
					break ;
				it++;
			}
			return (it);
		}

		const_iterator	lower_bound(const key_type& key) const {
			const_iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (!_key_comp(*it, key))
					break ;
				it++;
			}
			return (it);
		}

		iterator		upper_bound(const key_type& key) {
			iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (_key_comp(key, *it))
					break ;
				it++;
			}
			return (it);
		}

		const_iterator	upper_bound(const key_type& key) const {
			const_iterator	it(begin()), ite(end());

			while (it != ite)
			{
				if (_key_comp(key, *it))
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
		rb_tree			_tree;
		allocator_type	_alloc;
		key_compare		_key_comp;
	};

	template <class Key, class Compare, class Alloc>
	bool	operator==(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class Key, class Compare, class Alloc>
	bool	operator!=(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <class Key, class Compare, class Alloc>
	bool	operator<(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class Key, class Compare, class Alloc>
	bool	operator<=(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs)
	{
		return (!(rhs < lhs));
	}

	template <class Key, class Compare, class Alloc>
	bool	operator>(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs)
	{
		return (rhs < lhs);
	}

	template <class Key, class Compare, class Alloc>
	bool	operator>=(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs)
	{
		return (!(lhs < rhs));
	}

	template <class Key, class Compare, class Alloc>
	void	swap(set<Key, Compare, Alloc> &lhs, set<Key, Compare, Alloc> &rhs)
	{
		lhs.swap(rhs);
	}
}

#endif