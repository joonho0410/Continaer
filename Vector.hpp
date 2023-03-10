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

#include <algorithm>
#include <stdexcept>
#include <compare>
#include <initializer_list>
#include "./Utils.hpp"
#include "./Iterator.hpp"
#include "./map.hpp"
#include <iostream>

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
			typedef	typename ft::vectorIterator<const_pointer, vec_type > const_iterator;
			typedef	ft::reverse_iterator<iterator>						reverse_iterator;		
			typedef	ft::reverse_iterator<const_iterator>				const_reverse_iterator;

			typedef std::allocator_traits<allocator_type>				__alloc_traits;

		protected :
			Allocator alloc; 
			pointer	__begin_; // 0
			pointer __end_; // back() + 1;
			pointer __end_cap_; // ????????? ???.
		
		protected :

		// begin ~ end  ?????? ????????? ??????. pointer version.
		void _Destroy(pointer _begin, pointer _end)
		{
			pointer temp = _begin;
			for (; temp != _end; ++temp)
				alloc.destroy(temp);
		}
		// _begin ?????? _end ?????? ???????????? ?????? ??????. iterator version.
		void _Destroy(iterator _begin, iterator _end)
		{
			for (; _begin != _end; ++_begin)
				alloc.destroy(_begin.base());
		}

		//?????? ????????? ??????.
		void _Construct(pointer pos,  const T& __x = T()) {alloc.construct(pos, __x);}

		// _begin ~ _end ?????? start??? ????????? ?????? ??????????????? ,????????? ?????? ????????????.
		template <class Iter>
		pointer copy(Iter _begin, Iter _end, pointer start)
		{ 
			value_type temp;
			for (; _begin != _end; ++_begin)
			{	
				temp = *_begin;
				alloc.destroy(start);
				alloc.construct(start, temp);
				++ start;
			}
			__end_ = start;
			return __end_; 
		}

		pointer allocate_copy(size_type _n, pointer _begin, pointer _end)
		{
			pointer _new_begin = alloc.allocate(_n);
			pointer	_ret = _new_begin;

			for(; _begin != _end; ++_begin)
			{
				alloc.construct(_new_begin, *_begin);
				++_new_begin;
			}
			return _ret;
		}
		
		// _finish ?????? ?????? ????????? ???????????? begin ~ end ?????? ????????? ?????? ????????? end ???????????? ??????.
		template <class Iter>
		iterator uninitialized_copy(Iter _begin, Iter _end, pointer _finish)
		{
			for(; _begin != _end; ++_begin)
			{
				alloc.construct(_finish, *_begin);
				++_finish;
			}
			return iterator(_finish);
		}

		iterator uninitialized_fill_n(iterator first, size_type count, const T& value)
		{
			for (; count > 0; ++ first, --count)
				alloc.construct(first.base(), value);
			return first;
		}

		//?????? iter??? ????????? ?????? ??????????????? 
		//template < class T, class _Alloc >
		void _M_insert_aux(iterator pos, const T __x)
		{
			if (__end_ != __end_cap_)// capacity ??? ????????? ???
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
				const size_type _len = _old_size != 0 ? 2 * _old_size : 1; // 0 -> 1, ????????? 2??????
				iterator _new_start(_M_allocate(_len));
				iterator _new_end(_new_start);//?
				try
				{
					_new_end = uninitialized_copy(iterator(__begin_), pos, _new_start.base());
					_Construct(_new_end.base(), __x);
					++_new_end;
					_new_end = uninitialized_copy(pos, iterator(__end_), _new_end.base());//???????????? ???????????? ????????? _new_end??? ??????.
				}
				catch(...) 
				{
					_Destroy(_new_start.base(), _new_end.base());
					_M_deallocate(_new_start.base(), _len);
					//throw error
				}
				_Destroy(begin().base(), end().base());
				_M_deallocate(__begin_, __end_cap_ - __begin_);
				__begin_ = _new_start.base();
				__end_ = _new_end.base();
				__end_cap_ = _new_start.base() + _len;
			}
		}

		//_M_initialize_aux
		// ?????? type??? ????????? 2????????? ??????????????????.
		template <class _Integer>
		void 
		_M_initialize_aux(_Integer __n, _Integer __value, true_type)
		{
			//std::cout << "_M_initialze_aus true_type " << std::endl;
			__begin_ = _M_allocate(__n);
			__end_ = __begin_ + __n;
			__end_cap_ = __begin_ + __n; 
			for (pointer temp = __begin_; temp != __end_; ++temp)
				alloc.construct(temp, __value);
		}

		template<class _InputIterator>
		void
		_M_initialize_aux(_InputIterator __first, _InputIterator __last, false_type)
		{
			//std::cout << "_M_initialze_aus false_type " << std::endl;
			//typedef typename ft::iterator_traits<_InputIterator>::iterator_category _IterCategory;
			_M_range_initialize(__first, __last, ft::forward_iterator_tag());
		}

		//_M_range_initialzie
		//iterator_tag??? ????????? 2????????? ?????????.
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
			size_type __n = distance(__first, __last);
			__begin_ = _M_allocate(__n);
			__end_cap_ = __begin_ + __n;
			__end_ = uninitialized_copy(__first, __last, __begin_).base();
		}

		// pos ~ count?????? val??? ???????????????.
		void _M_fill_insert(iterator pos, size_type count, const T& val)
		{
			if (count != 0)
			{
				if(size_type(__end_cap_ - __end_) >= count) //capacity ??? ????????? ??????.
				{
					T val_copy = val;
					const size_type _elems_after = end() - pos; //pos????????? ????????? ??????.
					iterator old_end(__end_);
					if (_elems_after > count)
					{
						uninitialized_copy(iterator(__end_ - count), iterator(__end_) , __end_);
						__end_ += count;
						std::copy_backward(pos, old_end - count, old_end);
						iterator end = pos + count;
						for(; pos != end ; ++pos)
							*pos = val_copy;
					}
					else
					{
						uninitialized_fill_n(iterator(__end_), count - _elems_after, val_copy);// what is uninitialized_fill_n ?
						__end_ += count - _elems_after; // ?
						uninitialized_copy(pos, old_end, __end_);
						__end_ += _elems_after;
						for (; pos != old_end ; ++pos)
							*pos = val_copy;
					}
				}
				else
				{
					const size_type _old_size = size();
					size_type temp = _old_size > count ? _old_size : count;
					const size_type _len = _old_size + temp;
					iterator _new_start(_M_allocate(_len));
					iterator _new_end(_new_start);//?
					try
					{
						_new_end = uninitialized_copy(iterator(__begin_), pos, _new_start.base());
						_new_end = uninitialized_fill_n(_new_end, count, val);
						_new_end = uninitialized_copy(pos, iterator(__end_), _new_end.base());//???????????? ???????????? ????????? _new_end??? ??????.
					}
					catch(...) 
					{
						_Destroy(_new_start.base(), _new_end.base());
						_M_deallocate(_new_start.base(), _len);
						//throw error
					}
					_Destroy(begin().base(), end().base());
					_M_deallocate(__begin_, __end_cap_ - __begin_);
					__begin_ = _new_start.base();
					__end_ = _new_end.base();
					__end_cap_ = _new_start.base() + _len;
				}
			}
		}

		template <class _Integer>
		void _M_insert_dispatch(iterator _pos, _Integer _n, _Integer _val, true_type)
		{ 
			//std::cout << "this is integer" << std::endl;
			_M_fill_insert(_pos, static_cast<size_type>(_n), static_cast<T>(_val));
		}

		template <class _Iter>
		void _M_insert_dispatch(iterator _pos, _Iter _first, _Iter _last, false_type)
		{ 
			//std::cout << "this is Iter" << std::endl;
			for (; _first != _last; ++_first)
			{
				_pos = insert(_pos, *_first);
				++_pos;
			}
		}

		template <class _Integer>
		void _M_assign_dispatch(_Integer _n, _Integer _val, true_type)
		{
			_M_fill_assign((size_type) _n, (T) _val);
		}

		template <class _Iter>
		void _M_assign_dispatch(_Iter _first, _Iter _last, false_type)
		{
			size_type _len = distance(_first, _last);

			if (_len > capacity())
			{
				pointer _tmp(_M_allocate_and_copy(_len, _first, _last));
				_Destroy(__begin_, __end_);
				_M_deallocate(__begin_, __end_cap_ - __begin_);
				__begin_ = _tmp;
				__end_cap_ = __begin_ + _len;
				__end_ = __begin_ + _len;
			}
			else if (size() >= _len)
			{
				iterator _new(copy(_first, _last, __begin_));
				_Destroy(_new, end());
				__end_ = _new.base();
			}
			else //capcity() >= _len > size()
			{
				_Iter _mid = _first;
				for(size_t n = size(); n > 0; --n)
					++ _mid;
				copy(_first, _mid, __begin_); //?????????????????? ??? ?????? ??????????????? copy;
				__end_ = uninitialized_copy(_mid, _last, __end_).base();
			}
		}

		template <class It>
		difference_type distance(It _first, It _last)
		{
			difference_type _result = 0;
			while (_first != _last)
			{
				++ _first;
				++ _result;
			}
			return _result;
		}

		protected :
			//vector ????????? index ?????? capacity??? ????????? ?????????????????????. at????????? ??????.
			void _M_range_check(size_type __n) const
			{
				if (__n  >= this -> size())
				{
					//std::cout << "size error " << std::endl;
					throw std::out_of_range("range over");
				}
			}

			template <class It, class V>
			void	fill(It _first, It _last, const V& value)
			{
				for (; _first != _last; ++_first)
					*_first = value;
			}

			template <class It, class Size, class V>
			It	fill_n(It _first, Size count, V& value)
			{
				for (Size i = 0; i < count; i++)
					*_first++ = value;
				return _first;
			}
			
			void _M_fill_assign(size_t _n, const value_type& _val)//????????? ?????????;
			{
				if (_n > capacity())
				{
					vector<T, allocator_type> _tmp(_n, _val, get_allocator());
					_tmp.swap(*this);
				}
				else if (_n > size())
				{
					fill(begin(), end(), _val);
					__end_ = uninitialized_fill_n(iterator(__end_), _n - size(), _val).base();
				}
				else
					erase(fill_n(begin(), _n, _val), end());
			}
			
		
		/// allocaote && deallocate
		public :
			// _n??? ?????? allocate
			value_type* _M_allocate(size_t _n)
				{ return alloc.allocate(_n); }
			// ptr ?????? n?????? deallocate
			void _M_deallocate(value_type* _ptr, size_t _n)
				{ if(_ptr) alloc.deallocate(_ptr, _n); }
			// len ?????? allocate ?????? begin ~ end ?????? ??????. return ??? ????????? ????????????.
			template< class Iter >
			pointer _M_allocate_and_copy(size_t _len, Iter _begin, Iter _end)
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
		///////////////////////////////
		//// vector constructor
		///////////////////////////////
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
					size_type __n = size_type(other.__end_ - other.__begin_);
					if (__n > capacity())// ????????? ?????? > ????????????.
					{	
						//std::cout << "1" << std::endl;
						pointer temp = _M_allocate_and_copy(__n, other.begin(), other.end());
						_Destroy(__begin_, __end_);
						_M_deallocate(__begin_, __end_cap_ - __begin_);
						__begin_ = temp;
						__end_cap_ = __begin_ + __n;
					}
					else if (size() >= __n)//??????????????? ????????? ???
					{
						//std::cout << " 2" << std::endl;
						iterator save_end = end();
						iterator __i(copy(other.begin(), other.end(), __begin_));
						_Destroy(__i, save_end);
					}
					else // ???????????? ????????? ??????????????? ????????? ???, ????????? ???????????????.
					{
						//std::cout << " 3 " << std::end
						copy(other.begin(), other.begin() + size(), __begin_);
						uninitialized_copy(other.begin() + size(), other.end(), __end_);
					}
					__end_ = __begin_ + __n;
				}
				return *this;
			}
			explicit vector( const Allocator& alloc ) : alloc(alloc()), __begin_(0), __end_(0), __end_cap_(0){}
			explicit vector( size_type	count, const T& value = T(), const Allocator& alloc2 = Allocator() )
			{
				//std::cout << "size_type, value, alloc" << std::endl;
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
			vector( InputIt first, InputIt last, const Allocator& alloc2 = Allocator() )
			{
				(void) alloc2;
				//std::cout << " iter , iter constructor" << std::endl;
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

			//range check??? ??? ??? ?????? throw??? ???????????? .	
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
			iterator				begin()  { return iterator(__begin_); }
			const_iterator			begin() const { return const_iterator(__begin_); }
			iterator				end()  { return iterator(__end_); }
			const_iterator			end() const { return const_iterator(__end_); }

			const_reverse_iterator  rbegin() const { return const_reverse_iterator(end()); }
			reverse_iterator		rbegin() {  return reverse_iterator(end()); }
			const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }
			reverse_iterator		rend(){  return reverse_iterator(begin()); }
			
		
		////////////////////////
		//  Capacity
		////////////////////////

		public :
			bool 		empty() const
			{ return begin() == end(); }
			size_type 	size() const
			{ return size_type( end() - begin()); }
			size_type	max_size() const
			{ return std::min<size_type>(size_type(-1) / sizeof(T), 
				std::numeric_limits<difference_type>::max()); }
			void 		reserve( size_type new_cap )
			{
				size_type __n = (size_type)(__end_cap_ - __begin_);
				if (__n < new_cap)
				{
					const size_type old_size = size();
					pointer			_tmp = allocate_copy(new_cap, __begin_, __end_);
					_Destroy(__begin_, __end_);
					_M_deallocate(__begin_, __end_cap_ - __begin_);
					__begin_ = _tmp;
					__end_ = _tmp + old_size;
					__end_cap_ = __begin_ + new_cap;
				}
			}
			size_type	capacity() const
			{ return size_type(const_iterator(__end_cap_) - begin()); }


			////////////////////////
			//  Modifiers
			////////////////////////

			void		assign(size_type _n, const T& _val){ _M_fill_assign(_n, _val); }
			template<class _InputIterator>
			void		assign(_InputIterator _first, _InputIterator _last)
			{
				typedef typename ft::is_integral< _InputIterator >::type _type;
				_M_assign_dispatch(_first, _last, _type());
			}
			void 		clear()
			{ erase(begin(), end()); }

			iterator 	insert( iterator pos, const T& value) //o
			{
				//std::cout << "pos , val" << std::endl;
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
			
			iterator 	insert( iterator pos, size_type count, const T& value) //o
			{
				//std::cout << "insert iter, size_type, value  " << std::endl;
				_M_fill_insert(pos, count, value);
				return pos; //temp
			}

			template< class InputIt >
			typename ft::enable_if< 
			!ft::is_integral< InputIt >::value, 
			iterator
			>::type
			insert( iterator pos, InputIt first, InputIt last) //o
			{
				//std::cout << "insert  iter, iter,  iter " << std::endl;
				typedef typename ft::is_integral< InputIt >::type type;
				_M_insert_dispatch(pos, first, last, type());

				return iterator(__begin_); //s
			}
			
			iterator 	erase( iterator pos )
			{
				//std::cout << "erase (iter)" << std::endl; 
				if (pos + 1 != end())
					copy(pos + 1, end(), pos.base());
				else
				{
					-- __end_;
					alloc.destroy(__end_);
				}
				return pos;
			}

			iterator	erase( iterator first, iterator last)
			{
				iterator save_end = end();
				iterator __i(copy(last, end(), first.base())); // ????????? ?????? ??? ??????????????? ????????????????????? ?????? ????????????.
				_Destroy(__i.base(), save_end.base());// ??? ??????????????? ???  ?????????  ?????? ???????????? ?????? ????????????;
				__end_ = save_end.base() - (last - first); // ????????? ???????????? ?????????.
				
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

	template<class InputIt1, class InputIt2>
	bool _M_lexicographical_compare(InputIt1 first1, InputIt1 last1,
								InputIt2 first2, InputIt2 last2)
	{
		for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template <class _Tp, class _Alloc>//?
	inline bool 
	operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
	{
	return __x.size() == __y.size() &&
			ft::equal(__x.begin(), __x.end(), __y.begin());
	}

	template <class _Tp, class _Alloc>
	inline bool 
	operator<(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
	{
	return _M_lexicographical_compare(__x.begin(), __x.end(), 
									__y.begin(), __y.end());
	}

	template <class _Tp, class _Alloc>
	inline void swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y)
	{
		__x.swap(__y);
	}

	template <class _Tp, class _Alloc>
	inline bool
	operator!=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
	return !(__x == __y);
	}

	template <class _Tp, class _Alloc>
	inline bool
	operator>(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
	return __y < __x;
	}

	template <class _Tp, class _Alloc>
	inline bool
	operator<=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
	return !(__y < __x);
	}

	template <class _Tp, class _Alloc>
	inline bool
	operator>=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
	return !(__x < __y);
	}
};
