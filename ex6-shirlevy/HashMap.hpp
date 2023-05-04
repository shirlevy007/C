#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include<cstdio>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

#define START_SIZE 16
#define BASE 2
#define UPPER 0.75
#define LOWER 0.25
#define DIFF_LEN "keys and values are not with same length"
#define END_BUT_FULL "end "
#define KEY_NOT_FOUND "the key given is not in the HushMap"

template<class KeyT, class ValueT>
class HashMap
{
 protected:
  int _capacity;
  int _size;
  std::vector<std::pair<KeyT, ValueT>> *buckets;
  /**
   *
   * @param key
   * @return the hashed key
   */
  int hash (const KeyT &key) const
  {
    std::hash<KeyT> hashed;
    return hashed (key) & (_capacity - 1);
  }
  /**
   * called only by insert \ erase.
   * changes capacity, rehashes all keys&values according to new capacity
   */
  void rehash ()
  {
    int cap = _capacity;
    if (get_load_factor () > UPPER)
    {
      while (get_load_factor () > UPPER)
      {
        _capacity *= BASE;
      }
    }
    else
    {
      if (_size == 0)
      {
        _capacity = 1;
      }
      while (get_load_factor () < LOWER || _capacity == 1)
      {
        _capacity /= BASE;
      }
    }
    auto *temp = new std::vector<std::pair<KeyT, ValueT>>[_capacity];
    for (int i = 0; i < cap; ++i)
    {
      if (buckets[i].size () != 0)
      {
        int k = 0;
        for (auto j = buckets[i].begin (); j < buckets[i].end (); ++j)
        {
          int id = hash (buckets[i][k].first);
          temp[id].push_back (buckets[i][k]);
          k++;
        }
      }
    }
    delete[] buckets;
    buckets = temp;
  }
  /**
   *
   * @param a to check all its items
   * @param b other hm
   * @return true if all items of a in b
   */
  bool all_items (const HashMap &a, const HashMap &b) const
  {
    for (int i = 0; i < a._capacity; ++i)
    {
      if (a.buckets[i].size () != 0)
      {
        int k = 0;
        for (auto j = a.buckets[i].cbegin (); j < a.buckets[i].cend (); ++j)
        {
          if (b.contains_key (a.buckets[i][k].first))
          {
            if (b[a.buckets[i][k].first] != a[a.buckets[i][k].first])
            {
              return false;
            }
          }
          else
          {
            return false;
          }
          k++;
        }
      }
    }
    return true;
  }

