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
#include "Utils.hpp"

#pragma once
namespace	ft
{
	//////////////////////////////////////////////////////
	/////           iterator_traits begin	         /////
	//////////////////////////////////////////////////////

	struct  input_iterator_tag {};
	struct  output_iterator_tag {};
	struct  forward_iterator_tag       : public input_iterator_tag {};
	struct  bidirectional_iterator_tag : public forward_iterator_tag {};
	struct  random_access_iterator_tag : public bidirectional_iterator_tag {};

	 template<typename _Iterator>
     struct iterator_traits {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };

  template<typename _Tp>
    struct iterator_traits<_Tp*> {
      typedef ft::random_access_iterator_tag iterator_category;
      typedef _Tp                      	  value_type;
      typedef ptrdiff_t                   difference_type;
      typedef _Tp*                        pointer;
      typedef _Tp&                        reference;
    };

  template<typename _Tp>
    struct iterator_traits<const _Tp*> {
      typedef ft::random_access_iterator_tag iterator_category;
      typedef _Tp                         value_type;
      typedef ptrdiff_t                   difference_type;
      typedef const _Tp*                  pointer;
      typedef const _Tp&                  reference;
    };

	//////////////////////////////////////////////////////
	/////           iterator begin	         
	//////////////////////////////////////////////////////
	template<class Category, class T, class Difference = ptrdiff_t, class Pointer = T*,
             class Reference = T&>
	struct iterator
	{
		typedef T           value_type;
		typedef Difference  difference_type;
		typedef Pointer     pointer;
		typedef Reference   reference;
		typedef Category    iterator_category;
	};
	
	//////////////////////////////////////////////////////
	/////     vector iterator begin	         
	//////////////////////////////////////////////////////


	template < class Iter, typename Container >
	class vectorIterator : public ft::iterator < ft::random_access_iterator_tag	,
						typename ft::iterator_traits<Iter>::value_type,
                        typename ft::iterator_traits<Iter>::difference_type,
                        typename ft::iterator_traits<Iter>::pointer,                   
						typename ft::iterator_traits<Iter>::reference >
	{
		public :
			typedef typename ft::iterator_traits<Iter>::difference_type	difference_type;
			typedef typename ft::iterator_traits<Iter>::reference  		reference;
			typedef typename ft::iterator_traits<Iter>::pointer     	pointer;
			typedef typename ft::iterator_traits<Iter>::value_type		value_type;

		protected :
			Iter current;
			
		public :
			vectorIterator() : current(Iter()){}
			
			explicit
			vectorIterator(const Iter& i) : current(i){} //explicit keyword 차이?
			
			//template <typename Iter>
			vectorIterator(const vectorIterator<Iter, Container >& i)
			 : current(i.base()){}

			vectorIterator&
			 operator++()
			  {++current; return *this;}

			vectorIterator
			 operator++( int )
			 {vectorIterator __tmp(*this); ++current; return __tmp;}

			vectorIterator&
			 operator--()
			  {--current; return *this;}

			vectorIterator
			 operator--( int )
			  {vectorIterator __tmp(*this); --current; return __tmp;}

			vectorIterator
			 operator+ ( difference_type __n )
			  const {return vectorIterator(current + __n);}

			vectorIterator&
			 operator+=( difference_type __n )
			  {current += __n; return *this;}

			vectorIterator  
			 operator- ( difference_type __n ) const 
			 {return vectorIterator(current - __n);}

			vectorIterator&
			 operator-=( difference_type __n )
			  {current -= __n; return *this;}

			reference
			 operator*() const 
			 {return *current;}

			pointer
			 operator->() const 
			 {return current;}

			reference
			 operator[](const difference_type n ) const 
			 {return *(*this + n);}

			difference_type
			 operator- (const vectorIterator &i) const
			 {return current - i.current;}

			const Iter&
			 base() const 
			 {return current;}
	};
	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
	operator==(const vectorIterator<_IteratorL, _Container>& __lhs,
			const vectorIterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() == __rhs.base(); }

	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
	operator!=(const vectorIterator<_IteratorL, _Container>& __lhs,
			const vectorIterator<_IteratorR, _Container>& __rhs)
	{ return !(__lhs == __rhs); }

	// Random access iterator requirements
	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool 
	operator<(const vectorIterator<_IteratorL, _Container>& __lhs,
					const vectorIterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() < __rhs.base(); }
	
	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
	operator>(const vectorIterator<_IteratorL, _Container>& __lhs,
			const vectorIterator<_IteratorR, _Container>& __rhs)
	{ return __rhs < __lhs; }

	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
	operator<=(const vectorIterator<_IteratorL, _Container>& __lhs,
			const vectorIterator<_IteratorR, _Container>& __rhs)
	{ return !(__rhs < __lhs); }

	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
	operator>=(const vectorIterator<_IteratorL, _Container>& __lhs,
			const vectorIterator<_IteratorR, _Container>& __rhs)
	{ return !(__lhs < __rhs); }

