#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
//#include<cstdio>
//#include <iostream>
#include "HashMap.hpp"

#define KEY_NOT_FOUND_DICT "the key given is not in the HushMap"
#define INVALID_KEY "Invalid Key"

class InvalidKey : public std::invalid_argument{
 public:
  InvalidKey() : std::invalid_argument(INVALID_KEY) {}
  InvalidKey (const std::string &error) : std::invalid_argument
                                              (error) {}
};


class Dictionary: public HashMap<std::string, std::string>
{
//  HashMap<std::string, std::string> _hm;
 public:
  Dictionary() = default;

  Dictionary(const std::vector<std::string> &_keys, const
  std::vector<std::string> &_values): HashMap<std::string, std::string>
      (_keys, _values)
  {}

  Dictionary(const HashMap &hm): HashMap<std::string, std::string> (hm)
  {}

  virtual bool erase(const std::string &key) override
  {
    if (contains_key (key))
    {
      return HashMap::erase (key);
    }
    else
    {
      throw InvalidKey();
    }
  }

  template<class T>
      void update (T _begin, T end)
  {
    for (T cur = _begin; cur < end; cur++)
    {
      auto to_update = *cur;
      operator[](to_update.first) =  to_update.second;
    }
  }


};

#endif