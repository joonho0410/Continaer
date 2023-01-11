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
#include "Iterator.hpp"
#include <iostream>

#pragma once 
namespace	ft
{
	template < class T, class Allocator = std::allocator<T> >
	class vector
	{
		public :
			typedef vector<T, Allocator>								vec_type;
			typedef	T													value_type;
			typedef	Allocator											allocator_type;
			typedef	std::size_t											size_type;
			typedef	std::ptrdiff_t										difference_type;
			typedef	typename Allocator::pointer							pointer;
			typedef	typename Allocator::const_pointer					const_pointer;			
			typedef	value_type&											reference;
			typedef	const value_type&									const_reference;	
			typedef	typename ft::vectorIterator<pointer, vec_type >		iterator;
			typedef	typename ft::vectorIterator<const_pointer,vec_type> const_iterator;
			typedef	typename ft::reverse_iterator<iterator>				reverse_iterator;		
			typedef	typename ft::reverse_iterator<const_iterator>		const_reverse_iterator;

			typedef std::allocator_traits<allocator_type>				__alloc_traits;

		protected :
			Allocator alloc; 
			pointer	__begin_; // 0
			pointer __end_; // back() + 1;
			pointer __end_cap_; // 할당의 끝.
		
		protected :
			//void _M_insert_aux(iterator pos, const T& val);
			//void _M_insert_aux(iterator pos, T& val);

		public :
		// begin ~ end  까지 소멸자 호출. pointer version.
		void _Destroy(pointer _begin, pointer _end)
		{
			pointer temp = _begin;
			for (; temp != _end; ++temp)
				alloc.destroy(temp);
		}
		// _begin 부터 _end 까지 소멸자를 모두 호출. iterator version.
		void _Destory(iterator _begin, iterator _end)
		{
			for (; _begin != _end; ++_begin)
				alloc.destroy(_begin.base());
		}

		//해당 주소에 생성.
		void _Construct(pointer pos,  const T& __x = T()) {alloc.construct(pos, __x);}

		// _begin ~ _end 까지 start에 기존의 값을 소멸시키고 ,새로운 값을 복사한다.
		pointer copy(iterator _begin, iterator _end, pointer start)
		{
			for (; _begin != _end; ++_begin)
			{	
				alloc.destroy(start);
				alloc.construct(start, *_begin);
				++ start;
			}
			__end_ = start;
			return __end_; 
		}
		
		// _finish 부터 이미 할당된 메모리에 begin ~ end 까지 할당한 후에 마지막 end 포인터를 반환.
		iterator uninitialized_copy(iterator _begin, iterator _end, pointer _finish)
		{
			for(; _begin != _end; ++_begin)
			{
				alloc.construct(_finish, *_begin);
				++_finish;
			}
			return iterator(_finish);
		}

		//해당 iter에 새로운 값을 집어넣는다 
		//template < class _Tp, class _Alloc >
		void _M_insert_aux(iterator pos, const T __x)
		{
			if (__end_ != __end_cap_)// capacity 가 존재할 때
			{
				_Construct(__end_, *(__end_ -1));
				++ __end_;
				T __copy = __x;
				std::copy_backward(pos, iterator(__end_ - 2), iterator(__end_ - 1));//?
				*pos = __copy;
			}
			else
			{
				const size_type _old_size = size();
				const size_type _len = _old_size != 0 ? 2 * _old_size : 1; // 0 -> 1, 아니면 2배로
				iterator _new_start(_M_allocate(_len));
				iterator _new_end(_new_start);//?
				try
				{
					_new_end = uninitialized_copy(iterator(__begin_), pos, _new_start.base());
					_Construct(_new_end.base(), __x);
					++_new_end;
					_new_end = uninitialized_copy(pos, iterator(__end_), _new_end.base());//마지막에 넣는경우 그대로 _new_end를 반환.
				}
				catch(...) 
				{
					_Destroy(_new_start.base(), _new_end.base());
					_M_deallocate(_new_start.base(), _len);
					//throw error
				}
				_Destroy(begin().base(), end().base());
				//_M_allocate(__begin_, __end_cap_ - __begin_);
				__begin_ = _new_start.base();
				__end_ = _new_end.base();
				__end_cap_ = _new_start.base() + _len;
			}
		}

		//_M_initialize_aux
		// 뒤의 type에 따라서 2가지로 구현되어있음.
		template <class _Integer>
		void 
		_M_initialize_aux(_Integer __n, _Integer __value, true_type)
		{
			__begin_ = _M_allocate(__n);
			__end_ = __begin_ + __n;
			__end_cap_ = __begin_ + __n; 
			for (pointer temp = __begin_; temp != __end_; ++temp)
				alloc.allocate(temp, __value);
		}

		template<class _InputIterator>
		void
		_M_initialize_aux(_InputIterator __first, _InputIterator __last, false_type)
		{
			typedef typename ft::iterator_traits<_InputIterator>::iterator_category _IterCategory;
			_M_range_initialize(__first, __last, _IterCategory());
		}

