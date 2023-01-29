#ifndef SET
# define SET

# include <iostream>
# include <string>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "pair.hpp"
# include "vector.hpp"
# include "map_iterator.hpp"
# include "_Rbtree_node.hpp"
#include <set>

class set_iterator;

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>,
	class Allocator = std::allocator<ft::pair<const Key, T> > >
	class set {
	public:
		typedef Key																key_type;
		typedef T																mapped_type;
		typedef pair<const key_type, mapped_type>								value_type;
		typedef Compare															key_compare;
		typedef Allocator														allocator_type;
		typedef	Node<value_type>												tree_node;
		typedef typename allocator_type::reference								reference;
		typedef typename allocator_type::const_reference						const_reference;
		typedef typename allocator_type::pointer								pointer;
		typedef typename allocator_type::const_pointer							const_pointer;
		typedef typename allocator_type::size_type								size_type;
		typedef typename allocator_type::difference_type						difference_type;
		typedef typename Allocator:: template rebind<Node>::other				node_allocator;
		typedef typename node_allocator::pointer								node_pointer;

		typedef ft::wrapper_it<ft::map_iterator<tree_node, value_type> >		iterator;
		typedef ft::wrapper_it<ft::map_iterator<tree_node, const value_type> >	const_iterator;
		typedef ft::reverse_iterator<iterator>									reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>							const_reverse_iterator;

		class value_compare
			: public std::binary_function<value_type, value_type, bool>
		{
			friend class set;

		protected:
			key_compare	comp;

			value_compare(key_compare c);

		public:
			bool operator()(const value_type& x, const value_type& y) const { return x == y; }
		};

		friend std::ostream	&operator<< (std::ostream &o, const Node &node) {
			o << "key: " << node.data().first;
			o << ", value: " << node.data().second;
			o << ", height: " << node.height() << std::endl;
			return (o);
		}

	public:
		explicit set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size(), _black_height() {
				_root = new_node();
		}

		template <class InputIterator>
		set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
		set(const set& x);

		~set() { delete_node(protoroot()); }
		set&	operator= (set const& base);
		tree_node	*protoroot() { return (_root); }
		tree_node	*dummy() { return (protoroot()); }
		tree_node	*root() { return (_root->right()); }
		void	set_root(tree_node *current) { _root->set_right(current); }

		void	prefix_traversal(tree_node *current, char sep) {
			if (current)
			{
				current->treat(sep);
				prefix_traversal(current->left(), sep);
				prefix_traversal(current->right(), sep);
			}
		}

		void	infix_traversal(tree_node *current, char sep) {
			if (current)
			{
				infix_traversal(current->left(), sep);
				current->treat(sep);
				infix_traversal(current->right(), sep);
			}
		}

		void	suffix_traversal(tree_node *current, void (*f)(tree_node *)) {
			if (current)
			{
				suffix_traversal(current->left(), f);
				suffix_traversal(current->right(), f);
				f(current);
			}
		}

		void	level_order_traversal(tree_node *current, void (*f)(tree_node *)){
			vector<tree_node *>	deck;

			deck.push_back(current);
			while (!deck.empty())
			{
				current = deck.front();
				deck.pop_front();
				f(current);
				if (current->left())
					deck.push_back(current->left());
				if (current->right())
					deck.push_back(current->right());
			}
		}

		static void	print_node(tree_node *node)
		{
			if (node)
			{
				std::cout << "key: " << node->data().first;
				std::cout << ", value: " << node->data().second;
				std::cout << ", colour: " << node->colour();
				std::cout << ", height: " << node->height() << std::endl;
			}
			else
				std::cout << "NULL pointer" << std::endl;
		}

		void	print_tree()
		{
			std::cout << "-- Here's my Red-Black tree: --" << std::endl;
			level_order_traversal(root(), print_node);
			std::cout << "-- Here my tree ends. Tschüss. --" << std::endl;
		}

		iterator	begin()	{
			if (root())
			{
				iterator it(root());

				it.leftmost();
				return (it);
			}
			iterator it(protoroot());
			return (it);
		}

		const_iterator	begin() const {
			if (root())
			{
				const_iterator it(root());

				it.leftmost();
				return (it);
			}
			const_iterator it(protoroot());
			return (it);
		}

		iterator	end() {
			if (root())
			{
				iterator it(root());

				it.rightmost();
				return (it);
			}
			iterator it(protoroot());
			return (it);
		}

		const_iterator	end() const {
			if (root())
			{
				const_iterator it(root());

				it.rightmost();
				return (it);
			}
			const_iterator it(protoroot());
			return (it);
		}

		iterator	find(const key_type& k)
		{
			tree_node *found = _recursive_Rb_tree_search(root(), k);
			if (found)
				return (iterator(found));
			return (end());
		}
//		const_iterator	find (const key_type& k) const;

		tree_node	*new_node(const value_type& pair = value_type(), tree_node *parent = NULL)
		{
			tree_node *ptr = _alloc_node.allocate(1);
//			_alloc_node.construct(ptr);
			try
			{
				_alloc_pair.construct(&ptr->_data, pair);
			}
			catch (...)
			{
				_alloc_node.deallocate(ptr, 1);
				throw;
			}
			ptr->_left = NULL;
			ptr->_right = NULL;
			ptr->_parent = parent;
			ptr->_height = 1;
			ptr->_colour = "red";
			return (ptr);
		}

		void	delete_node(tree_node *node)
		{
			_alloc_node.destroy(node);
			_alloc_node.deallocate(node, 1);
		}

		/****************************************************************************\
		** 																			**
		**					Utility functions for Red-Black trees					**
		** 																			**
		\****************************************************************************/

		tree_node	*_iterative_Rb_tree_search(const key_type& k)
		{
			tree_node	*current(root());
			tree_node	*prev(root());

			while (current)
			{
				if (k == current->data().first)
					return (current);
				else if (_key_comp(k, current->data().first))
				{
					prev = current;
					current = current->left();
				}
				else
				{
					prev = current;
					current = current->right();
				}
			}
			return (prev);
		}

		tree_node	*_recursive_Rb_tree_search(tree_node *node, const key_type& k)
		{
			if (node == NULL)
				return (NULL);
			else if (node->data().first == k)
				return (node);
			else if (_key_comp(k, node->data().first))
			{
				tree_node *temp = _recursive_Rb_tree_search(node->left(), k);
				return (temp);
			}
			else
			{
				tree_node *temp = _recursive_Rb_tree_search(node->right(), k);
				return (temp);
			}
		}

		void	_Rb_tree_transplant(tree_node *u, tree_node *v)
		{
			if (u->parent() == NULL)
				set_root(v);
			else if (u->parent()->left() == u)
				u->parent()->set_left(v);
			else
				u->parent()->set_right(v);
			if (v != NULL)
				v->set_parent(u->parent());
		}

		tree_node	*_Rb_tree_successor(tree_node *node)
		{
			if (node->right() != NULL)
			{
				node = node->right();
				while(node->left() != NULL)
					node = node->left();
				return (node);
			}
			else
			{
				while(node->parent() != NULL && node->parent()->right() == node)
					node = node->parent();
				return (node->parent());
			}
		}

		void fix_double_black_conflicts(tree_node *x) {
			if (x == root())
				return ;
			tree_node *sibling = x->get_sibling(), *parent = x->parent();
			if (sibling == NULL) {
				fix_double_black_conflicts(parent);
			} else {
				if (sibling->colour() == "red") {
					parent->set_colour("red");
					sibling->set_colour("black");
					if (sibling->is_left_child()) {
						_Rb_tree_right_rotation(parent);
					} else {
						_Rb_tree_left_rotation(parent);
					}
					fix_double_black_conflicts(x);
				} else {
					if (sibling->has_red_child()) {
						if (sibling->left() != NULL and sibling->left()->colour() == "red") {
							if (sibling->is_left_child()) {
								sibling->left()->set_colour(sibling->colour());
								sibling->set_colour(parent->colour());
								_Rb_tree_right_rotation(parent);
							} else {
								sibling->left()->set_colour(parent->colour());
								_Rb_tree_right_rotation(sibling);
								_Rb_tree_left_rotation(parent);
							}
						} else {
							if (sibling->is_left_child()) {
								sibling->right()->set_colour(parent->colour());
								_Rb_tree_left_rotation(sibling);
								_Rb_tree_right_rotation(parent);
							} else {
								sibling->right()->set_colour(sibling->colour());
								sibling->set_colour(parent->colour());
								_Rb_tree_left_rotation(parent);
							}
						}
						parent->set_colour("black");
					} else {
						sibling->set_colour("red");
						if (parent->colour() == "black")
							fix_double_black_conflicts(parent);
						else
							parent->set_colour("black");
					}
				}
			}
		}

		tree_node	*BST_replace_node(tree_node *x) {
			if (x->left() != NULL && x->right() != NULL)
				return _Rb_tree_successor(x);
			if (x->left() == NULL && x->right() == NULL)
				return (NULL);
			if (x->left() != NULL)
				return (x->left());
			else
				return (x->right());
		}

		void	_Rb_tree_node_deletion(tree_node *v)
		{
			tree_node *u = BST_replace_node(v);

			bool uvBlack = ((u == NULL || u->colour() == "black") && (v->colour() == "black"));
			tree_node *parent = v->parent();

			if (u == NULL) {
				if (v == root()) {
					set_root(NULL);
				} else {
					if (uvBlack)
						fix_double_black_conflicts(v);
					else {
						if (v->get_sibling() != NULL)
						v->get_sibling()->set_colour("red");
					}
					if (v->is_left_child()) {
						parent->set_left(NULL);
					} else {
						parent->set_right(NULL);
					}
				}
				delete_node(v);
				return ;
			}
			if (v->left() == NULL or v->right() == NULL) {
				if (v == root()) {
					_Rb_tree_transplant(v, u);
					v->set_left(NULL);
					v->set_right(NULL);
					delete_node(u);
				} else {
					if (v->is_left_child())
						parent->set_left(u);
					else
						parent->set_right(u);
					delete_node(v);
					u->set_parent(parent);
					if (uvBlack)
						fix_double_black_conflicts(u);
					else
						u->set_colour("black");
				}
				return ;
			}
			_Rb_tree_transplant(u, v);
			_Rb_tree_node_deletion(u);
		}

		void	swap_colours(tree_node *_x, tree_node *_y) {
			std::string temp = _x->colour();
			_x->set_colour(_y->colour());
			_y->set_colour(temp);
		}

		void	_Rb_tree_left_rotation(tree_node *_x) {
			tree_node *nParent = _x->right();
			if (_x == root())
				set_root(nParent);
			_x->move_down(nParent);
			_x->set_right(nParent->left());
			if (nParent->left() != NULL)
				nParent->left()->set_parent(_x);
			nParent->set_left(_x);
		}

		void	_Rb_tree_right_rotation(tree_node *_x) {
			tree_node *nParent = _x->left();
			if (_x == root())
				set_root(nParent);
			_x->move_down(nParent);
			_x->set_left(nParent->right());
			if (nParent->right() != NULL)
				nParent->right()->set_parent(_x);
			nParent->set_right(_x);
		}

		void	fix_red_red_violations(tree_node *_x) {
			if (_x == root()) {
				_x->set_colour("black");
				return ;
			}
			tree_node *parent = _x->parent(), *grandparent = parent->parent(), *uncle = _x->get_uncle_node();
			if (parent->colour() != "black") {
				if (uncle != NULL && uncle->colour() == "red") {
					parent->set_colour("black");
					uncle->set_colour("black");
					grandparent->set_colour("red");
					fix_red_red_violations(grandparent);
				} else {
					if (parent->is_left_child()) {
						if (_x->is_left_child()) {
							swap_colours(parent, grandparent);
						} else {
							_Rb_tree_left_rotation(parent);
							swap_colours(_x, grandparent);
						}
						_Rb_tree_right_rotation(grandparent);
					} else {
						if (_x->is_left_child()) {
							_Rb_tree_right_rotation(parent);
							swap_colours(_x, grandparent);
						} else {
							swap_colours(parent, grandparent);
						}
						_Rb_tree_left_rotation(grandparent);
					}
				}
			}
		}

		static int	calculate_height(tree_node *temp)
		{
			int h = 0;
			if (temp != NULL) {
				int l_height = calculate_height(temp->left());
				int r_height = calculate_height(temp->right());
				int max_height = std::max(l_height, r_height);
				h = max_height + 1;
			}
			return h;
		}

		static void	recomp_height(tree_node *_x)
		{
			while (_x != NULL)
			{
				_x->_height = calculate_height(_x);
				_x = _x->parent();
			}
		}

		tree_node	*_Rb_tree_insert(tree_node *_x, const value_type& val)
		{
			tree_node	*_y = new_node(val);

			if (!_x)
			{
				_x = _y;
				_x->set_colour("black");
				++_size;
				++_black_height;
				return (_x);
			}
			tree_node *temp = _iterative_Rb_tree_search(val.first);
			if (temp->data().first == val.first)
			{
				delete_node(_y);
				return (temp);
			}
			_y->set_parent(temp);
			if (_key_comp(val.first, temp->data().first))
				temp->set_left(_y);
			else
				temp->set_right(_y);
			fix_red_red_violations(_y);
			suffix_traversal(root(), recomp_height);
			++_size;
			_y = _recursive_Rb_tree_search(root(), val.first);
			return (_y);
		}

		void erase( iterator pos );
//		iterator erase( const_iterator pos );	//C++ 11
		void erase( iterator first, iterator last );
//		iterator erase( const_iterator first, const_iterator last );	//C++ 11

		size_type	erase( const key_type& key )
		{
			tree_node *p = _recursive_Rb_tree_search(root(), key);
			if (!p)
				return (0);
			--_size;
			_Rb_tree_node_deletion(p);
			return (1);
		}

/*
		void	clear() {
			erase(begin(), end());
		}
*/
		pair<iterator, bool>	insert(const value_type& val)
		{
			tree_node *temp = _Rb_tree_insert(root(), val);
			if (!root())
			{
				set_root(temp);
				temp->set_parent(NULL);
			}
			iterator it(temp);

			if (it->second == val.second)
				return (make_pair(it, true));
			return (make_pair(it, false));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			(void)pos;
			tree_node *temp = _Rb_tree_insert(pos.base(), val);
			if (!root())
			{
				set_root(temp);
				temp->set_parent(NULL);
			}
			return (iterator(temp));
		}

		template<class InputIterator> void	insert(InputIterator first, InputIterator last)
		{
			for (; first < last; ++first)
				insert(*first);
		}

		mapped_type&	operator[] (const key_type& k)
		{
			tree_node *found = _recursive_Rb_tree_search(root(), k);
			if (found)
				return (found->data().second);
			iterator it = insert(root(), ft::make_pair(k, mapped_type()));
			return (it->second);
		}

		mapped_type&	at(const key_type& k)
		{
			tree_node *found = _recursive_Rb_tree_search(root(), k);
			if (found)
				return (found->data().second);
			throw(std::out_of_range("set"));
		}

		const mapped_type&	at(const key_type& k) const
		{
			return (const_cast<mapped_type>(at((k))));
		}

	private :
		tree_node		*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_key_comp;
		size_type		_size;
		size_type		_black_height;
	};
}

#endif