 public:
  /**
   * default constructor
   */
  HashMap () : _capacity (START_SIZE), _size (0)
  {
    buckets = new std::vector<std::pair<KeyT, ValueT>>[_capacity] ();
  }
  /**
   * constructor by keys and values
   * @param keys organized in vector
   * @param values organized in vector
   */
  HashMap (const std::vector<KeyT> &_keys, const std::vector<ValueT> &_values)
  {
    if (_keys.size () != _values.size ())
    {
      throw std::length_error (DIFF_LEN);
    }
    std::vector<KeyT> keys;
    std::vector<ValueT> values;
    int unique_keys = 0;
    for (int i = _keys.size () - 1; i >= 0; --i)
    {
      if (std::count (keys.begin (), keys.end (), _keys[i]) == 0)
      {
        keys.push_back (_keys[i]);
        values.push_back (_values[i]);
        unique_keys++;
      }
    }
    _capacity = START_SIZE;
    _size = 0;
    while (UPPER * _capacity < unique_keys)
    {
      _capacity *= BASE;
    }
    buckets = new std::vector<std::pair<KeyT, ValueT>>[_capacity] ();
    for (int i = 0; i < unique_keys; ++i)
    {
      auto pair = std::make_pair (keys[i], values[i]);
      int id = hash (keys[i]);
      buckets[id].push_back (pair);
      _size++;
    }
  }
  /**
   * copy constructor
   * @param hm hushmap to copy
   */
  HashMap (const HashMap &hm)
  {
    _capacity = hm.capacity ();
    _size = hm.size ();
    buckets = new std::vector<std::pair<KeyT, ValueT>>[_capacity] ();
    for (int i = 0; i < _capacity; ++i)
    {
      if (hm.buckets[i].size () != 0)
      {
        buckets[i] = hm.buckets[i];
      }
    }
  }
  /**
   * destructor
   */
  virtual ~HashMap ()
  {
    delete[] buckets;
  }
  /**
   *
   * @return size of HM
   */
  int size () const
  {
    return _size;
  }
  /**
   *
   * @return capacity of HM
   */
  int capacity () const
  {
    return _capacity;
  }
  /**
   *
   * @returntrue if HM is empty, else false
   */
  bool empty () const
  {
    return size () == 0;
  }
  /**
   * adds key and value to HM (if key doesn't exists)
   * @param key
   * @param val
   * @return true if success, else false
   */
  bool insert (const KeyT &key, const ValueT &val)
  {
    if (contains_key (key))
    {
      return false;
    }
    _size++;
    auto pair = std::make_pair (key, val);
    int id = hash (key);
    buckets[id].push_back (pair);
    if (get_load_factor () > UPPER)
    {
      rehash ();
    }
    return true;
  }
  /**
   *
   * @param key
   * @return true if key is already in HM
   */
  bool contains_key (const KeyT &key) const
  {
    for (int i = 0; i < _capacity; ++i)
    {
      if (buckets[i].size () != 0)
      {
        int k = 0;
        for (auto j = buckets[i].cbegin (); j < buckets[i].cend (); ++j)
        {
          if (buckets[i][k].first == key)
          {
            return true;
          }
          k++;
        }
      }
    }
    return false;
  }
  /**
   *
   * @return load factor of HM
   */
  double get_load_factor () const
  {
    if (_size == 0)
    {
      return 0;
    }
    return ((double) _size) / ((double) _capacity);
  }
  /**
   *
   * @param key
   * @return the value at the key index in HM
   */
  ValueT at (const KeyT &key) const
  {
    int id = hash (key);
    if (buckets[id].size () != 0)
    {
      int k = 0;
      for (auto j = buckets[id].cbegin (); j < buckets[id].cend (); ++j)
      {
        if (buckets[id][k].first == key)
        {
          return buckets[id][k].second;
        }
        k++;
      }
    }
    throw std::runtime_error (KEY_NOT_FOUND);
  }
  /**
 *
 * @param key
 * @return the value at the key index in HM by ref
 */
  ValueT &at (const KeyT &key)
  {
    int id = hash (key);
    if (buckets[id].size () != 0)
    {
      int k = 0;
      for (auto j = buckets[id].cbegin (); j < buckets[id].cend (); ++j)
      {
        if (buckets[id][k].first == key)
        {
          return buckets[id][k].second;
        }
        k++;
      }
    }
    throw std::runtime_error (KEY_NOT_FOUND);
  }
  /**
   *
   * @param key
   * @return returns size of key's bucket
   */
  int bucket_size (const KeyT &key) const
  {
    int id = hash (key);
    if (buckets[id].size () != 0)
    {
      int k = 0;
      for (auto j = buckets[id].cbegin (); j < buckets[id].cend (); ++j)
      {
        if (buckets[id][k].first == key)
        {
          return buckets[id].size ();
        }
        k++;
      }
    }
    throw std::runtime_error (KEY_NOT_FOUND);
  }
  /**
   *
   * @param key
   * @return
   */
  int bucket_index (const KeyT &key) const
  {
    int id = hash (key);
    if (buckets[id].size () != 0)
    {
      int k = 0;
      for (auto j = buckets[id].cbegin (); j < buckets[id].cend (); ++j)
      {
        if (buckets[id][k].first == key)
        {
          return id;
        }
        k++;
      }
    }
    throw std::runtime_error (KEY_NOT_FOUND);
  }
  /**
   *
   * @param key to remove
   * @return removes key value from HM
   */
  virtual bool erase (const KeyT &key)
  {
    int id = hash (key);
    if (buckets[id].size () != 0)
    {
      int k = 0;
      for (auto j = buckets[id].cbegin (); j < buckets[id].cend (); ++j)
      {
        if (buckets[id][k].first == key)
        {
          buckets[id].erase (std::next (buckets[id].begin () + id));
          _size--;
          if (get_load_factor () < LOWER && get_load_factor () != 0)
          {
            rehash ();
          }
          if (_size == 0)
          {
            _capacity = 1;
          }
          return true;
        }
        k++;
      }
    }
    return false;
  }
  /**
   * clears all items in HM
   * capacity doesn't change
   */
  void clear ()
  {
    if (_size!=0)
    {
      for (int i = 0; i < _capacity; ++i)
      {
        if (buckets[i].size () != 0)
        {
          for (auto j = buckets[i].cbegin (); j < buckets[i].cend (); ++j)
          {
            _size-= buckets[i].size();
            buckets[i].clear();
          }
        }
      }
    }
  }
  /**
   *
   * @param hm to
   * @return ref to HM after assignment
   */
  HashMap &operator= (const HashMap& hm)
  {
    if (this == &hm)
    {
      return *this;
    }
    _capacity = hm.capacity ();
    _size = hm.size ();
    buckets = new std::vector<std::pair<KeyT, ValueT>>[_capacity] ();
    for (int i = 0; i < _capacity; ++i)
    {
      if (hm.buckets[i].size () != 0)
      {
        buckets[i] = hm.buckets[i];
      }
    }
    return *this;
  }
  /**
   *
   * @param key
   * @return the value at key
   */
  ValueT operator[] (const KeyT &key) const
  {
    int id = hash (key);
    if (buckets[id].size () != 0)
    {
      int k = 0;
      for (auto j = buckets[id].cbegin (); j < buckets[id].cend (); ++j)
      {
        if (buckets[id][k].first == key)
        {
          return buckets[id][k].second;
        }
        k++;
      }
    }
    return ValueT ();
  }
  /**
   *
   * @param key
   * @return the value at key by ref
   */
  ValueT &operator[] (const KeyT &key)
  {
    if (!contains_key (key))
    {
      insert (key, ValueT ());
    }
    return at (key);
  }
  /**
   *
   * @param hm to compare
   * @return true if all keys&values are at both HMs, else false
   */
  bool operator== (const HashMap &hm) const
  {
    if (_size != hm.size ())
    {
      return false;
    }
    return (all_items (*this, hm) && all_items (hm, *this));
  }
  /**
   *
   * @param hm to compare
   * @return true if not== to hm
   */
  bool operator!= (const HashMap &hm) const
  {
    if (_size != hm.size ())
    {
      return true;
    }
    return !(*this == hm);
  }

