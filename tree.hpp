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
        typedef _Rb_tree_node_base::_node_ptr                _node_ptr;
        typedef ft::bidirectional_iterator_tag              iterator_category;
        typedef ptrdiff_t                                   difference_type;

        _node_ptr                                           _m_node;

        void _M_increment()//iterator ??????.
        {
            if (_m_node->_m_right != 0)
            {
                _m_node = _m_node->_m_right;
                while (_m_node->_m_left != 0)
                    _m_node = _m_node->_m_left;
            }
            else
            {
                _node_ptr _parents = _m_node->_m_parents;
                while (_m_node == _parents->_m_right)//?????? parents??? ????????? ??? ??? ?????? ????????????.
                {
                    _m_node = _parents;
                    _parents = _parents -> _m_parents;
                }
                if (_m_node->_m_right != _parents)//? 
                    _m_node = _parents;
            }
        }
        
        void _M_decrement()//iterator ??????.
        {
            if (_m_node->_m_color ==_red && _m_node->_m_parents->_m_parents == _m_node)
                _m_node = _m_node->_m_right; // _m_node??? header node ?????? right_max return;
            else if (_m_node->_m_left != 0)
            {
                _m_node = _m_node->_m_left;
                while (_m_node->_m_right != 0)
                    _m_node = _m_node->_m_right;
            }
            else
            {
                _node_ptr _parents = _m_node->_m_parents;
                while (_m_node == _parents->_m_left)
                {
                    _m_node = _parents;
                    _parents = _parents->_m_parents;
                }
                _m_node = _parents;
            }
        }
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
        typedef _Rb_tree_node<_value>*                                  _Link_type;


        _Rb_tree_iterator(){}
        _Rb_tree_iterator(_Link_type _x) { _m_node = _x ; }
        _Rb_tree_iterator(const iterator* _it) { _m_node = _it->_m_node ; }
        reference operator*() const { return _Link_type(_m_node)->_m_value_field; }
        pointer operator->() const { return &(operator*()); }

        _Self& operator++() { _M_increment(); return *this ;}
        _Self  operator++(int)
        {
            _Self _tmp = *this;
            _M_increment();
            return _tmp;
        }

        _Self& operator--() { _M_decrement(); return *this ;}
        _Self  operator--(int)
        {
            _Self _tmp = *this;
            _M_decrement();
            return _tmp;
        }

        operator _Rb_tree_iterator<_value, const _value&, const _value*>() const
        {  
            return _Rb_tree_iterator<_value, const _value&, const _value*>((_Link_type)_m_node);
        }
        static inline void
        _Rb_tree_rotate_left(_Rb_tree_node_base* _x, _Rb_tree_node_base*& _root)
        {
            _Rb_tree_node_base* _xr = _x-> _m_right;
            _x -> _m_right = _xr -> _m_left;
            if (_xr->_m_left != 0) 
                _xr->_m_left->_m_parents = _x;
            _xr->_m_parents = _x->_m_parents;

            if (_x == _root)
                _root = _xr;
            else if (_x == _x->_m_parents->_m_left)
                _x->_m_parents->_m_left = _xr;
            else
                _x->_m_parents->_m_right = _xr;

            _xr->_m_left = _x;
            _x->_m_parents = _xr;
        }

        static inline void
        _Rb_tree_rotate_right(_Rb_tree_node_base* _x, _Rb_tree_node_base*& _root)
        {
            _Rb_tree_node_base* _xl = _x->_m_left;
            _x->_m_left = _xl->_m_right;

            if (_xl->_m_right != 0)
                _xl->_m_right->_m_parents = _x;
            _xl->_m_parents = _x->_m_parents;

            if(_x == _root)
                _root = _xl;
            else if (_x == _x->_m_parents->_m_left)
                _x->_m_parents->_m_left = _xl;
            else
                _x->_m_parents->_m_right = _xl;

            _xl->_m_right = _x;
            _x ->_m_parents = _xl;
        }

        static inline void
        _Rb_tree_rebalance_insert(_Rb_tree_node_base* _x, _Rb_tree_node_base*& _root)// _x -> ????????? node, _root -> ????????????.
        {
            _x->_m_color = _red; // insert node ??? ?????? red;
            while (_x != _root && _x->_m_parents->_m_color == _red)//RR ????????? ??????. ????????? black??? ???????????? ????????? root??? ????????? ????????????????????? ????????? ?????????????????????.
            {
                if (_x->_m_parents == _x->_m_parents->_m_parents->_m_left)//????????? ??????????????? ?????? ????????? ??????;
                {
                    if (_x->_m_parents->_m_parents->_m_right && 
                        _x->_m_parents->_m_parents->_m_right->_m_color == _red)//????????? color??? ???????????? case1;
                    {
                        _x->_m_parents->_m_color = _black; // ????????? ????????? ????????? ?????? black?????? ?????????;
                        _x->_m_parents->_m_parents->_m_right->_m_color = _black;
                        _x->_m_parents->_m_parents->_m_color = _red;
                        _x = _x->_m_parents->_m_parents; // ?????????????????????????????? ???????????? ????????????.
                    }
                    else // ????????? ??????????????? ????????????????????? red, ????????? black??? ??????  
                    {
                        if (_x->_m_parents->_m_right == _x) // _x??? ????????? ????????? ????????? ?????? case2
                        {
                            _x = _x->_m_parents;
                            _Rb_tree_rotate_left(_x, _root);// p[z]??? ????????? rotate
                        }
                        // ????????? ?????? ????????? ?????? case3
                        _x->_m_parents->_m_color = _black;
                        _x->_m_parents->_m_parents->_m_color = _red;
                        _Rb_tree_rotate_right(_x->_m_parents->_m_parents, _root);//p[p[z]]??? ????????? rotate
                    }
                }
                else // ????????? ??????????????? ????????? ????????? ??????. ?????? ??????????????? left??? right??? ?????? ????????? ????????? ??????.
                {
                    
                    if (_x->_m_parents->_m_parents->_m_left && 
                        _x->_m_parents->_m_parents->_m_left->_m_color == _red)//????????? color??? ???????????? case1;
                    {
                        _x->_m_parents->_m_color = _black; // ????????? ????????? ????????? ?????? black?????? ?????????;
                        _x->_m_parents->_m_parents->_m_left->_m_color = _black;
                        _x->_m_parents->_m_parents->_m_color = _red;
                        _x = _x->_m_parents->_m_parents; // ?????????????????????????????? ???????????? ????????????.
                    }
                    else // ????????? ??????????????? ????????????????????? red, ????????? black??? ??????  
                    {
                        if (_x->_m_parents->_m_left == _x) // _x??? ????????? ?????? ????????? ?????? case2
                        {
                            _x = _x->_m_parents;
                            _Rb_tree_rotate_right(_x, _root);// p[z]??? ????????? rotate
                        }
                        // ????????? ?????? ????????? ?????? case3
                        _x->_m_parents->_m_color = _black;
                        _x->_m_parents->_m_parents->_m_color = _red;
                        _Rb_tree_rotate_left(_x->_m_parents->_m_parents, _root);//p[p[z]]??? ????????? rotate
                    }
                }
            }
            _root->_m_color = _black;//????????? black??? ????????? ????????? ???????????? ????????? ?????? ??????????????????. _x??? _root??? ????????? ???????????? _root??? black?????? ????????????.
        }

        static inline _Rb_tree_node_base*
        _Rb_tree_rebalance_erase(_Rb_tree_node_base* _z,
                                 _Rb_tree_node_base*& _root,
                                 _Rb_tree_node_base*& _leftmost,
                                 _Rb_tree_node_base*& _rightmost)
        {
            _Rb_tree_node_base* _target = _z; // really deleted (succesor)
            _Rb_tree_node_base* _succesor_p = 0; 
            _Rb_tree_node_base* _succesor_c = 0; // succesor's child node;

            //succesor??? ?????? ???????????? ????????? ???????????? ????????? ?????? ??????.

            if (_target->_m_left == 0)// ????????? ??????????????? ???????????? ??????????????? null?????? ?????? ???????????? ?????? ??????????????????.
                _succesor_c = _target->_m_right;
            else if (_target->_m_right == 0) //????????? ????????? ???????????? ????????? ????????? ?????????.
                _succesor_c = _target->_m_left;
            else // ????????? ?????? ????????? ???;
            {
                _target = _target->_m_right;
                while (_target->_m_left != 0)
                    _target = _target->_m_left;
                _succesor_c = _target->_m_right; // ????????? ?????? ?????????????????? ????????? ???????????? ????????? ?????? ?????????.
            }
            
            if (_target != _z) //????????? ???????????? ?????? ????????? ?????? ; availalbe succesor;
            {
                //target??? _z??? ???????????? ????????? ?????? ????????????.
                _z->_m_left->_m_parents = _target;// ????????? ????????? ????????? ????????? ????????? ??????????????????.
                _target->_m_left = _z->_m_left; //_target ??? ????????? ????????? ??? ?????? ?????? ???????????? ?????? ????????? ??????????????????;
                if (_target != _z->_m_right) // ???????????? ????????? ???????????? ????????? ????????? ????????? ????????????.
                {
                    _succesor_p = _target->_m_parents;
                    if (_succesor_c) _succesor_c->_m_parents = _target->_m_parents; //????????? ?????????????????? ???????????? ????????? ????????? ??????
                    _target->_m_parents->_m_left = _succesor_c; // ???????????? ????????? ????????? ?????????.
                    _target->_m_right = _z->_m_right; // ???????????? ????????? ???????????? ??????
                    _z->_m_right->_m_parents = _target; //???????????? ????????? ???????????? ??????2
                } 
                else
                    _succesor_p = _target; // ???????????? ????????? ???????????? ????????? ????????? ????????????.
                if(_root == _z) // ?????? _root??? ??????????????? _root??? ????????? ??????
                    _root = _target;
                else if (_z->_m_parents->_m_left == _z)// ?????? ?????? ??????????????? ???, ???????????? ????????? ????????? ????????? ??????
                    _z->_m_parents->_m_left = _target;
                else// ???????????? ????????? ????????? ???????????? ??????;
                    _z->_m_parents->_m_right = _target;
                _target->_m_parents = _z->_m_parents; // ???????????? ????????? ????????? ???????????? ????????? ????????? ??????;
                std::swap(_target->_m_color, _z->_m_color); // ?????? ?????? _z??? ????????? ???????????? _target??? ????????? ??????.
                _target = _z; // ???????????? ????????? ?????? ??????
            }
            else // ???????????? ????????? ??????????????? ???????????? ?????? ?????? ?????? ?????? ?????? ?????? ???????????? ???????????? ????????? ????????? ????????? ??????????????????. ?????? ??? ?????????????????????? -> _target == _z ??? ?????? ????????? ????????? ??????
            {
                //succesor_c ??? _target??? ????????? ?????? ??????. ??? ???????????? ????????????. ??? ?????? _succesor??? null??? ??? ??? ??????.
                _succesor_p = _z->_m_parents;
                if (_succesor_c)
                    _succesor_c->_m_parents = _target->_m_parents;
                if (_z == _root)
                    _root = _succesor_c;
                else if (_z->_m_parents->_m_left == _z)
                    _z->_m_parents->_m_left = _succesor_c;
                else
                    _z->_m_parents->_m_right = _succesor_c;
                if (_leftmost == _z) // ???????????? z??? left most ??? ????????? ?????? ??? ??????
                {
                    if (_z->_m_right == 0) // ???????????? ???????????? ???????????? ????????? ?????? ?????? ??????.
                        _leftmost = _z->_m_parents;
                    else // ???????????? ????????? ?????? ?????? ???????????? ??????.
                        _leftmost = _Rb_tree_node_base::_S_minimum(_succesor_c);
                }
                if (_rightmost == _z) // ????????? ????????????
                {
                    if(_z->_m_left == 0)
                        _rightmost = _z->_m_parents;
                    else // ???????????? ????????? ????????? ?????? ???????????? ??????.
                        _rightmost = _Rb_tree_node_base::_S_maximum(_succesor_c);
                }
            }
            /// ???????????? ?????? ?????? ?????? ?????? 


            if (_target->_m_color == _black) // _target??? ????????? red??? ?????????????????? ???????????? ??????. ?????? x
            {
                //?????? ???????????? ?????? ????????? ????????? ?????? ???????????? ??????????????? succesor??? ????????? succesor??? child??? ????????? ????????? ??????.
                while (_succesor_c != _root && (_succesor_c == 0 || _succesor_c->_m_color == _black))
                { // ???????????? root??? ????????? ???????????? ????????? black ??? ?????????????????? black??? ???
                    _Rb_tree_node_base* _w;
                    if (_succesor_c == _succesor_p->_m_left)
                    {
                        _w = _succesor_p->_m_right;
                        if (_w->_m_color == _red )//case 1. ??????????????? red ??? ??????.
                        {
                            _w->_m_color = _black;
                            _succesor_p -> _m_color = _red;
                            _Rb_tree_rotate_left(_succesor_p, _root);
                            _w = _succesor_p->_m_right;
                        }
                        if ((_w->_m_left == 0 || _w->_m_left->_m_color == _black) && // case2. ??????????????? black, ??????????????? ???????????? ?????? black;
                        (_w->_m_right == 0 || _w->_m_right->_m_color == _black)) // nil node is black
                        {
                            _w->_m_color = _red;
                            _succesor_c = _succesor_p;
                            _succesor_p = _succesor_p->_m_parents;
                        }
                        else
                        {
                            if (_w->_m_right == 0 || _w->_m_right->_m_color == _black)//case3. ???????????? black, ??????????????? ????????? red
                            {
                                if (_w->_m_left) _w->_m_left->_m_color = _black;
                                    _w->_m_color = _red;
                                _Rb_tree_rotate_right(_w, _root);
                                _w = _succesor_p->_m_right;
                            }
                            _w->_m_color = _succesor_p->_m_color;//case4. ???????????? black, ??????????????? ???????????? red
                            _succesor_p ->_m_color = _black;
                            if (_w->_m_right) _w->_m_right->_m_color = _black;
                            _Rb_tree_rotate_left(_succesor_p, _root);
                            //_succesor_c = _root;
                            break;
                        }
                    }
                    else // ????????????
                    {
                        _w = _succesor_p->_m_left;
                        if (_w->_m_color == _red)
                        {
                            _w->_m_color = _black;
                            _succesor_p->_m_color = _red;
                            _Rb_tree_rotate_right(_succesor_p, _root);
                            _w = _succesor_p->_m_left;
                        }
                        if ((_w->_m_right == 0 || _w->_m_right->_m_color == _black)
                            && (_w->_m_left == 0 || _w->_m_left->_m_color == _black))
                        {
                            _w->_m_color = _red;
                            _succesor_c = _succesor_p;
                            _succesor_p = _succesor_c->_m_parents;
                        }
                        else
                        {
                            if (_w->_m_left == 0 || _w->_m_left->_m_color == _black)
                            {                        
                                if (_w->_m_right) _w->_m_right->_m_color = _black;
                                    _w->_m_color = _red;
                            _Rb_tree_rotate_left(_w, _root);
                            _w = _succesor_p -> _m_left;
                            }
                            _w->_m_color = _succesor_p->_m_color;
                            _succesor_p->_m_color = _black;
                            if (_w->_m_left) _w->_m_left->_m_color = _black;
                            _Rb_tree_rotate_right(_succesor_p, _root);
                            //_succesor_c = _root;
                            break;
                        }
                    }
                }
                if (_succesor_c) _succesor_c->_m_color = _black;
            }
            return _target;
        }
    };

    template <class _Value, class _Ref, class _Ptr>
        inline bool operator==(const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __x,
                    const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __y) {
        return __x._m_node == __y._m_node;
        }

        template <class _Value>
        inline bool operator==(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
                    const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) {
        return __x._m_node == __y._m_node;
        }

        template <class _Value>
        inline bool operator==(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
                    const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y) {
        return __x._m_node == __y._m_node;
        }

        template <class _Value, class _Ref, class _Ptr>
        inline bool operator!=(const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __x,
                    const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __y) {
        return __x._m_node != __y._m_node;
        }

        template <class _Value>
        inline bool operator!=(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
                    const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) {
        return __x._m_node != __y._m_node;
        }

        template <class _Value>
        inline bool operator!=(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
                    const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y) {
        return __x._m_node != __y._m_node;
        }


    //////////////////////////////////////////////////////
	/////     rb_tree 	         
	//////////////////////////////////////////////////////

    template <class _Tp, class _Alloc>
    class _Rb_tree_base
    {
        public : 
            typedef typename _Alloc::template rebind<_Rb_tree_node<_Tp> >::other allocator_type; 
        
        protected :
            _Rb_tree_node<_Tp>*         _m_header; // ????????? root

        public :
            _Rb_tree_base (const allocator_type& _a) : _m_header(0)
            { (void) _a; _m_header = _M_get_node(); }
            ~_Rb_tree_base ()
            { _M_put_node(_m_header); }
            
            // allocator_type()??? ?????? ???????????? ????????? ????????? ?????? ?
            // Node allocate;

            _Rb_tree_node<_Tp>* _M_get_node()
            { return allocator_type().allocate(1); }

            // Node deallocate;
            void _M_put_node(_Rb_tree_node<_Tp>* _p)
            { allocator_type().deallocate(_p, 1); }
    };
    
    template <class Key, class Value, class KeyOfValue, class Compare, class _Alloc = std::allocator<Value> >
    class _Rb_tree : public _Rb_tree_base<Value, _Alloc>
    {   
        protected :
            typedef _Rb_tree_base<Value, _Alloc> _Base;   
            typedef _Rb_tree_node_base*     _Base_ptr; // node_base pointer, ??????????????? ??????,??????,???????????? ????????? ??????.
            typedef _Rb_tree_node<Value>    _Rb_tree_node;
            typedef _color                  _color;
            using _Base::_M_get_node;
            using _Base::_M_put_node;
            using _Base::_m_header;

        public :    
            typedef Key                 key_type;
            typedef Value               value_type; // ft::pair<key, value>;
            typedef value_type*         pointer;
            typedef const value_type*   const_pointer;
            typedef value_type&         reference;
            typedef const value_type&   const_reference;
            typedef _Rb_tree_node*      _Link_type; // node pointer, value ?????? ??????.
            typedef size_t              size_type;
            typedef ptrdiff_t           difference_type;

            typedef _Rb_tree_iterator<value_type, reference, pointer>                    iterator;
            typedef _Rb_tree_iterator<value_type, const_reference, const_pointer>        const_iterator;
            typedef ft::reverse_iterator<iterator>                                       reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>                                 const_reverse_iterator;

            typedef typename _Rb_tree_base<Value, _Alloc>::allocator_type   allocator_type; // Node?????????(_Rb_tree_node)??? allocator;
            allocator_type get_allocator() const { return allocator_type(); } 

        protected :
            iterator
            _M_insert(_Link_type _child, _Link_type _parents, const value_type& _v)
            {
                // _child ????????? _v??? ????????? ??????
                _Link_type _tmp;
                // _y = _m_header ?????????  _x?????? ??????,  _v.value < _y.value;
                // _child != 0??? ??????? ????????? ???????????? ?????????  _child??? ????????? ???????????? ?????? ????????? ?????????????
                // _parents == _m_header ????????? ???????????? ???
                if (_parents == _m_header || _child != 0 || _m_key_compare(KeyOfValue()(_v), _S_key(_parents)))
                {
                    //parents ??? ????????? ?????? ??????.
                    _tmp = _M_create_node(_v);
                    _S_left(_parents) = _tmp;
                    if (_parents == _m_header)
                    {
                        _M_root() = _tmp;
                        _M_rightmost() = _tmp;
                    }
                    else if (_parents == _M_leftmost())
                        _M_leftmost() = _tmp;
                }
                else // _v.value >= _y.value
                {
                    //parents??? ???????????? ?????? ??????.
                    _tmp = _M_create_node(_v);
                    _S_right(_parents) = _tmp;
                    if (_parents == _M_rightmost())
                        _M_rightmost() = _tmp;
                }
                _S_parents(_tmp) = _parents; // 
                _S_left(_tmp) = 0; // 2?????? ????????? 
                _S_right(_tmp) = 0;
                iterator::_Rb_tree_rebalance_insert(_tmp, _m_header->_m_parents);
                ++_m_node_count;
                return iterator(_tmp);
            }

            void
            _M_erase(_Link_type _x)
            {
                if (_x->_m_left != 0)
                    _M_erase(_S_left(_x));// _x -> _m_left;
                if (_x->_m_right != 0)
                    _M_erase(_S_right(_x));
                _M_destroy_node(_x);
            }

            _Link_type
            _M_copy(_Link_type _x, _Link_type _p)// p?????????  x??? ????????? ????????????. 
            {
                _Link_type _top = _M_clone_node(_x);
                _top->_m_parents = _p;     
                if (_x -> _m_right) //???????????? ?????? ????????? ?????? ??????;
                    _top -> _m_right = _M_copy(_S_right(_x), _top);
                _p = _top;
                _x = _S_left(_x);

                while (_x != 0)
                {
                    _Link_type _y = _M_clone_node(_x);
                    _p->_m_left = _y;
                    _y->_m_parents = _p;
                    if ( _x->_m_right)
                        _y->_m_right = _M_copy(_S_right(_x), _y);
                    _p = _y;
                    _x = _S_left(_x);
                }
                return _top;
            }

            _Link_type
            _M_create_node(const value_type& _x)
            {
                _Link_type _tmp = _M_get_node();
                _Alloc().construct(&_tmp->_m_value_field, _x);
                return _tmp;
            }
            
            _Link_type
            _M_clone_node(_Link_type _x) // color??? ????????? ?????? ???????????? ???????????? ???????????? ?????????.
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
            Compare     _m_key_compare; // std::less<key>

            _Link_type& _M_root() const
            { return (_Link_type&) _m_header -> _m_parents; }
            _Link_type& _M_leftmost() const
            { return (_Link_type&) _m_header -> _m_left; }
            _Link_type& _M_rightmost() const
            { return (_Link_type&) _m_header -> _m_right; }

            static _Link_type& _S_left(_Link_type _x)
                { return (_Link_type&)(_x -> _m_left); }
            static _Link_type& _S_right(_Link_type _x)
                { return (_Link_type&)(_x -> _m_right); }
            static _Link_type& _S_parents(_Link_type _x)
                { return (_Link_type&)(_x -> _m_parents); }

            static reference _S_value(_Link_type _x)
                { return _x -> _m_value_field; }
            static const Key& _S_key(_Link_type _x) // what is keyofvalue?
                { return KeyOfValue()(_S_value(_x)); }
            static _color& _S_color(_Link_type _x)
                { return (_color&)_x -> _m_color; }

            /// base_ptr version. iterator << vector<int> iterator == int*
            
            static _Link_type& _S_left(_Base_ptr _x)
                { return (_Link_type&)(_x -> _m_left); }
            static _Link_type& _S_right(_Base_ptr _x)
                { return (_Link_type&)(_x -> _m_right); }
            static _Link_type& _S_parents(_Base_ptr _x)
                { return (_Link_type&)(_x -> _m_parents); }

            static reference _S_value(_Base_ptr _x)
                { return ((_Link_type)_x) -> _m_value_field; }
            static const Key& _S_key(_Base_ptr _x) // what is keyofvalue? && _Link_type(_x) ?
                { return KeyOfValue()(_S_value(_x)); }
            static _color& _S_color(_Base_ptr _x)
                { return (_color&)(_Link_type(_x) -> _m_color); }

            /// find left last or right last;
            static _Link_type _S_minimum(_Link_type _x) 
                { return (_Link_type) _Rb_tree_node_base::_S_minimum(_x); }
            static _Link_type _S_maximum(_Link_type _x)
                { return (_Link_type) _Rb_tree_node_base::_S_maximum(_x); }

            void _M_empty_initialize() // ????????? ??????????????? ????????? ???????????? ?????????.
            {
                _S_color(_m_header) = _red;
                _M_root() = 0;
                _M_leftmost() = _m_header;
                _M_rightmost() = _m_header;
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
            _Rb_tree(const _Rb_tree<Key, Value, KeyOfValue, Compare, _Alloc> & _x) // _Rb_tree ??????????????? ????????? ????????? ??????????????????
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
            _Rb_tree<Key, Value, KeyOfValue, Compare, _Alloc>&
            operator=(const _Rb_tree<Key, Value, KeyOfValue, Compare, _Alloc> & _x)
            {
                if (_M_root())
                    clear() ;
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
                return *this;
            }

            public :
                Compare key_comp() const { return _m_key_compare; }
                iterator begin() { return _M_leftmost(); }
                const_iterator begin() const { return _M_leftmost(); }
                iterator end() { return _m_header; }
                const_iterator end() const { return _m_header; }
                reverse_iterator rbegin() { return reverse_iterator(end()); }
                reverse_iterator rend() { return reverse_iterator(begin()); }
                const_reverse_iterator cbegin() const { return const_reverse_iterator(end()); }
                const_reverse_iterator cend() const { return const_reverse_iterator(begin()); }

                bool empty() const { return _m_node_count == 0; }
                size_type size() const { return _m_node_count; }
                size_type max_size() const { return std::min<size_type>(
                    (size_type(-1) / sizeof(_Rb_tree_node)) ,
                    std::numeric_limits<difference_type >::max());} // ?????? ?????? ????????????? ?????? ???????

                void swap(_Rb_tree<Key, Value, KeyOfValue, Compare, _Alloc>& _t)
                {
                    std::swap(_m_header, _t._m_header);
                    std::swap(_m_node_count, _t._m_node_count);
                    std::swap(_m_key_compare, _t._m_key_compare);
                }
            
            public :
                ft::pair<iterator, bool> insert_unique(const value_type& _x)
                {
                    //std::cout << "insert osne value_type" << std::endl;
                    _Link_type _parents  = _m_header;
                    _Link_type _child = _M_root();
                    bool       _comp = true;

                    while (_child != 0)
                    {
                        _parents = _child;
                        _comp = _m_key_compare(KeyOfValue()(_x), _S_key(_child));
                        //std::cout << "bool : " << _comp << std::endl;
                        _child = _comp ? _S_left(_child) : _S_right(_child); 
                    }
                    iterator _j = iterator(_parents);
                    //_comp == true ??????  _child??? _parents??? ????????????????????? nil;
                    //_comp == false ?????? _child??? _parents??? ???????????????????????? nil;
                    if (_comp)// ????????? ?????? ??? ???
                    {
                        if (_j == begin())// _j == leftmost; ????????? ???????????? ??????????????????.
                            return ft::pair<iterator, bool>((_M_insert(_child, _parents, _x)), true);
                        else // ???????????? ?????? ???????????? ????????? ??????????????????.
                            --_j;
                    }
                    if (_m_key_compare(_S_key(_j._m_node), KeyOfValue()(_x))) // --parents??? ??? ???????????? ????????? ????????????. ????????? ??????????????? ????????????????????? ?????????????????????.
                        return ft::pair<iterator, bool>((_M_insert(_child, _parents, _x)), true);
                    return ft::pair<iterator, bool>(_j, false);
                }

                iterator insert_unique(iterator _pos, const value_type& _x)// ????????? ??? ????????? ??????????????? ?????? ????????????????????? insert, ???????????? root?????? search??? ?????? insert 
                {
                    if (_pos._m_node == _m_header->_m_left) // leftmost?????? ?????? (begin)
                    {
                        if (size() > 0 && _m_key_compare(KeyOfValue()(_x), _S_key(_pos._m_node))) // leftmost??? ????????????, _x??? key ?????? leftmost?????? ?????? ???
                            return _M_insert((_Link_type)_pos._m_node, (_Link_type)_pos._m_node, _x); // ?????? ??????
                        else
                            return insert_unique(_x).first;
                    }
                    else if (_pos._m_node == _m_header) // (end) ?????? ?????? 
                    {
                        if (_m_key_compare(_S_key(_M_rightmost()), KeyOfValue()(_x)))//????????? ??? ????????? ??? ???
                            return _M_insert(0, _M_rightmost(), _x); // ????????? ??? ??????
                        else
                            return insert_unique(_x).first;
                    }
                    else // begin??? end??? ?????? ????????? ?????? ????????? ???;
                    {
                        iterator _before = _pos;
                        -- _before;
                        if (_m_key_compare(_S_key(_before._m_node), KeyOfValue()(_x))
                            && _m_key_compare(KeyOfValue()(_x), _S_key(_pos._m_node)))
                        {
                            if (_S_right(_before._m_node) == 0 )// b ??? p??? ??????
                                return _M_insert(0, (_Link_type)_before._m_node, _x);//before??? ????????? ??????.
                            else
                                return _M_insert((_Link_type)_pos._m_node, (_Link_type)_pos._m_node, _x);
                        }
                        else
                            return insert_unique(_x).first;
                    }
                }

                template<class _Iter>
                void insert_unique(_Iter _first, _Iter _last) // first ??????  last?????? ??????.
                {
                    //std::cout << "iter iter insert" << std::endl;
                    for (; _first != _last; ++_first)
                        insert_unique(*_first);
                }

                void        erase(iterator _pos)
                {
                    //std::cout << "erase pos" << std::endl;
                    _Link_type _y = (_Link_type) iterator::_Rb_tree_rebalance_erase(_pos._m_node
                    ,_m_header->_m_parents
                    ,_m_header->_m_left
                    ,_m_header->_m_right);
                    
                    _M_destroy_node(_y);
                    --_m_node_count;
                }
                size_type   erase(const key_type& _x)
                {
                    //std::cout << "erase key_type" << std::endl;
                    ft::pair<iterator, iterator> _p = equal_range(_x);
                    iterator temp = _p.first;
                    size_type _n = 0;
                    while (temp != _p.second)
                    {
                        ++ temp;
                        ++ _n;
                    }
                    erase(_p.first, _p.second);
                    return _n;
                }
                void        erase(iterator _first, iterator _last)
                {
                    if (_first == begin() && _last == end())
                        clear();
                    else
                        while (_first != _last)
                            erase(_first++);
                }
                void        clear()
                {
                    if (_m_node_count != 0)
                    {
                        _M_erase(_M_root());
                        _M_leftmost() = _m_header;
                        _M_root() = 0;
                        _M_rightmost() = _m_header;
                        _m_node_count = 0;
                    }
                }

                public :
                    iterator find(const key_type& _x)
                    {
                        iterator it = lower_bound(_x);
                        if (it == end() || _m_key_compare(_x, _S_key(it._m_node)))
                            return end();
                        else
                            return it;
                    }
                    const_iterator find(const key_type& _x) const
                    {
                        const_iterator it = lower_bound(_x);
                        if (it == end() || _m_key_compare(_x, _S_key(it._m_node)))
                            return end();
                        else
                            return it;
                    }
                    size_type count(const key_type& _x) const
                    {
                        ft::pair<const_iterator, const_iterator> _p = equal_range(_x);
                        size_type _n = 0;
                        const_iterator _first = _p.first;
                        while (_first != _p.second)
                        {
                            ++ _first;
                            ++ _n;
                        }
                        return _n;
                    }

                    iterator lower_bound(const key_type& _x)// ????????? ???????????? ???????????? ????????? ?????? ?????? ????????? key??? iter
                    {
                        _Link_type _p = _m_header;
                        _Link_type _c = _M_root();
                        while (_c != 0)
                        {
                            if (!_m_key_compare(_S_key(_c), _x)) // _c >= _x;
                            {
                                _p = _c;
                                _c = _S_left(_c);
                            }
                            else // _x < _c;
                                _c = _S_right(_c);
                        }
                        return iterator(_p);
                    }
                    const_iterator lower_bound(const key_type& _x) const
                    {
                        _Link_type _p = _m_header;
                        _Link_type _c = _M_root();

                        while (_c != 0)
                        {
                            if (!_m_key_compare(_S_key(_c), _x)) // _c >= _x;
                            {
                                _p = _c;
                                _c = _S_left(_c); // _c->_m_right;
                            }
                            else // _x < _c;
                                _c = _S_right(_c); // _c->_m_left;
                        }
                        return const_iterator(_p);
                    }
                    iterator upper_bound(const key_type& _x) // ???????????? ??? ????????? key?????? ????????? ?????? iter 
                    {
                        _Link_type _p = _m_header;
                        _Link_type _c = _M_root();

                        while (_c != 0)
                        {
                            if (_m_key_compare(_x, _S_key(_c)))
                            {
                                _p = _c;
                                _c = _S_left(_c);
                            }
                            else
                                _c = _S_right(_c);
                        }
                        return iterator(_p);
                    }
                    const_iterator upper_bound(const key_type& _x) const
                    {
                        _Link_type _p = _m_header;
                        _Link_type _c = _M_root();

                        while (_c != 0)
                        {
                            if (_m_key_compare(_x, _S_key(_c)))
                            {
                                _p = _c;
                                _c = _S_left(_c);
                            }
                            else
                                _c = _S_right(_c);
                        }
                        return const_iterator(_p);
                    }
                    ft::pair<iterator, iterator> equal_range(const key_type& _x)// iter1.key <= _x < iter2.key
                    {
                        return ft::make_pair(lower_bound(_x), upper_bound(_x));
                    }
                    ft::pair<const_iterator, const_iterator> equal_range(const key_type& _x) const
                    {
                         return ft::make_pair(lower_bound(_x), upper_bound(_x));
                    }
    };
    template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>
    inline bool 
    operator==(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
            const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y)
    {
    return __x.size() == __y.size() &&
            ft::equal(__x.begin(), __x.end(), __y.begin());
    }

    template <class _Key, class _Value, class _KeyOfValue, 
            class _Compare, class _Alloc>
    inline bool 
    operator<(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
            const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y)
    {
    return ft::lexicographical_compare(__x.begin(), __x.end(), 
                                    __y.begin(), __y.end());
    }

    template <class _Key, class _Value, class _KeyOfValue, 
            class _Compare, class _Alloc>
    inline bool 
    operator!=(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
            const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y) {
    return !(__x == __y);
    }

    template <class _Key, class _Value, class _KeyOfValue, 
            class _Compare, class _Alloc>
    inline bool 
    operator>(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
            const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y) {
    return __y < __x;
    }

    template <class _Key, class _Value, class _KeyOfValue, 
            class _Compare, class _Alloc>
    inline bool 
    operator<=(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
            const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y) {
    return !(__y < __x);
    }

    template <class _Key, class _Value, class _KeyOfValue, 
            class _Compare, class _Alloc>
    inline bool 
    operator>=(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
            const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y) {
    return !(__x < __y);
    }  
};