/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhjeon <junhjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 17:35:10 by junhjeon          #+#    #+#             */
/*   Updated: 2023/01/12 17:35:14 by junhjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include "Utils.hpp"
#include "tree.hpp"
#include "Iterator.hpp"

namespace ft
{
    template< class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair< const Key, T> > >
    class map
    {
        protected :
            template < class _key, class _value >
            class keyOfValue
            {
                public :
                    typedef _value      value;
                public :
                    keyOfValue(){}
                    const _key& operator()(const ft::pair<const _key, _value>& _x) const
                    { return _x.first; }
            };
        public :    
            typedef Key                                                     key_type;
            typedef T                                                       mapped_type;
            typedef typename ft::pair<const Key, T>                         value_type;
            typedef std::size_t                                             size_type;
            typedef std::ptrdiff_t                                          difference_type;
            typedef Compare                                                 key_compare;
            typedef Allocator                                               allocator_type;
            typedef value_type&                                             reference;
            typedef const value_type&                                       const_referecne;
            typedef typename Allocator::pointer                             pointer;
            typedef typename Allocator::const_pointer                       const_pointer;
            typedef _Rb_tree<Key, value_type, map::keyOfValue<Key, T>, Compare, Allocator>  _Rep_type;
            typedef	typename _Rep_type::iterator                            iterator;
            typedef	typename _Rep_type::const_iterator                      const_iterator;
            typedef	typename ft::reverse_iterator<iterator>			    	reverse_iterator;		
            typedef	typename ft::reverse_iterator<const_iterator>	    	const_reverse_iterator;
        
            class value_compare 
            {
                friend class map<Key, T, Compare, Allocator>;
                protected :
                    Compare comp;
                    value_compare(Compare _c) : comp(_c){}
                public :
                    bool operator()(const value_type& _x, const value_type& _y) const //key compare
                     { return comp(_x.first, _y.first); }
            };

        private :
            _Rep_type _t;

        public :
            map() : _t(Compare(), allocator_type()){}
            explicit map(const Compare& _comp, const allocator_type& _a = allocator_type()) : _t(_comp, _a){}
            map(const map<Key, T, Compare, Allocator>& _x) { _t = _x._t; }
            template< class InputIt >

            map( InputIt first, InputIt last,
                const Compare& comp = Compare(),
                const Allocator& alloc = Allocator() ) : _t(Compare(), allocator_type())//?
            { (void)comp;(void)alloc; _t.insert_unique(first, last); }

            ft::map<Key, T, Compare, Allocator>&
            operator=(const ft::map< Key, T, Compare, Allocator >& _x){ _t = _x._t; return *this; }

        //////////////////
        // Element access
        /////////////////
        public :
            T&  at( const Key& key)
            {
                (void) key;
            }
            const T& at( const Key& key) const
            {
                (void) key;
            }
            T& operator[] (const Key& key)
            {
                iterator it = _t.lower_bound(key);
                if (it == end() || key_comp()(key, (*it).first))
                    it = insert(it, value_type(key, T()));
                return (it -> second);
            }

        /////////////////
        // iterators
        /////////////////

        iterator				begin(){ return iterator(_t.begin());}
        const_iterator          begin() const { return const_iterator(_t.begin()); }
		iterator				end(){ return iterator(_t.end());};
        const_iterator          end() const { return const_iterator(_t.end()); }
		reverse_iterator		rbegin(){ return reverse_iterator(end()); }
		reverse_iterator		rend(){ return reverse_iterator(begin()); }

       ////////////////
        // Capacity
        ////////////////

        bool empty() const { return _t.empty(); };
        size_type size() const { return _t.size(); }
        size_type max_size() const { return _t.max_size(); };

        ////////////////
        // Modifiers
        ////////////////

        void clear() { _t.clear(); }
        ft::pair<iterator, bool> insert( const value_type& value ) { return _t.insert_unique(value); }
        iterator insert( iterator pos, const value_type& value ) { return _t.insert_unique(pos, value); }
        template< class InputIt >
        void insert( InputIt first, InputIt last ){ _t.insert_unique(first, last); }
        void erase( iterator pos ) { _t.erase(pos); }
        void erase( iterator first, iterator last ){ _t.erase(first, last); }
        size_type erase (const Key& key ) { return _t.erase(key); }
        void swap( map& other ) { _t.swap(other._t); }

        ////////////////
        // LookUp
        ////////////////

        size_type count( const Key& key ) const{ return _t.count(key); }
        iterator find( const Key& key ) { return _t.find(key); }
        const_iterator find( const Key& key ) const { return _t.find(key); }
        ft::pair<iterator, iterator> equal_range( const Key& key ) { return _t.equal_range(key); }
        ft::pair<const_iterator, const_iterator> equal_range( const Key& key ) const { return _t.equal_range(key); }
        iterator lower_bound( const Key& key ) { return _t.lower_bound(key); }
        const_iterator lower_bound( const Key& key ) const { return _t.lower_bound(key); }
        iterator upper_bound( const Key& key ) { return _t.upper_bound(key); }
        const_iterator upper_bound( const Key& key ) const { return _t.upper_bound(key); }

        ///////////////
        // Observers
        ///////////////

        key_compare key_comp() const { return _t.key_comp(); }
        value_compare value_comp() const { return value_compare(_t.key_comp()); }

        // friend 선언의 차이점?
        template <class _K1, class _T1, class _C1, class _A1>
        friend bool operator== (const map<_K1, _T1, _C1, _A1>&,
                                const map<_K1, _T1, _C1, _A1>&);
        template <class _K1, class _T1, class _C1, class _A1>
        friend bool operator< (const map<_K1, _T1, _C1, _A1>&,
                                const map<_K1, _T1, _C1, _A1>&);
    };

    template <class _Key, class _Tp, class _Compare, class _Alloc>
    inline bool operator==(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                        const map<_Key,_Tp,_Compare,_Alloc>& __y) {
    return __x._t == __y._t;
    }

    template <class _Key, class _Tp, class _Compare, class _Alloc>
    inline bool operator<(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                        const map<_Key,_Tp,_Compare,_Alloc>& __y) {
    return __x._t < __y._t;
    }

    template <class _Key, class _Tp, class _Compare, class _Alloc>
    inline bool operator!=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                        const map<_Key,_Tp,_Compare,_Alloc>& __y) {
    return !(__x == __y);
    }

    template <class _Key, class _Tp, class _Compare, class _Alloc>
    inline bool operator>(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                        const map<_Key,_Tp,_Compare,_Alloc>& __y) {
    return __y < __x;
    }

    template <class _Key, class _Tp, class _Compare, class _Alloc>
    inline bool operator<=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                        const map<_Key,_Tp,_Compare,_Alloc>& __y) {
    return !(__y < __x);
    }

    template <class _Key, class _Tp, class _Compare, class _Alloc>
    inline bool operator>=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                        const map<_Key,_Tp,_Compare,_Alloc>& __y) {
    return !(__x < __y);
    }

    template <class _Key, class _Tp, class _Compare, class _Alloc>
    inline void swap(map<_Key,_Tp,_Compare,_Alloc>& __x, 
                    map<_Key,_Tp,_Compare,_Alloc>& __y) {
    __x.swap(__y);
    }
};