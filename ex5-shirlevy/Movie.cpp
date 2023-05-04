
#include "Movie.h"
#include <cmath>
#define HASH_START 17
#define RES_MULT 31

#define FEATURES_SIZE 5

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash (const sp_movie &movie)
{
  std::size_t res = HASH_START;
  res = res * RES_MULT + std::hash<std::string> () (movie->get_name ());
  res = res * RES_MULT + std::hash<int> () (movie->get_year ());
  return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal (const sp_movie &m1, const sp_movie &m2)
{
  return !(*m1 < *m2) && !(*m2 < *m1);
}

/**
     * constructor
     * @param name: name of movie
     * @param year: year it was made
     */
Movie::Movie (const std::string &name, int year) : _name (name), _year (year)
{
//  _fitures = new int[FEATURES_SIZE]{0};
//  _id = new sp_movie;
}

/**
 * returns the name of the movie
 * @return const ref to name of movie
 */
std::string Movie::get_name () const
{
  return _name;
}

/**
 * returns the year the movie was made
 * @return year movie was made
 */
int Movie::get_year () const
{
  return _year;
}

/**
 * operator< for two movies
 * @param rhs: right hand side
 * @param lhs: left hand side
 * @return returns true if (lhs.year) < rhs.year or
 * (rhs.year == lhs.year & lhs.name < rhs.name) else return false
 *NOTICE: BAD DOCUMENTATION OF THE EX HERE! THEY MIXED IT!
 */
bool operator< (const Movie &lhs, const Movie &rhs)
{
  if (lhs.get_year () < rhs.get_year ())
  {
    return true;
  }
  if (lhs.get_year () > rhs.get_year ())
  {
    return false;
  }
  int len = std::min (rhs.get_name ().length (), rhs.get_name ().length ());
  for (int i = 0; i < len; ++i)
  {
    if (int (lhs.get_name ()[i]) == int (rhs.get_name ()[i])) //ASCII value
    {
      continue;
    }
    return int (lhs.get_name ()[i]) < int (rhs.get_name ()[i]);
  }
  int rhs_len = static_cast<int>(rhs.get_name ().length ());
  return rhs_len > len;
//  if (rhs.get_name ().length () > len)
//  {
//    return true;
//  }
//  return false;
}

/**
    * operator<< for movie
    * @param os ostream to output info with
    * @param movie movie to output
    */
std::ostream &operator<< (std::ostream &os, const Movie &movie)
{
  os << movie.get_name () << " (" << movie.get_year () << ")" << std::endl;
  return os;
}