		//_M_range_initialzie
		//iterator_tag에 따라서 2가지로 구현됨.
		template <class _InputIterator>
		void _M_range_initialize(_InputIterator __first,  
								_InputIterator __last, input_iterator_tag)
		{
			for (; __first != __last; ++__first)
				push_back(*__first);
		}

		template <class _ForwardIterator>
		void _M_range_initialize(_ForwardIterator __first,
								_ForwardIterator __last, forward_iterator_tag)
		{
			size_type __n = (__last - __first);
			__begin_ = _M_allocate(__n);
			__end_cap_ = __begin_ + __n;
			__end_ = std::uninitialized_copy(__first, __last, __begin_);
		}

		//_M_range_insert
		//마찬가지로 iterator_tag에 따라서 2가지로 분류.
		template <class _InputIterator>
		void _M_range_insert(iterator __pos,
							_InputIterator __first, _InputIterator __last,
							input_iterator_tag);

		template <class _ForwardIterator>
		void _M_range_insert(iterator __pos,
							_ForwardIterator __first, _ForwardIterator __last,
							forward_iterator_tag);

		void _M_fill_insert(const_iterator pos, size_type count, const T& val)
		{
			(void)pos;
			(void)count;
			(void)val;
			if(capacity() >= size() + count) // not reallocation just destroy; 
			{

			}
			else // reallocation
			{

			}
		}

		public :
			//vector 에서도 index 값이 capacity를 넘는지 확인하지않는다. at에서만 확인.
			void _M_range_check(size_type __n) const
			{
				if (__n  > this -> size())
					exit(1); //throw error;
			}
		
		/// allocaote && deallocate
		public :
			// _n개 만큼 allocate
			value_type* _M_allocate(size_t _n)
				{ return alloc.allocate(_n); }
			// ptr 부터 n개를 deallocate
			void _M_deallocate(value_type* _ptr, size_t _n)
				{ if(_ptr) alloc.deallocate(_ptr, _n); }
			// len 만큼 allocate 후에 begin ~ end 까지 할당. return 은 할당의 시작주소.
			pointer _M_allocate_and_copy(size_t _len, iterator _begin, iterator _end)
			{
				pointer ret = alloc.allocate(_len);
				pointer save = ret;
				for (; _begin != _end; ++_begin)
				{
					alloc.construct(ret, *_begin);
					++ret;
				}
				return save;
			}
			
		public :
			vector() : alloc(allocator_type()), __begin_(0), __end_(0), __end_cap_(0){}
			~vector()
			{
				if (__begin_)
				{
					pointer temp;
					size_type __n = (size_type)(__end_cap_ - __begin_);
					temp = __begin_;
					while (temp != __end_cap_)
						alloc.destroy(temp++);
					alloc.deallocate(__begin_, __n);
				}
			}

			vector& operator=( const vector& other )
			{
				if (&other != this)
				{ 
					alloc = allocator_type();
					size_type __n = size_type(other.__end_cap_ - other.__begin_);
					if (__n > capacity())// 할당할 크기 > 현재크기.
					{	
						pointer temp = _M_allocate_and_copy(__n, other.begin(), other.end());
						_Destroy(__begin_, __end_);
						_M_deallocate(__begin_, __end_cap_ - __begin_);
						__begin_ = temp;
						__end_cap_ = __begin_ + __n;
					}
					else if (size() >= __n)//현재크기가 충분할 때
					{
						iterator __i(copy(other.begin(), other.end(), begin()));
						_Destroy(__i, end());
					}
					else // 사이즈는 작은데 카파시티는 충분할 때, 그대로 카피하면됨.
					{
						copy(other.begin(), other.begin() + size(), __begin_);
						uninitialized_copy(other.begin() + size(), other.end(), __end_);
					}
					__end_ = __begin_ + __n;
				}
				return *this;
			}
			explicit vector( const Allocator& alloc ) : alloc(alloc()), __begin_(0), __end_(0), __end_cap_(0){}
			explicit vector( size_type	count, const T& value, const Allocator& alloc2 = Allocator() )
			{
				(void)alloc2;
				alloc = Allocator();
				__begin_ = alloc.allocate(count);
				__end_ = __begin_ + count;
				__end_cap_ = __begin_ + count;
				for (pointer temp = __begin_; temp != __end_ ; ++temp)
				{
					alloc.construct(temp, value);
				}
			}

			template< class InputIt >
			vector( InputIt first, InputIt last, typename ft::enable_if<ft::is_integral< InputIt >::value, int>::type)
			{
				(void) alloc;
				typedef typename ft::is_integral< InputIt >::type type;
				_M_initialize_aux(first, last, type());		
			}
			vector( const vector<T, Allocator>& other )
			{
				alloc = Allocator();
				__begin_ = alloc.allocate(other.size());
				__end_ = __begin_ + other.size();
				__end_cap_ = __end_;
				pointer temp = __begin_;
				size_t t = 0;
				for (; temp != __end_; ++temp)
				{
					alloc.construct(temp, other[t]);
					++t;
				}
			}
		
