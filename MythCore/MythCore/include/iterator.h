// Types used in iterator implementation -*- C++ -*-

// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
// Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */
#ifndef __iterator_h__
#define __iterator_h__


namespace MythInternal
{
/** @file stl_iterator_base_types.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 *
 *  This file contains all of the general iterator-related utility types,
 *  such as iterator_traits and struct iterator.
 */
//@{
  /**
   *  @defgroup iterator_tags Iterator Tags
   *  These are empty types, used to distinguish different iterators.  The
   *  distinction is not made by what they contain, but simply by what they
   *  are.  Different underlying algorithms can then be used based on the
   *  different operations supported by different iterator types.
  */
  ///  Marking input iterators.
  struct input_iterator_tag {};
  ///  Marking output iterators.
  struct output_iterator_tag {};
  /// Forward iterators support a superset of input iterator operations.
  struct forward_iterator_tag : public input_iterator_tag {};
  /// Bidirectional iterators support a superset of forward iterator
  /// operations.
  struct bidirectional_iterator_tag : public forward_iterator_tag {};
  /// Random-access iterators support a superset of bidirectional iterator
  /// operations.
  struct random_access_iterator_tag : public bidirectional_iterator_tag {};
  //@}


  /**
   *  @brief  Common %iterator class.
   *
   *  This class does nothing but define nested typedefs.  %Iterator classes
   *  can inherit from this class to save some work.  The typedefs are then
   *  used in specializations and overloading.
   *
   *  In particular, there are no default implementations of requirements
   *  such as @c operator++ and the like.  (How could there be?)
  */
  template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
           typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator
    {
      /// One of the @link iterator_tags tag types@endlink.
      typedef _Category  iterator_category;
      /// The type "pointed to" by the iterator.
      typedef _Tp        value_type;
      /// Distance between iterators is represented as this type.
      typedef _Distance  difference_type;
      /// This type represents a pointer-to-value_type.
      typedef _Pointer   pointer;
      /// This type represents a reference-to-value_type.
      typedef _Reference reference;
    };

