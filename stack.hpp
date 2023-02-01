/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhjeon <junhjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 21:11:18 by junhjeon          #+#    #+#             */
/*   Updated: 2023/01/26 17:42:46 by junhjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "vector.hpp"

namespace ft
{
    template <typename T, typename Container_type = ft::vector<T> >
    class stack
    {
        public :
            typedef T               value_type;
            typedef Container_type  container_type;
            typedef std::size_t     size_type;
        protected :
            container_type c;
        public :
            explicit stack (const container_type& ctnr = container_type())
            { c = ctnr; }
            ~stack(){}
            stack<T, container_type>& operator = ( const stack<T, container_type>& m)
            { c = m.c; return *this;}
            T&	top( void )
            {
                return ( c.back() );
            }
            const T& top( void ) const
            {
                return ( c.back() );
            }
            void pop( void )
            {
                c.pop_back();
            }
            size_type size() const
            {
                return c.size();
            }
            bool empty() const
            {
                return c.empty();
            }
            void push(const value_type& val)
            {
                c.push_back(val);
            }
        template <typename _T, typename _C>
        friend bool operator==(const stack<_T, _C>& lhs, const stack<_T, _C>& rhs);
        template <typename _T, typename _C>
        friend bool operator>(const stack<_T, _C>& lhs, const stack<_T, _C>& rhs);
    };
    template <typename _T, typename _C>
    bool operator==(const stack<_T, _C>& lhs, const stack<_T, _C>& rhs)
    { return lhs.c == rhs.c; }
    template <typename _T, typename _C>
    bool operator>(const stack<_T, _C>& lhs, const stack<_T, _C>& rhs)
    { return lhs.c > rhs.c; }


    template <typename _T, typename _C>
    bool operator!=(const stack<_T, _C>& lhs, const stack<_T, _C>& rhs)
    { return !(lhs == rhs); }
    template <typename _T, typename _C>
    bool operator>=(const stack<_T, _C>& lhs, const stack<_T, _C>& rhs)
    { return !(lhs < rhs); }
    template <typename _T, typename _C>
    bool operator<(const stack<_T, _C>& lhs, const stack<_T, _C>& rhs)
    { return rhs > lhs; }
    template <typename _T, typename _C>
    bool operator<=(const stack<_T, _C>& lhs, const stack<_T, _C>& rhs)
    { return !(lhs > rhs); }
}


