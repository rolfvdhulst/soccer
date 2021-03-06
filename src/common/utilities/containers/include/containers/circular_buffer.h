//
// Created by rolf on 21-04-20.
//

#ifndef SOCCER_CIRCULARBUFFER_H_
#define SOCCER_CIRCULARBUFFER_H_
#include <iterator>
#include <iostream>

template<typename _Tp, std::size_t _Size>
class circular_buffer_iterator_type
{
 private:
  _Tp * const buffer;
  std::size_t offset;
 public:
  circular_buffer_iterator_type(_Tp *buffer, std::size_t offset) :buffer{buffer}, offset{offset}{}
  bool operator==(const circular_buffer_iterator_type &other){
    return other.buffer == buffer && other.offset == offset; //TODO: iterators invalidated by pushing back elements now?
  }
  bool operator!=(const circular_buffer_iterator_type &other){
    return !(*this == other);
  }
  circular_buffer_iterator_type &operator++() {
    ++offset;
    return *this;
  }
  circular_buffer_iterator_type &operator--() {
    --offset;
    return *this;
  }
  std::ptrdiff_t operator-(circular_buffer_iterator_type const& other) const{
    return offset - other.offset;
  }
  circular_buffer_iterator_type & operator +=(int amount){
    offset+=amount;
    return *this;
  }
  circular_buffer_iterator_type &operator -=(int amount){
    offset-=amount;
    return *this;
  }
  _Tp& operator*() const {
    return buffer[offset % _Size];
  }
};
// need to define the iterator traits for std algorithms
template<typename _Tp, std::size_t _Size>
class std::iterator_traits<circular_buffer_iterator_type<_Tp,_Size>>{
 public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = _Tp;
  using pointer = _Tp*;
  using reference = _Tp&;
  using iterator_category = std::random_access_iterator_tag;
};
/**
 * @brief A standard container for Circular buffers
 * @tparam _Tp
 * @tparam _Size
 */
template<typename _Tp, std::size_t _Size>
class circular_buffer{
 public:
  typedef _Tp 	    			      value_type;
  typedef value_type*			      pointer;
  typedef const value_type*                       const_pointer;
  typedef value_type&                   	      reference;
  typedef const value_type&             	      const_reference;
  typedef std::size_t                    	      size_type;
  typedef std::ptrdiff_t                   	      difference_type;

  typedef circular_buffer_iterator_type<_Tp,_Size> iterator;
  typedef circular_buffer_iterator_type<const _Tp, _Size> const_iterator;
  typedef std::reverse_iterator<iterator>	      reverse_iterator;
  typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

 private:
  // the internal array that actually stores the values
  _Tp _m_buff[_Size];

  //The virtual beginning of the circular buffer
  size_type _current_offset;

  //The current size of the circular buffer
  size_type _current_size;

  _Tp &last_value() {
    return _m_buff[(_current_offset + _current_size -1) % _Size];
  }
 public:
  //Default constructor
  circular_buffer() : _current_offset(0), _current_size(0){

  }
  //modifying functions
  _Tp & pop_front(){
    _Tp &ret = _m_buff[_current_offset++];
    _current_offset %= _Size;
    --_current_size;
    return ret;
  }
  void push_back(const _Tp& value){
    if(_current_size < capacity()){
      ++_current_size;
    } else{
      ++_current_offset %= _Size;
    }
    last_value() = value;
  }
  void push_back(_Tp&& value){
    if(_current_size < capacity()){
      ++_current_size;
    } else{
      ++ _current_offset %= _Size;
    }
    last_value() = std::move(value);
  }
  //iterator functions
  constexpr iterator begin() noexcept{
    return iterator{_m_buff,_current_offset};
  }

  constexpr const_iterator begin() const noexcept {
    return const_iterator{_m_buff,_current_offset};
  }

  constexpr iterator end() noexcept
  { return iterator{_m_buff,_current_offset+_Size}; }

  constexpr const_iterator end() const noexcept
  {return const_iterator{_m_buff,_current_offset+_Size};}

  constexpr reverse_iterator rbegin() noexcept
  {return reverse_iterator(end());}

  constexpr const_reverse_iterator rbegin() const noexcept
  {return const_reverse_iterator(end());}

  constexpr reverse_iterator rend() noexcept
  {return reverse_iterator(begin()); }

  constexpr  const_reverse_iterator rend() const noexcept
  {return const_reverse_iterator(begin());}

  constexpr const_iterator cbegin() const noexcept
  {return const_iterator(begin());}

  constexpr const_iterator cend() const noexcept
  {return const_iterator(end());}

  constexpr const_reverse_iterator crbegin() const noexcept
  {return rbegin();}

  constexpr const_reverse_iterator crend() const noexcept
  {return rend();}

  //Capacity functions
  constexpr size_type size() const noexcept
  {return _current_size;}

  constexpr size_type capacity() const noexcept
  {return _Size;}

  constexpr size_type max_size() const noexcept
  {return _Size;}

  constexpr bool empty() const noexcept
  { return size() == 0;}

  constexpr bool full() const noexcept
  {return size() == _Size;}
  //Element access
  constexpr reference operator[](size_type __n)  noexcept
  {}

  constexpr const_reference operator[](size_type __n) const noexcept
  {}

  constexpr reference at(size_type __n)
  {
    if(__n >= _Size){
      std::__throw_out_of_range_fmt(__N("circular_buffer::at: __n (which is %zu) "
                                        ">= _Size (which is %zu)"),
                                    __n, _Size);
    }
  }
  //Result of expression must be lvalue so we use inline boolean as follows:
  constexpr const_reference at(size_type __n) const
  {
//    return __n < _Size ? : (      std::__throw_out_of_range_fmt(__N("circular_buffer::at: __n (which is %zu) "
//                                                                    ">= _Size (which is %zu)"),
//                                                                __n, _Size), );
  }
  constexpr reference front() noexcept
  {return _m_buff[_current_offset];}

  constexpr const_reference front() const noexcept
  {return _m_buff[_current_offset];}

  constexpr reference back() noexcept
  {return last_value();}

  constexpr const_reference back() const noexcept
  {return last_value();}

  //Comparisons
};
#endif //SOCCER_CIRCULARBUFFER_H_
