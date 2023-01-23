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
        typedef _Rb_tree<Key, T, const Key, Compare, Allocator>        _Rep_type;
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
        typedef	typename _Rep_type::iterator                            iterator;
		typedef	typename _Rep_type::const_iterator                      const_iterator;
		typedef	typename ft::reverse_iterator<iterator>			    	reverse_iterator;		
		typedef	typename ft::reverse_iterator<const_iterator>	    	const_reverse_iterator;
        

        private :
            _Rep_type _t;
        //////////////////
        // Element access
        /////////////////
        public :
            T&  at( const Key& key);
            const T& at*( const Key& key) const;
            T& operator[] (const Key& key);

        /////////////////
        // iterators
        /////////////////

        iterator				begin(){ return iterator(_t.begin());}
		iterator				end(){ return iterator(_t.end());};
		reverse_iterator		rbegin(){ return reverse_iterator(begin()); }
		reverse_iterator		rend(){ return reverse_iterator(end()); }

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
        ft::pair<iteraotr, bool> insert( const value_type& value )
        iterator insert( iterator pos, const value_type& value );
        template< class InputIt >
        void insert( InputIt first, InputIt last );
        void erase( iterator pos );
        void erase( iterator first, iterator last );
        size_type erase (const Key& key );
        void swap( map& other );

        ////////////////
        // LookUp
        ////////////////

        size_type count( const Key& key ) const;
        iterator find( const Key& key );
        const_iterator find( const Key& key ) const;
        ft::pair<iterator, iterator> equal_range( const Key& key );
        ft::pair<const_iterator, const_iterator> equal_range( const Key& key ) const;
        iterator lower_bound( const Key& key );
        const_iterator lower_bound( const Key& key ) const;
        iterator upper_bound( const Key& key );
        const_iterator upper_bound( const Key& key ) const;

        ///////////////
        // Observers
        ///////////////

        key_compare key_comp() const;
        ft::map::value_compare value_comp() const;





    };

}