		////////////////////////
		//  Element access
		////////////////////////

		public :
			allocator_type		get_allocator() const
			{ return allocator_type(); }// ?

			reference operator[](size_type __n){ return *(begin() + __n); }
			const_reference operator[](size_type __n) const { return *(begin() + __n); }

			//range check를 한 후 예외 throw가 필요하다 .	
			value_type&			at( size_type pos )
			{ _M_range_check(pos); return (*this)[pos]; }
			const value_type& 	at( size_type pos ) const
			{ _M_range_check(pos); return (*this)[pos]; }

			value_type&			front()
			{ return *begin(); }
			const value_type& 	front() const
			{ return *begin(); }

			value_type&			back()
			{ return *(this->__end_ - 1); }
			const value_type& 	back() const
			{ return *(this->__end_ - 1); }

			//need size check!
			T*					data()
			{ return __begin_; }
			const T*			data() const
			{ return __begin_; }

		////////////////////////
		//  Iterators
		////////////////////////

		public :
			iterator				begin(){ return iterator(__begin_);}
			iterator				end(){ return iterator(__end_);};
			reverse_iterator		rbegin(){ return reverse_iterator(begin()); }
			const_reverse_iterator	crbegin() const { return const_reverse_iterator(begin()); }
			reverse_iterator		rend(){ return reverse_iterator(end()); }
			const_reverse_iterator	crend() const { return const_reverse_iterator(end()); }
			const_iterator			cbegin() const { return const_iterator (__begin_);}
			const_iterator			cend() const { return const_iterator (__end_);};
		
		////////////////////////
		//  Capacity
		////////////////////////

		public :
			bool 		empty() const
			{ return cbegin() == cend(); }
			size_type 	size() const
			{ return size_type( cend() - cbegin()); }
			size_type	max_size() const
			{ return size_type(-1) / sizeof(T); }
			void 		reserve( size_type new_cap )
			{
				size_type __n = (size_type)(__end_cap_ - __begin_);
				if (__n < new_cap)
				{
					const size_type old_size = size();
					pointer			_tmp = allocate_copy(new_cap, __begin_, __end_);
					_Destroy(__begin_, __end_);
					_Deallocate(__begin_, __end_cap_ - __begin_);
					__begin_ = _tmp;
					__end_ = _tmp + old_size;
					__end_cap_ = __begin_ + new_cap;
				}
			}
			size_type	capacity() const
			{ return size_type(const_iterator(__end_cap_) - cbegin()); }


			////////////////////////
			//  Modifiers
			////////////////////////

			void 		clear()
			{ erase(begin(), end()); }

			iterator 	insert( const_iterator pos, const T& value)
			{
				size_type __n = pos - begin();
				if (__end_ != __end_cap_ && pos == end())//eqaul push_back()
				{
					_Construct(__end_, value); // need _construct;
					++ __end_;
				}
				else
					_M_insert_aux(iterator(pos), value);// need _M_insert_aux;
				return begin() + __n;
			}

			iterator 	insert( const_iterator pos, size_type count, const T& value)
			{
				_M_fill_insert(pos, count, value);
			}

			template< class InputIt >
			iterator 	insert( const_iterator pos, InputIt first, InputIt last)
			{
				(void)pos;
				(void)first;
				(void)last;
				//typedef typename ft::is_integral<InputIt>::type type; // ?
				//_M_insert_dispatch(pos, first, last, type());

				return iterator(__begin_); //need change
			}
			
			iterator 	erase( iterator pos )
			{ 
				if (pos + 1 != end())
					copy(pos + 1, end(), pos);
				-- __end_;
				_Destroy(__end_);
				return pos;
			
			}
			iterator	erase( iterator first, iterator last)
			{
				iterator __i(copy(last, end(), first));
				_Destroy(__i, end());
				__end_ = __end_ - (last - first);
				return first;
			}

			void		push_back( const T& value )//o
			{
				if (__end_ != __end_cap_)
				{
					_Construct(__end_, value);
					++ __end_;
				}
				else
					_M_insert_aux(end(), value);
			}
			void		pop_back() // o
			{
				--__end_;
				_Destroy(__end_, __end_ + 1);
			}

			void		resize( size_type count, const T& t)
			{
				if (count < size())
					erase(begin() + count, end());
				else
					insert(end(), count - size(), t);
			}

			void		resize( size_type count)
			{ resize(count, T()); }

			void		swap( vector& other )
			{
				std::swap(__begin_, other.__begin_);
				std::swap(__end_, other.__end_);
				std::swap(__end_cap_, other.__end_cap_);
			}
	};
};