  /**
   * ConstIterator forward only
   */
  class ConstIterator
  {
    friend class HashMap<KeyT, ValueT>;

   private:
    const HashMap &_hm;
    int _buckets_ind;
    int _vec_ind;

   public:

    typedef std::pair<KeyT, ValueT> value_type;
    typedef const std::pair<KeyT, ValueT> &reference;
    typedef const std::pair<KeyT, ValueT> *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    /**
     * Constructor
     * @param hm
     * @param bi
     * @param vi
     */
    ConstIterator (const HashMap<KeyT, ValueT> &hm,
                   int bi = 0, int vi = 0) : _hm (hm),
                                             _buckets_ind (bi),
                                             _vec_ind (vi)
    {}
    /**
     *
     * @return the next iter
     */
    ConstIterator &operator++ ()
    {
      if (_vec_ind < _hm.buckets[_buckets_ind].size () - 1)
      {
        _vec_ind++;
        return *this;
      }
      _vec_ind = 0;
      _buckets_ind++;
      while (_buckets_ind < _hm._capacity
             && _hm.buckets[_buckets_ind].size () == 0)
      {
        _buckets_ind++;
      }
      return *this;
    }
    /**
     *
     * @return next iter after func
     */
    ConstIterator &operator++ (int)
    {
      ConstIterator it (_hm, _buckets_ind, _vec_ind);
      this->operator++();
      return it;
    }
    /**
     *
     * @param rhs
     * @return true if both iters are same, else false
     */
    bool operator== (const ConstIterator &rhs) const
    {
      return (&_hm == &rhs._hm && _buckets_ind == rhs._buckets_ind &&
              _vec_ind == rhs._vec_ind);
    }
    /**
     *
     * @param rhs
     * @returnif false if both iters are same, else true
     */
    bool operator!= (const ConstIterator &rhs) const
    {
      return !((*this) == rhs);
    }

    reference operator* () const
    {
      return _hm.buckets[_buckets_ind][_vec_ind];
    }

    pointer operator-> () const
    {
      return &(operator* ());
    }
  };

// public:
  using const_iterator = ConstIterator;

  const_iterator begin () const
  {
    if (empty ())
    {
      return ConstIterator (*this, _capacity, 0);
    }
    for (int i = 0; i < _capacity; ++i)
    {
      if (buckets[i].size () != 0)
      {
        return ConstIterator (*this, i, 0);
      }
    }
    throw std::out_of_range (END_BUT_FULL);
  }

  const_iterator end () const
  {
    return ConstIterator (*this, _capacity, 0);
  }

  const_iterator cbegin () const
  {
    return begin ();
  }
  const_iterator cend () const
  {
    return end ();
  }

};

#endif