  /**
   *  This class does nothing but define nested typedefs.  The general
   *  version simply "forwards" the nested typedefs from the Iterator
   *  argument.  Specialized versions for pointers and pointers-to-const
   *  provide tighter, more correct semantics.
  */
  template<typename _Iterator>
    struct iterator_traits
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };

  template<typename _Tp>
    struct iterator_traits<_Tp*>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp                         value_type;
      typedef ptrdiff_t                   difference_type;
      typedef _Tp*                        pointer;
      typedef _Tp&                        reference;
    };

  template<typename _Tp>
    struct iterator_traits<const _Tp*>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp                         value_type;
      typedef ptrdiff_t                   difference_type;
      typedef const _Tp*                  pointer;
      typedef const _Tp&                  reference;
    };

	template<typename _Iterator, typename _Container>
	class __normal_iterator
	{
	protected:
		_Iterator _M_current;

	public:
		typedef _Iterator					     iterator_type;
		typedef typename iterator_traits<_Iterator>::iterator_category
			iterator_category;
		typedef typename iterator_traits<_Iterator>::value_type  value_type;
		typedef typename iterator_traits<_Iterator>::difference_type
			difference_type;
		typedef typename iterator_traits<_Iterator>::reference reference;
		typedef typename iterator_traits<_Iterator>::pointer   pointer;

		__normal_iterator() : _M_current(_Iterator()) { }

		explicit
			__normal_iterator(const _Iterator& __i) : _M_current(__i) { }

		// Allow iterator to const_iterator conversion
		template<typename _Iter>
		__normal_iterator(const __normal_iterator<_Iter,
			_Container>& __i)
			: _M_current(__i.base()) { }

		// Forward iterator requirements
		reference
			operator*() const
		{ return *_M_current; }

		pointer
			operator->() const
		{ return _M_current; }

		__normal_iterator&
			operator++()
		{
			++_M_current;
			return *this;
		}

		__normal_iterator
			operator++(int)
		{ return __normal_iterator(_M_current++); }

		// Bidirectional iterator requirements
		__normal_iterator&
			operator--()
		{
			--_M_current;
			return *this;
		}

		__normal_iterator
			operator--(int)
		{ return __normal_iterator(_M_current--); }

		// Random access iterator requirements
		reference
			operator[](const difference_type& __n) const
		{ return _M_current[__n]; }

		__normal_iterator&
			operator+=(const difference_type& __n)
		{ _M_current += __n; return *this; }

		__normal_iterator
			operator+(const difference_type& __n) const
		{ return __normal_iterator(_M_current + __n); }

		__normal_iterator&
			operator-=(const difference_type& __n)
		{ _M_current -= __n; return *this; }

		__normal_iterator
			operator-(const difference_type& __n) const
		{ return __normal_iterator(_M_current - __n); }

		const _Iterator&
			base() const
		{ return _M_current; }
	};

	// Note: In what follows, the left- and right-hand-side iterators are
	// allowed to vary in types (conceptually in cv-qualification) so that
	// comparison between cv-qualified and non-cv-qualified iterators be
	// valid.  However, the greedy and unfriendly operators in std::rel_ops
	// will make overload resolution ambiguous (when in scope) if we don't
	// provide overloads whose operands are of the same type.  Can someone
	// remind me what generic programming is about? -- Gaby

	// Forward iterator requirements
	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
		operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
		const __normal_iterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() == __rhs.base(); }

	template<typename _Iterator, typename _Container>
	inline bool
		operator==(const __normal_iterator<_Iterator, _Container>& __lhs,
		const __normal_iterator<_Iterator, _Container>& __rhs)
	{ return __lhs.base() == __rhs.base(); }

	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
		operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs,
		const __normal_iterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() != __rhs.base(); }

	template<typename _Iterator, typename _Container>
	inline bool
		operator!=(const __normal_iterator<_Iterator, _Container>& __lhs,
		const __normal_iterator<_Iterator, _Container>& __rhs)
	{ return __lhs.base() != __rhs.base(); }

	// Random access iterator requirements
	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
		operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
		const __normal_iterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() < __rhs.base(); }

	template<typename _Iterator, typename _Container>
	inline bool
		operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
		const __normal_iterator<_Iterator, _Container>& __rhs)
	{ return __lhs.base() < __rhs.base(); }

	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
		operator>(const __normal_iterator<_IteratorL, _Container>& __lhs,
		const __normal_iterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() > __rhs.base(); }

	template<typename _Iterator, typename _Container>
	inline bool
		operator>(const __normal_iterator<_Iterator, _Container>& __lhs,
		const __normal_iterator<_Iterator, _Container>& __rhs)
	{ return __lhs.base() > __rhs.base(); }

	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
		operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs,
		const __normal_iterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() <= __rhs.base(); }

	template<typename _Iterator, typename _Container>
	inline bool
		operator<=(const __normal_iterator<_Iterator, _Container>& __lhs,
		const __normal_iterator<_Iterator, _Container>& __rhs)
	{ return __lhs.base() <= __rhs.base(); }

	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline bool
		operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs,
		const __normal_iterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() >= __rhs.base(); }

	template<typename _Iterator, typename _Container>
	inline bool
		operator>=(const __normal_iterator<_Iterator, _Container>& __lhs,
		const __normal_iterator<_Iterator, _Container>& __rhs)
	{ return __lhs.base() >= __rhs.base(); }

	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// According to the resolution of DR179 not only the various comparison
	// operators but also operator- must accept mixed iterator/const_iterator
	// parameters.
	template<typename _IteratorL, typename _IteratorR, typename _Container>
	inline typename __normal_iterator<_IteratorL, _Container>::difference_type
		operator-(const __normal_iterator<_IteratorL, _Container>& __lhs,
		const __normal_iterator<_IteratorR, _Container>& __rhs)
	{ return __lhs.base() - __rhs.base(); }

	template<typename _Iterator, typename _Container>
	inline typename __normal_iterator<_Iterator, _Container>::difference_type
		operator-(const __normal_iterator<_Iterator, _Container>& __lhs,
		const __normal_iterator<_Iterator, _Container>& __rhs)
	{ return __lhs.base() - __rhs.base(); }

	template<typename _Iterator, typename _Container>
	inline __normal_iterator<_Iterator, _Container>
		operator+(typename __normal_iterator<_Iterator, _Container>::difference_type
		__n, const __normal_iterator<_Iterator, _Container>& __i)
	{ return __normal_iterator<_Iterator, _Container>(__i.base() + __n); }


	  // 24.4.1 Reverse iterators
  /**
   *  "Bidirectional and random access iterators have corresponding reverse
   *  %iterator adaptors that iterate through the data structure in the
   *  opposite direction.  They have the same signatures as the corresponding
   *  iterators.  The fundamental relation between a reverse %iterator and its
   *  corresponding %iterator @c i is established by the identity:
   *  @code
   *      &*(reverse_iterator(i)) == &*(i - 1)
   *  @endcode
   *
   *  This mapping is dictated by the fact that while there is always a
   *  pointer past the end of an array, there might not be a valid pointer
   *  before the beginning of an array." [24.4.1]/1,2
   *
   *  Reverse iterators can be tricky and surprising at first.  Their
   *  semantics make sense, however, and the trickiness is a side effect of
   *  the requirement that the iterators must be safe.
  */
  template<typename _Iterator>
    class reverse_iterator
    : public iterator<typename iterator_traits<_Iterator>::iterator_category,
		      typename iterator_traits<_Iterator>::value_type,
		      typename iterator_traits<_Iterator>::difference_type,
		      typename iterator_traits<_Iterator>::pointer,
                      typename iterator_traits<_Iterator>::reference>
    {
    protected:
      _Iterator current;

    public:
      typedef _Iterator					       iterator_type;
      typedef typename iterator_traits<_Iterator>::difference_type
							       difference_type;
      typedef typename iterator_traits<_Iterator>::reference   reference;
      typedef typename iterator_traits<_Iterator>::pointer     pointer;

    public:
      /**
       *  The default constructor default-initializes member @p current.
       *  If it is a pointer, that means it is zero-initialized.
      */
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 235 No specification of default ctor for reverse_iterator
      reverse_iterator() : current() { }

      /**
       *  This %iterator will move in the opposite direction that @p x does.
      */
      explicit
      reverse_iterator(iterator_type __x) : current(__x) { }

      /**
       *  The copy constructor is normal.
      */
      reverse_iterator(const reverse_iterator& __x)
      : current(__x.current) { }

      /**
       *  A reverse_iterator across other types can be copied in the normal
       *  fashion.
      */
      template<typename _Iter>
        reverse_iterator(const reverse_iterator<_Iter>& __x)
	: current(__x.base()) { }

      /**
       *  @return  @c current, the %iterator used for underlying work.
      */
      iterator_type
      base() const
      { return current; }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reference
      operator*() const
      {
	_Iterator __tmp = current;
	return *--__tmp;
      }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      pointer
      operator->() const
      { return &(operator*()); }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reverse_iterator&
      operator++()
      {
	--current;
	return *this;
      }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reverse_iterator
      operator++(int)
      {
	reverse_iterator __tmp = *this;
	--current;
	return __tmp;
      }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reverse_iterator&
      operator--()
      {
	++current;
	return *this;
      }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reverse_iterator
      operator--(int)
      {
	reverse_iterator __tmp = *this;
	++current;
	return __tmp;
      }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reverse_iterator
      operator+(difference_type __n) const
      { return reverse_iterator(current - __n); }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reverse_iterator&
      operator+=(difference_type __n)
      {
	current -= __n;
	return *this;
      }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reverse_iterator
      operator-(difference_type __n) const
      { return reverse_iterator(current + __n); }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reverse_iterator&
      operator-=(difference_type __n)
      {
	current += __n;
	return *this;
      }

      /**
       *  @return  TODO
       *
       *  @doctodo
      */
      reference
      operator[](difference_type __n) const
      { return *(*this + __n); }
    };

  //@{
  /**
   *  @param  x  A %reverse_iterator.
   *  @param  y  A %reverse_iterator.
   *  @return  A simple bool.
   *
   *  Reverse iterators forward many operations to their underlying base()
   *  iterators.  Others are implemented in terms of one another.
   *
  */
  template<typename _Iterator>
    inline bool
    operator==(const reverse_iterator<_Iterator>& __x,
	       const reverse_iterator<_Iterator>& __y)
    { return __x.base() == __y.base(); }

  template<typename _Iterator>
    inline bool
    operator<(const reverse_iterator<_Iterator>& __x,
	      const reverse_iterator<_Iterator>& __y)
    { return __y.base() < __x.base(); }

  template<typename _Iterator>
    inline bool
    operator!=(const reverse_iterator<_Iterator>& __x,
	       const reverse_iterator<_Iterator>& __y)
    { return !(__x == __y); }

  template<typename _Iterator>
    inline bool
    operator>(const reverse_iterator<_Iterator>& __x,
	      const reverse_iterator<_Iterator>& __y)
    { return __y < __x; }

  template<typename _Iterator>
    inline bool
    operator<=(const reverse_iterator<_Iterator>& __x,
	       const reverse_iterator<_Iterator>& __y)
    { return !(__y < __x); }

  template<typename _Iterator>
    inline bool
    operator>=(const reverse_iterator<_Iterator>& __x,
	       const reverse_iterator<_Iterator>& __y)
    { return !(__x < __y); }

  template<typename _Iterator>
    inline typename reverse_iterator<_Iterator>::difference_type
    operator-(const reverse_iterator<_Iterator>& __x,
	      const reverse_iterator<_Iterator>& __y)
    { return __y.base() - __x.base(); }

  template<typename _Iterator>
    inline reverse_iterator<_Iterator>
    operator+(typename reverse_iterator<_Iterator>::difference_type __n,
	      const reverse_iterator<_Iterator>& __x)
    { return reverse_iterator<_Iterator>(__x.base() - __n); }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 280. Comparison of reverse_iterator to const reverse_iterator.
  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator==(const reverse_iterator<_IteratorL>& __x,
	       const reverse_iterator<_IteratorR>& __y)
    { return __x.base() == __y.base(); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator<(const reverse_iterator<_IteratorL>& __x,
	      const reverse_iterator<_IteratorR>& __y)
    { return __y.base() < __x.base(); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator!=(const reverse_iterator<_IteratorL>& __x,
	       const reverse_iterator<_IteratorR>& __y)
    { return !(__x == __y); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator>(const reverse_iterator<_IteratorL>& __x,
	      const reverse_iterator<_IteratorR>& __y)
    { return __y < __x; }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator<=(const reverse_iterator<_IteratorL>& __x,
	       const reverse_iterator<_IteratorR>& __y)
    { return !(__y < __x); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator>=(const reverse_iterator<_IteratorL>& __x,
	       const reverse_iterator<_IteratorR>& __y)
    { return !(__x < __y); }

  template<typename _IteratorL, typename _IteratorR>
    inline typename reverse_iterator<_IteratorL>::difference_type
    operator-(const reverse_iterator<_IteratorL>& __x,
	      const reverse_iterator<_IteratorR>& __y)
    { return __y.base() - __x.base(); }
  //@}

}

#endif