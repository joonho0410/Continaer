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

        void _M_increment()//iterator 증가.
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
                while (_m_node == _parents->_m_right)//내가 parents의 왼쪽이 될 떄 까지 올라간다.
                {
                    _m_node = _parents;
                    _parents = _parents -> _m_parents;
                }
                if (_m_node->_m_right != _parents)//? 
                    _m_node = _parents;
            }
        }
        
        void _M_decrement()//iterator 감소.
        {
            if (_m_node->_m_color ==_red && _m_node->_m_parents->_m_parents == _m_node)
                _m_node = _m_node->_m_right; // _m_node가 header node 일때 right_max return;
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

        // template <class _Value, class _Ref, class _Ptr>
        // inline bool operator==(const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __x,
        //             const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __y) {
        // return __x._m_node == __y._m_node;
        // }

        // template <class _Value>
        // inline bool operator==(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
        //             const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) {
        // return __x._m_node == __y._m_node;
        // }

        // template <class _Value>
        // inline bool operator==(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
        //             const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y) {
        // return __x._m_node == __y._m_node;
        // }

        // template <class _Value, class _Ref, class _Ptr>
        // inline bool operator!=(const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __x,
        //             const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __y) {
        // return __x._m_node != __y._m_node;
        // }

        // template <class _Value>
        // inline bool operator!=(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
        //             const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) {
        // return __x._m_node != __y._m_node;
        // }

        // template <class _Value>
        // inline bool operator!=(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
        //             const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y) {
        // return __x._m_node != __y._m_node;
        // }

        static inline void
        _Rb_tree_rotate_left(_Rb_tree_node_base* _x, _Rb_tree_node_base* _root)
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
        _Rb_tree_rotate_right(_Rb_tree_node_base* _x, _Rb_tree_node_base* _root)
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
        _Rb_tree_rebalance_insert(_Rb_tree_node_base* _x, _Rb_tree_node_base* _root)// _x -> 추가된 node, _root -> 루트노드.
        {
            _x->_m_color = _red; // insert node 는 항상 red;
            while (_x != _root && _x->_m_parents->_m_color == _red)//RR 위반의 경우. 부모가 black일 경우에는 본인이 root일 경우에 색상변경외에는 위반이 발생하지않는다.
            {
                if (_x->_m_parents == _x->_m_parents->_m_parents->_m_left)//부모가 할아버지의 왼쪽 자식인 경우;
                {
                    if (_x->_m_parents->_m_parents->_m_right && 
                        _x->_m_parents->_m_parents->_m_right->_m_color == _red)//삼촌의 color가 레드일때 case1;
                    {
                        _x->_m_parents->_m_color = _black; // 부모와 삼촌의 색깔을 모두 black으로 바꿔줌;
                        _x->_m_parents->_m_parents->_m_right->_m_color = _black;
                        _x->_m_parents->_m_parents->_m_color = _red;
                        _x = _x->_m_parents->_m_parents; // 할아버지노드에서부터 다시한번 위반검사.
                    }
                    else // 부모는 할아버지의 왼쪽자식이면서 red, 삼촌은 black인 경우  
                    {
                        if (_x->_m_parents->_m_right == _x) // _x가 부모의 오른쪽 자식인 경우 case2
                        {
                            _x = _x->_m_parents;
                            _Rb_tree_rotate_left(_x, _root);// p[z]에 대해서 rotate
                        }
                        // 부모의 왼쪽 자식인 경우 case3
                        _x->_m_parents->_m_color = _black;
                        _x->_m_parents->_m_parents->_m_color = _red;
                        _Rb_tree_rotate_right(_x->_m_parents->_m_parents, _root);//p[p[z]]에 대해서 rotate
                    }
                }
                else // 부모가 할아버지의 오른쪽 자식인 경우. 위의 케이스에서 left와 right를 전부 반대로 해주면 된다.
                {
                    
                    if (_x->_m_parents->_m_parents->_m_left && 
                        _x->_m_parents->_m_parents->_m_left->_m_color == _red)//삼촌의 color가 레드일때 case1;
                    {
                        _x->_m_parents->_m_color = _black; // 부모와 삼촌의 색깔을 모두 black으로 바꿔줌;
                        _x->_m_parents->_m_parents->_m_left->_m_color = _black;
                        _x->_m_parents->_m_parents->_m_color = _red;
                        _x = _x->_m_parents->_m_parents; // 할아버지노드에서부터 다시한번 위반검사.
                    }
                    else // 부모는 할아버지의 왼쪽자식이면서 red, 삼촌은 black인 경우  
                    {
                        if (_x->_m_parents->_m_left == _x) // _x가 부모의 왼쪽 자식인 경우 case2
                        {
                            _x = _x->_m_parents;
                            _Rb_tree_rotate_right(_x, _root);// p[z]에 대해서 rotate
                        }
                        // 부모의 왼쪽 자식인 경우 case3
                        _x->_m_parents->_m_color = _black;
                        _x->_m_parents->_m_parents->_m_color = _red;
                        _Rb_tree_rotate_left(_x->_m_parents->_m_parents, _root);//p[p[z]]에 대해서 rotate
                    }
                }
            }
            _root->_m_color = _black;//부모가 black인 경우에 위반이 발생하지 아므로 바로 넣어주면된다. _x가 _root일 경우를 생각해서 _root을 black으로 바꿔준다.
        }

        static inline _Rb_tree_node_base*
        _Rb_tree_rebalance_erase(_Rb_tree_node_base* _z,
                                 _Rb_tree_node_base* _root,
                                 _Rb_tree_node_base* _leftmost,
                                 _Rb_tree_node_base* _rightmost)
        {
            _Rb_tree_node_base* _target = _z; // really deleted (succesor)
            _Rb_tree_node_base* _succesor_p = 0; 
            _Rb_tree_node_base* _succesor_c = 0; // succesor's child node;

            //succesor를 찾고 사라지는 노드를 대체해서 노드를 잇는 작업.

            if (_target->_m_left == 0)// 왼쪽이 비어있으면 오른쪽이 올라오는데 null이던 있던 상관없이 그냥 붙여주면된다.
                _succesor_c = _target->_m_right;
            else if (_target->_m_right == 0) //왼쪽 오른쪽이 모두 비어있으면 둘중하나 아무거나 올려도 null 아님 ?
                _succesor_c = _target->_m_left;
            else // 왼쪽도 비어있지 않고, 오른쪽도 비어있지 않을 때
            {
                _target = _target->_m_right;
                while (_target->_m_left != 0)
                    _target = _target->_m_left;
                _succesor_c = _target->_m_right; // 왼쪽을 계속 타고왔으므로 당연히 오른쪽이 있거나 혹은 없거나.
            }
            if (_target != _z) //왼쪽과 오른쪽이 둘다 존재할 경우 ; availalbe succesor;
            {
                //target을 _z를 대체해서 왼쪽을 서로 이어줬다.
                _z->_m_left->_m_parents = _target;// 사라질 노드의 자식과 대체할 노드를 이어주는역할.
                _target->_m_left = _z->_m_left; //_target 의 왼쪽은 존재할 수 없다 위의 조건에서 왼쪽 끝까지 내려왔으므로;
                if (_target != _z->_m_right) // 대체되는 노드가 사라지는 노드의 오른쪽 자식이 아닌경우.
                {
                    _succesor_p = _target->_m_parents;
                    if (_succesor_c) _succesor_c->_m_parents = _target->_m_parents; //오른쪽 자식이있다면 대체되는 노드의 부모와 연결
                    _target->_m_parents->_m_left = _succesor_c; // 대체되는 노드의 부모와 연결됨.
                    _target->_m_right = _z->_m_right; // 사라지는 노드의 오른쪽과 연결
                    _z->_m_right->_m_parents = _target; //사라지는 노드의 오른쪽과 연결2
                } 
                else
                    _succesor_p = _target; // 대체되는 노드의 오른쪽을 연결할 필요가 없어진다.
                if(_root == _z) // 만약 _root이 지워진다면 _root의 자리를 계승
                    _root = _target;
                else if (_z->_m_parents->_m_left == _z)// 내가 왼쪽 자식이였을 때, 사라지는 노드의 부모의 왼쪽과 연결
                    _z->_m_parents->_m_left = _target;
                else// 사라지는 노드의 부모의 오른쪽과 연결;
                    _z->_m_parents->_m_right = _target;
                _target->_m_parents = _z->_m_parents; // 대체되는 노드의 부모를 사라지는 노드의 부모와 연결;
                std::swap(_target->_m_color, _z->_m_color); // 색깔 대체 _z의 컬러는 사라지는 _target의 색깔이 된다.
                _target = _z; // 사라지는 노드를 다시 설정
            }
            else // 왼쪽이나 오른쪽 둘중하나가 존재하지 않는 경우 왼쪽이나 오른쪽 자식을 그대로 올려오게된다. 여긴 왜 컬리스왑이없지? -> _target == _z 로 결국 사라질 컬러와 같다
            {
                //succesor_c 가 _target의 역할을 하고 있다. 즉 대체하는 노드이다. 이 경우 _succesor은 null이 될 수 있다.
                if (_succesor_c) _succesor_c->_m_parents = _z->_m_parents;
                if (_z == _root)
                    _root = _succesor_c;
                else if (_z->_m_parents->_m_left == _z)
                    _z->_m_parents->_m_left = _succesor_c;
                else
                    _z->_m_parents->_m_right = _succesor_c;
                if (_leftmost == _z) // 삭제되는 z가 left most 즉 트리의 왼쪽 끝 일때
                    if (_z->_m_right == 0) // 오른쪽도 존재하지 않는다면 부모가 왼쪽 끝이 된다.
                        _leftmost = _z->_m_parents;
                    else // 대체하는 노드의 왼쪽 끝이 최댓값이 된다.
                        _leftmost = _Rb_tree_node_base::_S_minimum(_succesor_c);
                if (_rightmost == _z) // 왼쪽과 마찬가지
                    if(_z->_m_left == 0)
                        _rightmost = _z->_m_parents;
                    else // 대체하는 노드의 오른쪽 끝이 최댓값이 된다.
                        _rightmost = _Rb_tree_node_base::_S_maximum(_succesor_c);
                _target = _z; // 사라지는 노드는 _target이다.
            }
            if (_target->_m_color == _black) // _target의 컬러가 red면 생각하지않고 동작해도 된다. 영향 x
            {
                //결국 사라지고 나고 대체가 끝나게 되면 트리에서 달라진점은 succesor가 빠지고 succesor의 child가 올라온 부분이 된다.
                while (_succesor_c != _root && (_succesor_c == 0 || _succesor_c->_m_color == _black))
                { // 후계자가 root이 아니고 후계자의 컬러가 black 즉 사라지는색이 black일 때
                    _Rb_tree_node_base* _w;
                    if (_succesor_c == _succesor_p->_m_left)
                    {
                        _w = _succesor_p->_m_right;
                        if (_w->_m_color == _red)//case 1. 형제노드가 red 인 경우.
                        {
                            _w->_m_color = _black;
                            _succesor_c->_m_parents->_m_color = _red;
                            _Rb_tree_rotate_left(_succesor_c->_m_parents, _root);
                            _w = _succesor_c->_m_parents->_m_right;
                        }
                        if ((_w->_m_left == 0 || _w->_m_left->_m_color == _black) && // case2. 형제노드가 black, 형제노드의 자식들이 모두 black;
                        (_w->_m_right == 0 || _w->_m_right->_m_color == _black)) // nil node is black
                        {
                            _w->_m_color = _red;
                            _succesor_c = _succesor_c->_m_parents;
                        }
                        else if (_w->_m_right == 0 || _w->_m_right->_m_color == _black)//case3. 형제노드 black, 형제노드의 왼쪽이 red
                        {
                            if (_w->_m_left) _w->_m_left->_m_color = _black;
                            _w->_m_color = _red;
                            _Rb_tree_rotate_right(_succesor_c->_m_parents, _root);
                            _w = _succesor_c->_m_parents->_m_right;
                        }
                        _w->_m_color = _succesor_c->_m_color;//case4. 형제노드 black, 형제노드의 오른쪽이 red
                        _succesor_c->_m_parents->_m_color = _black;
                        if (_w->_m_right) _w->_m_right->_m_color = _black;
                        _Rb_tree_rotate_left(_succesor_c->_m_parents, _root);
                        _succesor_c = _root;
                    }
                    else // 좌우반전
                    {
                        _w = _succesor_p->_m_left;
                        if (_w->_m_color == _red)
                        {
                            _w->_m_color = _black;
                            _succesor_c->_m_parents->_m_color = _red;
                            _Rb_tree_rotate_right(_succesor_c->_m_parents, _root);
                            _w = _succesor_c->_m_parents->_m_left;
                        }
                        if ((_w->_m_right == 0 || _w->_m_right->_m_color == _black)
                            && (_w->_m_left == 0 || _w->_m_left->_m_color == _black))
                        {
                            _w->_m_color = _red;
                            _succesor_c = _succesor_c->_m_parents;
                        }
                        else if (_w->_m_left->_m_color == _black)
                        {
                            if (_w->_m_right) _w->_m_right->_m_color = _black;
                            _w->_m_color = _red;
                            _Rb_tree_rotate_left(_succesor_c->_m_parents, _root);
                            _w = _succesor_c->_m_parents->_m_left;
                        }
                        _w->_m_color = _succesor_c->_m_color;
                        _succesor_c->_m_parents->_m_color = _black;
                        if (_w->_m_left) _w->_m_left->_m_color = _black;
                        _Rb_tree_rotate_right(_succesor_c->_m_parents, _root);
                        _succesor_c = _root;
                    }
                }
                if (_succesor_c) _succesor_c->_m_color = _black;
            }
            return _target;
        }
    };

    //////////////////////////////////////////////////////
	/////     rb_tree 	         
	//////////////////////////////////////////////////////

    template <class _Tp, class _Alloc>
    class _Rb_tree_base
    {
        public : 
            typedef typename _Alloc::template rebind< _Rb_tree_node<_Tp> >::other allocator_type; 
        
        public :
            _Rb_tree_node<_Tp>*         _m_header; // 트리의 root

        public :
            _Rb_tree_base (const allocator_type& _a) : _m_header(0)
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

    
    template <class Key, class Value, class KeyOfValue, class Compare, class _Alloc = std::allocator<Value> >
    class _Rb_tree : public _Rb_tree_base<Value, _Alloc>
    {
        public :
            typedef _Rb_tree_base<Value, _Alloc> _Base;
            
        protected :
            typedef _Rb_tree_node_base*     _Base_ptr; // node_base pointer, 해당노드의 부모,자식,컬러등의 정보를 저장.
            typedef _Rb_tree_node<Value>    _Rb_tree_node;
            typedef _color                  _color;
        
        public :    
            typedef Key                 key_type;
            typedef Value               value_type; // ft::pair<key, value>;
            typedef value_type*         pointer;
            typedef const value_type*   const_pointer;
            typedef value_type&         reference;
            typedef const value_type&   const_reference;
            typedef _Rb_tree_node*      _Link_type; // node pointer, value 값을 저장.
            typedef size_t              size_type;
            typedef ptrdiff_t           difference_type;

            typedef _Rb_tree_iterator<value_type, reference, pointer>                    iterator;
            typedef _Rb_tree_iterator<value_type, const_reference, const_pointer>        const_iterator;
            typedef ft::reverse_iterator<iterator>                                       reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>                                 const_reverse_iterator;

            typedef typename _Rb_tree_base<Value, _Alloc>::allocator_type   allocator_type; // Node구조체(_Rb_tree_node)의 allocator;
            allocator_type get_allocator() const { return allocator_type(); } 

        public :
            iterator
            _M_insert(_Link_type _child, _Link_type _parents, const value_type& _v)
            {
                // _child 새로운 _v가 들어갈 자리
                _Link_type _tmp;
                // _y = _m_header 이거나  _x값이 존재,  _v.value < _y.value;
                // _child != 0의 의미? 무조건 빈자리를 찾아서  _child에 넣는데 빈자리가 아닌 경우가 있을까요?
                // _parents == _m_header 트리가 비어있을 때
                if (_parents == _m_header || _child != 0 || _m_key_compare(KeyOfValue()(_v), _S_key(_parents)))
                {
                    //parents 의 왼쪽에 붙는 경우.
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
                else // _v.value > _y.value
                {
                    //parents의 오른쪽에 붙는 경우.
                    _tmp = _M_create_node(_v);
                    _S_right(_parents) = _tmp;
                    if (_parents == _M_rightmost())
                        _M_rightmost() = _tmp;
                }
                _S_parents(_tmp) = _parents; // 
                _S_left(_tmp) = 0; // 2개의 닐노드 
                _S_right(_tmp) = 0;
                iterator::_Rb_tree_rebalance_insert(_tmp, _m_header->_m_parents);
                ++_m_node_count;
                return iterator(_tmp);
            }

            void
            _M_erase(_Link_type& _x)
            {
                if (_x->_m_left != 0)
                    _M_erase(_x->_m_left);
                if (_x->_m_right != 0)
                    _M_erase(_x->_m_right);
                _M_destroy_node(_x);
            }

            _Link_type&
            _M_copy(_Link_type& _x, _Link_type& _p)// p에다가  x의 트리를 복사한다. 
            {
                _Link_type _top = _M_clone_node(_x);
                _top->_m_parents = _p;
                if (_x -> _m_right) //오른쪽을 타고 가면서 모두 복사;
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
            _M_clone_node(_Link_type _x) // color와 가지고 있는 변수값을 복사하고 나머지는 초기화.
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
            { return (_Link_type&) _m_header -> _m_parent; }
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
                { return _x -> KeyOfValue()(_S_value(_x)); }
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
                { return (_Link_type)  _Rb_tree_node_base::_S_minimum(_x); }
            static _Link_type _S_maximum(_Link_type _x)
                { return (_Link_type) _Rb_tree_node_base::_S_maximum(_x); }

        private :
            void _M_empty_initialize() // 각각의 레퍼런스를 불러서 초기화를 해준다.
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
            _Rb_tree(const _Rb_tree<Key, Value, KeyOfValue, Compare, _Alloc> & _x) // _Rb_tree 복사하는거 같은데 이해를 하지못하겠음
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
                size_type max_size() const { return sizetype(-1); } // ? max_size가 대체뭐냐

                void swap(_Rb_tree<Key, Value, KeyOfValue, Compare, _Alloc>& _t)
                {
                    std::swap(_m_header, _t._m_header);
                    std::swap(_m_node_count, _t._m_node_count);
                    std::swap(_m_key_compare, _t._m_key_compare);
                }
            
            public :
                ft::pair<iterator, bool> insert_unique(const value_type& _x)
                {
                    _Link_type _parents  = _m_header;
                    _Link_type _child = _M_root();
                    bool       _comp = true;
                    while (_child != 0)
                    {
                        _parents = _child;
                        _comp = _m_key_compare(KeyOfValue()(_x), _S_key(_child));
                        _child = _comp ? _S_left(_child) : _S_right(_child); 
                    }
                    iterator _j = iterator(_parents);
                    //_comp == true 이면  _child는 _parents의 왼쪽자식이면서 nil;
                    //_comp == false 이면 _child는 _parents의 오른쪽자식이면서 nil;
                    if (_comp)// 왼쪽에 삽입 될 때
                        if (_j == begin())// _j == leftmost; 생각할 필요없이 삽입하면된다.
                            return ft::pair<iterator, bool>((_M_insert(_child, _parents, _x)), true);
                        else // 중복하는 값이 들어오지 않는지 확인해야한다.
                            --_j;
                    if (_m_key_compare(_S_key(_j._m_node), KeyOfValue()(_x))) // --parents를 한 값보다는 반드시 커야한다. 같으면 오른쪽으로 빠져나왔으므로 검사가필요하다.
                        return ft::pair<iterator, bool>((_M_insert(_child, _parents, _x), true));
                    return ft::pair<iterator, bool>(_j, false);
                }

                iterator insert_unique(iterator _pos, const value_type& _x)// 제대로 된 위치로 들어왔다면 해당 이터레이터부터 insert, 아니라면 root부터 search를 해서 insert 
                {
                    if (_pos._m_node == _m_header->_m_left) // leftmost부터 삽입 (begin)
                    {
                        if (size() > 0 && _m_key_compare(KeyOfValue()(_x), _S_key(_pos._m_node))) // leftmost가 존재하고, _x의 key 값이 leftmost보다 작을 때
                            return _M_insert(_pos._m_node, _pos._m_node, _x); // 왼쪽 삽입
                        else
                            return insert_unique(_x).first;
                    }
                    else if (_pos._m_node == _m_header) // (end) 부터 삽입 
                    {
                        if (_m_key_compare(_S_key(_M_rightmost()), KeyOfValue()(_x)))//오른쪽 끝 값보다 클 때
                            return _M_insert(0, _M_rightmost(), _x); // 오른쪽 끝 삽입
                        else
                            return insert_unique(_x).first;
                    }
                    else // begin과 end가 아닌 뭣인가 부터 삽입할 때;
                    {
                        iterator _before = _pos;
                        -- _before;
                        if (_m_key_compare(_S_key(_before._m_node), KeyOfValue()(_x))
                            && _m_key_compare(KeyOfValue()(_x), _S_key(_pos._m_node)))
                        {
                            if (_S_right(_before._m_node) == 0 )// b 가 p의 왼쪽
                                return _M_insert(0, _before._m_node, _x);//before의 오른쪽 삽입.
                            else
                                return _M_insert(_pos._m_node, _pos._m_node, _x);
                        }
                        else
                            return insert_unique(_x).first;
                    }
                }

                iterator insert_unique(iterator _first, iterator _last) // first 부터  last까지 삽입.
                {
                    for (; _first != _last; ++_first)
                        insert_unique(*_first);
                }

                void        erase(iterator _pos)
                {
                    _Link_type _y = (_Link_type) iterator::_Rb_tree_rebalance_erase(_pos._m_node
                    ,_m_header->_m_parents
                    ,_m_header->_m_left
                    ,_m_header->_m_right);

                    _M_destroy_node(_y);
                    --_m_node_count;
                }
                size_type   erase(const key_type& _x);
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
                        _m_root() = 0;
                        _M_rightmost() = _m_header;
                        _m_node_count = 0;
                    }
                }

                public :
                    iterator find(const key_type& _x)
                    {
                        iterator it = lower_bound(_x);
                        if (p == end() || _m_key_compare(_x, p._m_node))
                            return end();
                        else
                            return it;
                    }
                    const_iterator find(const key_type& _x) const
                    {
                        const_iterator it = lower_bound(_x);
                        if (p == end() || _m_key_compare(_x, p._m_node))
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

                    iterator lower_bound(const key_type& _x)// 자신을 포함해서 자신보다 크거나 같은 바로 다음의 key값 iter
                    {
                        _Link_type _p = _m_header;
                        _Link_type _c = _M_root();

                        while (_c != 0)
                        {
                            if (!_m_key_compare(_S_key(_c), _x)) // _c >= _x;
                            {
                                _p = _c;
                                _c = _c->_m_right;
                            }
                            else // _x < _c;
                                _c = _c->_m_left;
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
                                _c = _c->_m_right;
                            }
                            else // _x < _c;
                                _c = _c->_m_left;
                        }
                        return iterator(_p);
                    }
                    iterator upper_bound(const key_type& _x) // 자신보다 큰 다음의 key값을 가지고 있는 iter 
                    {
                        _Link_type _p = _m_header;
                        _Link_type _c = _M_root();

                        while (_c != 0)
                        {
                            if (_m_key_compare(_x, _S_key(_c)))
                            {
                                _p = _c;
                                _c = _c->_m_left;
                            }
                            else
                                _c = _c->_m_right;
                        }
                        return iterator(_p);
                    }
                    const_iterator upper_bound(const key_type& _x) const
                    {
                        _Link_type _p = _m_header;
                        _Link_type _c = _M_root();

                        while (_c != 0)
                        {
                            _p = _c;
                            if (_m_key_compare(_x, _S_key(_c)))
                                _c = _c->_m_left;
                            else
                                _c = _c->_m_right;
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
};