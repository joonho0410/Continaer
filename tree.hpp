/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhjeon <junhjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 17:35:03 by junhjeon          #+#    #+#             */
/*   Updated: 2023/01/12 17:35:04 by junhjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Iterator.hpp"

namespace ft
{
    //////////////////////////////////////////////////////
	/////     rb_tree node  begin	         
	//////////////////////////////////////////////////////

	typedef bool _color;
	const _color _red = false;
	const _color _black = true;

    struct _Rb_tree_node_base
    {
        typedef _Rb_tree_node_base* _node_ptr;

        _color      _m_color;
        _node_ptr   _m_parents;
        _node_ptr   _m_left;
        _node_ptr   _m_right;

        static _node_ptr _S_minimum(_node_ptr _x)
        { while (_x -> _m_left != 0) _x = _x->_m_left; return _x; }
        static _node_ptr _S_maximum(_node_ptr _x)
        { while (_x -> _m_right != 0) _x = _x->_m_right; return _x; }

    };

    template <class _value>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
        typedef _Rb_tree_node<_value>*  _Link_type;//
        _value      _m_value_field;
    };

    //////////////////////////////////////////////////////
	/////     rb_tree iterator	         
	//////////////////////////////////////////////////////

    struct _Rb_tree_iterator_base
    {
        typedef typename _Rb_tree_node_base::_node_ptr      _node_ptr;
        typedef ft::bidirectional_iterator_tag              iterator_category;
        typedef ptrdiff_t                                   difference_type;

        _node_ptr                                           _m_node;
    };

    template <class _value, class _ref, class _ptr>
    struct _Rb_tree_iterator : public _Rb_tree_iterator_base
    {
        typedef _value                                                  value_type;
        typedef _ref                                                    reference;
        typedef _ptr                                                    pointer;
        typedef _Rb_tree_iterator<_value, _value&, _value*>             iterator;
        typedef _Rb_tree_iterator<_value, const _value&, const _value*> const_iterator;
        typedef _Rb_tree_iterator<_value, _ref, _ptr>                   _Self;
        typedef _Rb_tree_node<_value>*                                  _link_type;
    };

    //////////////////////////////////////////////////////
	/////     rb_tree 	         
	//////////////////////////////////////////////////////

    template <class _Tp, class _Alloc>
    struct _Rb_tree_base
    {
        public : 
            typedef typename _Alloc::rebind< _Rb_tree_node<_Tp> >::other allocator_type; 
        
        protected :
            _Rb_tree_node<_Tp>*         _m_header; // 트리의 root?

        public :
            _Rb_tree_base (const allocator_type& _a) : _M_header(0)
            { _m_header = _M_get_node(); }
            ~_Rb_tree_base ()
            { _M_put_node(_m_header); }
            
            // allocator_type()을 계속 생성하기 때문에 느려질 수도 ?
            // Node allocate;
            _Rb_tree_node<_Tp>* _M_get_node()
            { return allocator_type().allocate(1); }

            // Node deallocate;
            void _M_put_node(_Rb_tree_node<_Tp>& _p)
            { allocator_type().deallocator(_p, 1); }
    };

    template <class Key, class Value, class KeyofValue, class Compare,
                class _Alloc = std::allocator<Value> >
    class _Rb_tree : protected _Rb_tree_base<Value, _Alloc>
    {
            typedef typename _Rb_tree_base<Value, _Alloc> _Base;
        protected :
            typedef _Rb_tree_node_base*     _Base_ptr; // node_base pointer, 해당노드의 부모,자식,컬러등의 정보를 저장.
            typedef _Rb_tree_node<Value>    _Rb_tree_node;
            typedef _color                  _color;
        
        public :    
            typedef Key                 key_type;
            typedef Value               value_type;
            typedef value_type*         pointer;
            typedef const value_type*   const_pointer;
            typedef value_type&         reference;
            typedef const value_type&   const_reference;
            typedef _Rb_tree_node*      _Link_type; // node pointer, value 값을 저장.
            typedef size_t              size_type;
            typedef ptrdiff_t           difference_type;

            typedef _Rb_tree_iterator<value_type, reference, pointer>                iterator;
            typedef _Rb_tree_iterator<value_type, const_reference, const_pointer>    const_iterator;
            typedef reverse_iterator<iterator>                                       reverse_iterator;
            typedef reverse_iterator<const_iterator>                                 const_reverse_iterator;

            typedef typename _Rb_tree_base<Value, _Alloc>::allocator_type   allocator_type; // Node구조체(_Rb_tree_node)의 allocator;
            allocator_type get_allocator() const { return allocator_type(); } 

        protected :
            
            _Link_type&
            _M_copy(_Link_type& _from, _Link_type& _to)
            {
                _Link_type _top = _M_clone_node(_from);
                _top->_m_parents = _to;
            }

            _Link_type
            _M_create_node(const value_type& _x)
            {
                _Link_type _tmp = _M_get_node();
                _Alloc().construct(&_tmp->_m_value_field, _x);
                return _tmp;
            }
            
            _Link_type
            _M_clone_node(_Link_type _x)
            {
                _Link_type _tmp = _M_create_node(_x->_m_value_field);
                _tmp -> _m_color = _x->_m_color;
                _tmp -> _m_left = 0;
                _tmp -> _m_right = 0;
                return _tmp;
            }

            void
            _M_destroy_node(_Link_type _x)
            {
                _Alloc().destroy(&_x->_m_value_field);
                _M_put_node(_x);
            }

        protected :
            size_type   _m_node_count;
            Compare     _m_key_compare; // what is compare class ?

            _Link_type& _M_root() const
            { return (_Link_type&) _m_header -> _m_parent; }
            _Link_type& _M_leftmost() const
            { return (_Link_type&) _m_header -> _m_left; }
            _Link_type& _M_rightmost() const
            { return (_Link_type&) _m_header -> _m_right; }

            static _Link_type& _S_left(_Link_type _x)
                { return (_Link_type&)(_x -> _m_left; )}
            static _Link_type& _S_right(_Link_type _x)
                { return (_Link_type&)(_x -> _m_right; )}
            static _Link_type& _S_parents(_Link_type _x)
                { return (_Link_type&)(_x -> _m_parents; )}

            static reference _S_value(_Link_type _x)
                { return _x -> _m_value_field; }
            static const _Key& _S_key(_Link_type _x) // what is keyofvalue?
                { return _x -> KeyOfValue()(_S_value(_x)); }
            static _color& _S_color(_Link_type _x)
                { return (_color&)_x -> _m_color; }

            /// base_ptr version.
            
            static _Link_type& _S_left(_Base_ptr _x)
                { return (_Link_type&)(_x -> _m_left; )}
            static _Link_type& _S_right(_Base_ptr _x)
                { return (_Link_type&)(_x -> _m_right; )}
            static _Link_type& _S_parents(_Base_ptr _x)
                { return (_Link_type&)(_x -> _m_parents; )}

            static reference _S_value(_Base_ptr _x)
                { return _x -> _m_value_field; }
            static const _Key& _S_key(_Base_ptr _x) // what is keyofvalue? && _Link_type(_x) ?
                { return _x -> KeyOfValue()(_S_value(_Link_type(_x))); }
            static _color& _S_color(_Base_ptr _x)
                { return (_color&)(_Link_type(_x) -> _m_color); }

            /// find left last or right last;
            static _Link_type _S_minimum(_Link_type _x) 
                { return (_Link_type)  _Rb_tree_node_base::_S_minimum(_x); }
            static _Link_type _S_maximum(_Link_type __x)
                { return (_Link_type) _Rb_tree_node_base::_S_maximum(_x); }

        private :
            void _M_initialize() // 각각의 레퍼런스를 불러서 초기화를 해준다.
            {
                _S_color(_m_header) = _red;
                _m_root() = 0;
                _m_leftmost() = _m_header;
                _m_rightmost() = _m_header;
            }

        public :
            _Rb_tree()
                : _Base(allocator_type()), _m_node_count(0), _m_key_compare()
                { _M_empty_initialize(); }
            _Rb_tree(const Compare& _comp)
                : _Base(allocator_type()), _m_node_count(0), _m_key_compare(_comp)
                { _M_empty_initialize(); }
            _Rb_tree(const Compare& _comp, const allocator_type& _a)
                : _Base(_a), _m_node_count(0), _m_key_compare(_comp)
                { _M_empty_initialize(); }
            _Rb_tree(const _Rb_tree<Key, Value, KeyofValue, Comapre, _Alloc> & _x)
                : _Base(_x.get_allocator()), _m_node_count(0), _m_key_compare(_x._m_key_compare)
                {
                    if (_x._M_root() == 0) // change 0 == null_ptr ? 
                        _M_empty_initialize();
                    else
                    {
                        _S_color(_m_header) = _red;
                        _M_root() = _M_copy(_x._M_root(), _m_header);//?
                        _M_leftmost() = _S_minimum(_M_root());
                        _M_rightmost() = _S_maximum(_M_root());
                    }
                    _m_node_count = _x._m_node_count;
                }
            ~_Rb_tree() { clear (); }
    };
};