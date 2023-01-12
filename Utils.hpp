/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhjeon <junhjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 21:11:18 by junhjeon          #+#    #+#             */
/*   Updated: 2022/12/28 16:46:35 by junhjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <compare>
#include <initializer_list>

#pragma once
namespace	ft
{
	//////////////////////////////////////////////////////
	/////           integral_constant begin	         /////
	//////////////////////////////////////////////////////

	template < class T, T v >
	struct integral_constant
	{
		static const T value = v;
		typedef T					value_type;
		typedef integral_constant	type;
	};

	//////////////////////////////////////////////////////
	/////           is_integral begin	 	         /////
	//////////////////////////////////////////////////////
	
	typedef ft::integral_constant<bool, true>	true_type;
	typedef ft::integral_constant<bool, false>	false_type;
	template <class _Tp> struct is_integral	: public false_type{};
	template <>          struct is_integral<bool>            					   : public true_type {};
	template <>          struct is_integral<char>								   : public true_type {};
	template <>          struct is_integral<char16_t>          					   : public true_type {};
	template <>          struct is_integral<char32_t>           				   : public true_type {};
	template <>          struct is_integral<wchar_t>           					   : public true_type {};
	template <>          struct is_integral<signed char>     		   			   : public true_type {};
	template <>          struct is_integral<short int>             				   : public true_type {};
	template <>          struct is_integral<int>              					   : public true_type {};
	template <>          struct is_integral<long int>             				   : public true_type {};
	template <>          struct is_integral<long long int>            			   : public true_type {};
	template <>          struct is_integral<unsigned char> 						   : public true_type {};
	template <>          struct is_integral<unsigned short int>              	   : public true_type {};
	template <>          struct is_integral<unsigned int>           			   : public true_type {};
	template <>          struct is_integral<unsigned long int> 		               : public true_type {};
	template <>          struct is_integral<unsigned long long int>                : public true_type {};
	
	
	//////////////////////////////////////////////////////
	/////           enable_if begin		 	         /////
	//////////////////////////////////////////////////////
	template < bool Cond, class T = void >
	struct enable_if
	{};
	template < class T >
	struct enable_if< true, T >
	{
		typedef T type;
	};

	//////////////////////////////////////////////////////
	/////           eqaul begin			 	         /////
	//////////////////////////////////////////////////////

	template <class InputIterator1, class InputIterator2>
  	bool equal ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2 )
	{
		while (first1 != last1) {
			if (!(*first1 == *first2))   // or: if (!pred(*first1,*first2)), for version 2
				return false;
			++first1; ++first2;
		}
		return true;
	}

	template<class InputIt1, class InputIt2, class BinaryPredicate>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
	{
		for (; first1 != last1; ++first1, ++first2) {
			if (!p(*first1, *first2)) {
				return false;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////
	/////           pair begin			 	         /////
	//////////////////////////////////////////////////////

	template<class _T1,class _T2>
	struct pair
	{
		typedef _T1 first_type;
		typedef _T2 second_type;

		_T1 first;
		_T2 second;

		pair() : first(), second() {}
		pair( const _T1& x, const _T2& y ) : first(x), second(y){}
		template< class U1, class U2 >
		pair( const pair<U1, U2>& p ) : first(p.first), second(p.second){}
		pair( const pair& p ) {*this = p;}
		
		pair& operator=(pair const& __p) {
        first = __p.first;
        second = __p.second;
        return *this;
    	}
	};

	//////////////////////////////////////////////////////
	/////           make_pair begin			         /////
	//////////////////////////////////////////////////////

	template< class T1, class T2 >
	pair<T1, T2> make_pair( T1 t, T2 u )
	{
		return pair< T1, T2 >(t, u);
	}
};