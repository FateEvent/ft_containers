#ifndef MAP_H
# define MAP_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <limits>
# include "ContainerException.hpp"
# include "pair.hpp"
# include "vector.hpp"
# include "map_iterator.hpp"
#include <map>

class map_iterator;

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>,
	class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map {
		struct Node;
	public:
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
		typedef typename Allocator:: template rebind<Node>::other	node_allocator;
		typedef typename node_allocator::pointer					node_pointer;

		typedef ft::map_iterator<key_type, mapped_type, Node>		iterator;
//		typedef implementation-defined								const_iterator;
//		typedef std::reverse_iterator<iterator>						reverse_iterator;
//		typedef std::reverse_iterator<const_iterator>				const_reverse_iterator;

		class value_compare
			: public std::binary_function<value_type, value_type, bool>
		{
			friend class map;

		protected:
			key_compare	comp;

			value_compare(key_compare c);

		public:
			bool operator()(const value_type& x, const value_type& y) const { return x == y; }
		};

	private:
		struct Node {
		public:
			value_type	_data;
			Node		*_left;
			Node		*_right;
			Node		*_parent;
			int			_height;

		public:
			Node() : _data(value_type()), _left(NULL), _right(NULL), _parent(NULL), _height(1) {}
			Node(value_type pair) : _data(pair), _left(NULL), _right(NULL), _parent(NULL), _height(1) {}
			~Node() {}

			Node		&operator= (Node &other) { _left = other._left; _right = other._right; _parent = other._parent; _height = other._height; return (*this); }
			void		set_data(value_type &data) { _data = data; }
			void		set_left(Node *left) { _left = left; }
			void		set_right(Node *right) { _right = right; }
			void		set_parent(Node *parent) { _parent = parent; }
			value_type	&data() { return _data; }
			Node		*left() { return _left; }
			Node		*right() { return _right; }
			Node		*parent() { return _parent; }
			int			&height() { return _height; }
			void		treat(char sep) { std::cout << data().first << sep << data().second << " height: " << height() << std::endl; }
		};

	public:
		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(NULL), _alloc_node(node_allocator()), _alloc_pair(alloc), _key_comp(comp), _size() {
				_root = new_node();
			}

		template <class InputIterator>
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
		map(const map& x);

		~map() { delete_node(protoroot()); }
		map&	operator= (map const& base);
		Node	*protoroot() { return (_root); }
		Node	*root() { return (_root->right()); }
		void	set_root(Node *current) { _root->set_right(current); }

		void	prefix_traversal(Node *current, char sep) {
			if (current)
			{
//				current->treat(sep);
				update_height(current);
				prefix_traversal(current->left(), sep);
				prefix_traversal(current->right(), sep);
			}
		}

		void	infix_traversal(Node *current, char sep) {
			if (current)
			{
				infix_traversal(current->left(), sep);
//				current->treat(sep);
				update_height(current);
				infix_traversal(current->right(), sep);
			}
		}

		void	suffix_traversal(Node *current, void (*f)(Node *)) {
			if (current)
			{
				suffix_traversal(current->left(), f);
				suffix_traversal(current->right(), f);
				f(current);
			}
		}

		void	level_order_traversal(Node *current, char sep) {
			vector<Node *>	deck;

			deck.push_back(current);
			while (!deck.empty())
			{
				current = deck.front();
				deck.pop_front();
				current->treat(sep);
				if (current->left())
					deck.push_back(current->left());
				if (current->right())
					deck.push_back(current->right());
			}
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

		iterator	end() { return iterator(root() ? root() : protoroot()); }

		Node	*new_node(const value_type& pair = value_type(), Node *parent = NULL)
		{
			Node *ptr = _alloc_node.allocate(1);
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
			return (ptr);
		}

		Node *min_value_node(Node *node)
		{
			Node *current = node;
			while (current && current->left())
				current = current->left();
			return current;
		}

		void	delete_node(Node *node)
		{
			_alloc_node.destroy(node);
			_alloc_node.deallocate(node, 1);
		}

		void rebalance(Node *p)
		{
			Node *x, *y, *z;

			while ( p != NULL )
			{ 
				if ( diffHeight(p->left(), p->right()) > 1 )
				{
					x = p;
					y = tallerChild( x );
					z = tallerChild( y );
					p = tri_node_restructure( x, y, z );
				}

				p = p->parent();
			}
		}

		Node *tallerChild(Node *p)
		{
			if ( p->left() == NULL )
				return p->right();

			if ( p->right() == NULL )
				return p->left();

			if ( p->left()->height() > p->right()->height())
				return p->left();
			else
				return p->right();
		}

		Node *tri_node_restructure( Node *x, Node *y, Node *z)
		{
			bool zIsLeftChild = (z == y->left());
			bool yIsLeftChild = (y == x->left());
			Node *a, *b, *c;
			Node *T0, *T1, *T2, *T3;

			if (zIsLeftChild && yIsLeftChild) 
			{
				a = z;
				b = y;
				c = x;
				T0 = z->left();
				T1 = z->right();
				T2 = y->right();
				T3 = x->right();
			}
			else if (!zIsLeftChild && yIsLeftChild) 
			{
				a = y;
				b = z;
				c = x;
				T0 = y->left();
				T1 = z->left();
				T2 = z->right();
				T3 = x->right();
			}
			else if (zIsLeftChild && !yIsLeftChild) 
			{
				a = x;
				b = z;
				c = y;
				T0 = x->left();
				T1 = z->left();
				T2 = z->right();
				T3 = y->right();
			}
			else 
			{
				a = x;
				b = y;
				c = z;
				T0 = x->left();
				T1 = y->left();
				T2 = z->left();
				T3 = z->right();
			}
			if ( x == root() )
			{
				set_root(b);
				b->set_parent(NULL);
			}
			else 
			{
				Node *xParent;

				xParent = x->parent();
				if ( x == xParent->left() ) 
				{
					b->set_parent(xParent);
					xParent->set_left(b);
				}
				else 
				{
					b->set_parent(xParent);
					xParent->set_right(b);
				}
			}
			b->set_left(a);
			a->set_parent(b);
			b->set_right(c);
			c->set_parent(b);

			a->set_left(T0);
			if ( T0 != NULL ) T0->set_parent(a);
			a->set_right(T1);
			if ( T1 != NULL ) T1->set_parent(a);
			c->set_left(T2);
			if ( T2 != NULL ) T2->set_parent(c);
			c->set_right(T3);
			if ( T3 != NULL ) T3->set_parent(c);

			recompHeight(a);
			recompHeight(c);

			return b;
		}

		static int diffHeight( Node *t1, Node *t2 )
		{
			int h1, h2;

			if ( t1 == NULL )
				h1 = 0;
			else
				h1 = t1->height();

			if ( t2 == NULL )
				h2 = 0;
			else
				h2 = t2->height();

			return ((h1 >= h2) ? (h1-h2) : (h2-h1)) ;
		}

		static void recompHeight( Node *_x )
		{
			while ( _x != NULL )
			{
				_x->_height = calculate_height(_x);
				_x = _x->parent();
			}
		}

		void	replace_node(Node *a, Node *b)
		{
			Node *temp = new_node(b->data());
			temp->set_left(a->left());
			temp->set_right(a->right());
			temp->set_parent(a->parent());
			std::cout << "ciao " << std::endl;
			a = temp;
		}

		void	_avl_tree_node_deletion(const key_type& k)
		{
			Node	*p(NULL);
			Node	*parent(NULL);
			Node	*succ(NULL);

			p = _avl_tree_search(root(), k);
			if (!p || !(k == p->data().first))
				return ;
			if (p->left() == NULL && p->right() == NULL)
			{
				parent = p->parent();

				if ( parent->left() == p )
					parent->set_left(NULL);
				else
					parent->set_right(NULL);
				delete_node(p);
				recompHeight( parent );
				rebalance(parent);
				return ;
			}
			if ( p->left() == NULL )
			{
				parent = p->parent();
				if ( parent->left() == p )
					parent->set_left(p->right());
				else
					parent->set_right(p->right());
				delete_node(p);
				recompHeight( parent );
				rebalance(parent);
				return ;
			}

			if ( p->right() == NULL )
			{
				parent = p->parent();
				if ( parent->left() == p )
						parent->set_left(p->left());
				else
						parent->set_right(p->left());
				delete_node(p);
				recompHeight( parent );
				rebalance(parent);
				return ;
			}
			succ = p->right();
			while ( succ->left() != NULL )
				succ = succ->left();
			replace_node(p, succ);

			parent = succ->parent();
			parent->set_left(succ->right());

			recompHeight( parent );
			rebalance(parent);
			return ;
		}

		void	delete_tree_node(const key_type& k)
		{
//			return (iterator(_avl_tree_node_deletion(k)));
			_avl_tree_node_deletion(k);
		}

		Node	*_avl_tree_search(Node *node, const key_type& k)
		{
			if (node == NULL)
				return (NULL);
			else if (node->data().first == k)
				return (node);
			else if (_key_comp(k, node->data().first))
			{
				Node *temp = _avl_tree_search(node->left(), k);
				return (temp);
			}
			else
			{
				Node *temp = _avl_tree_search(node->right(), k);
				return (temp);
			}
		}

		static void	update_height(Node *node)
		{
			if (node != NULL) {
				int val = 1;
				if (node->left() != NULL)
					val = node->left()->height() + 1;
				if (node->right() != NULL)
					val = std::max(val, node->right()->height() + 1);
				node->_height = val;
			}
		}

		static int	calculate_height(Node *temp)
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

		Node	*_avl_tree_insert(Node *_x, const value_type& val)
		{
			Node	*newNode = new_node(val);
			Node	*_y;

			if (!_x)
			{
				_x = newNode;
				++_size;
				return (_x);
			}
			while (_x != NULL) {
				_y = _x;
				if (_key_comp(val.first, _x->data().first))
					_x = _x->left();
				else if (val.first == _x->data().first)
				{
					delete_node(newNode);
					return (_x);
				}
				else
					_x = _x->right();
			}
			if (_y == NULL)
			{
				_y = newNode;
				newNode->set_parent(_y->parent());
			}
			else if (_key_comp(val.first, _y->data().first))
			{
				_y->set_left(newNode);
				newNode->set_parent(_y);
			}
			else
			{
				_y->set_right(newNode);
				newNode->set_parent(_y);
			}
			++_size;
//			set_root(balance_tree(root()));
			rebalance(root());
			suffix_traversal(root(), update_height);
			// find the node (the iterator?) _y by finding the key
//			_y = _avl_tree_search(root(), val);
			return (_y);
		}


		pair<iterator, bool>	insert(const value_type& val)
		{
			Node *temp = _avl_tree_insert(root(), val);
			if (!root())
				set_root(temp);
			iterator	it(temp);

			if (it->second == val.second)
				return (make_pair(it, true));
			return (make_pair(it, false));
		}

		iterator	insert(iterator pos, const value_type& val)
		{
			if (pos >= begin() && pos <= end())
				return (iterator(_avl_tree_insert(pos.base(), val)));
			else
				throw(ContainerException("out_of_range"));
		}

		template<class InputIterator> void	insert(InputIterator first, InputIterator last)
		{
			for (; first < last; ++first)
				insert(*first);
		}

//		mapped_type&	operator[] (const key_type& k) {

//		}

	private :
		Node			*_root;
		node_allocator	_alloc_node;
		allocator_type	_alloc_pair;
		key_compare		_key_comp;
		size_type		_size;
	};
}

#endif