	template<typename _Iterator, typename _Container>
	inline vectorIterator<_Iterator, _Container>
	operator+(typename vectorIterator<_Iterator, _Container>::difference_type __n,
			const vectorIterator<_Iterator, _Container>& __i)
	{ return vectorIterator<_Iterator, _Container>(__i.base() + __n); }
	

	//////////////////////////////////////////////////////
	/////           reverse_iterator begin           /////
	//////////////////////////////////////////////////////

	template < class Iter >
	class reverse_iterator
	{
		protected :
			Iter current;
		public :
			typedef Iter													iterator_type;
			typedef typename ft::iterator_traits<Iter>::iterator_category	iterator_category;
			typedef typename ft::iterator_traits<Iter>::value_type			value_type;
			typedef typename ft::iterator_traits<Iter>::difference_type		difference_type;
			typedef typename ft::iterator_traits<Iter>::pointer				pointer;
			typedef typename ft::iterator_traits<Iter>::reference			reference;

			reverse_iterator() : current(){}
			iterator_type base() const {return current;}
			explicit reverse_iterator( iterator_type x ) : current(x){}
			template < class U >
			reverse_iterator( const reverse_iterator< U >& other ) : current(other.base()){}
			template < class U >
        	reverse_iterator& operator=( const reverse_iterator< U >& other )
            { current = other.base(); return *this; }
			reverse_iterator& operator++() {--current; return *this;}
			reverse_iterator  operator++( int ) {reverse_iterator __tmp(*this); --current; return __tmp;}
			reverse_iterator& operator--() {++current; return *this;}
			reverse_iterator  operator--( int ) {reverse_iterator __tmp(*this); ++current; return __tmp;}
			reverse_iterator  operator+ ( difference_type __n ) const {return reverse_iterator(current - __n);}
			reverse_iterator& operator+=( difference_type __n ) {current -= __n; return *this;}
			reverse_iterator  operator- ( difference_type __n ) const {return reverse_iterator(current + __n);}
			reverse_iterator& operator-=( difference_type __n ) {current += __n; return *this;}
			reference	operator*() const {Iter tmp = current; return *--tmp;}
			pointer		operator->() const {return &(operator*());}
			reference	operator[]( difference_type n ) const {return *(*this + n);}
	};
	template <class _Iter1, class _Iter2>
	bool operator==(const ft::reverse_iterator<_Iter1>& __x, const ft::reverse_iterator<_Iter2>& __y)
	{
    	return __x.base() == __y.base();
	}
	template <class _Iter1, class _Iter2>
	bool operator<(const ft::reverse_iterator<_Iter1>& __x, const ft::reverse_iterator<_Iter2>& __y)
	{
		return __x.base() > __y.base();
	}
	template <class _Iter1, class _Iter2>
	bool operator!=(const ft::reverse_iterator<_Iter1>& __x, const ft::reverse_iterator<_Iter2>& __y)
	{
		return __x.base() != __y.base();
	}
	template <class _Iter1, class _Iter2>
	bool operator>(const ft::reverse_iterator<_Iter1>& __x, const ft::reverse_iterator<_Iter2>& __y)
	{
		return __x.base() < __y.base();
	}
	template <class _Iter1, class _Iter2>
	bool operator>=(const ft::reverse_iterator<_Iter1>& __x, const ft::reverse_iterator<_Iter2>& __y)
	{
		return __x.base() <= __y.base();
	}
	template <class _Iter1, class _Iter2>
	bool operator<=(const ft::reverse_iterator<_Iter1>& __x, const ft::reverse_iterator<_Iter2>& __y)
	{
		return __x.base() >= __y.base();
	}
	template <class _Iter1, class _Iter2>
	typename ft::reverse_iterator<_Iter1>::difference_type 
	operator-(const ft::reverse_iterator<_Iter1>& __x, const ft::reverse_iterator<_Iter2>& __y)
	{
		return __y.base() - __x.base();
	}
	template <class _Iter>
	ft::reverse_iterator<_Iter> 
	operator+(typename ft::reverse_iterator<_Iter>::difference_type __n, const ft::reverse_iterator<_Iter>& __x)
	{
		return ft::reverse_iterator<_Iter>(__x.base() - __n);
	}
};