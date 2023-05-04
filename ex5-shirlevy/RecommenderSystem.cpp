//
// Created by shirl on 05/06/2022.
//
#include "RecommenderSystem.h"
#include <cmath>

//#define FEATURES_SIZE 4

/**
 * Constructor
 */
RecommenderSystem::RecommenderSystem ()
{
  _movies_set = {};
}

/**
 *
 * @param ranks all user ranks
 * @return ranks average
 */
double average (const rank_map &ranks)
{
  double sum = 0;
  for (std::pair<sp_movie, double> rank: ranks)
  {
    sum += rank.second;
  }
  return sum / (double (ranks.size ()));
}

/**
 *
 * @param ranks all user ranks
 * @param avrg ranks average
 * @return normelized rank_map (ranks minus average)
 */
//rank_map normalize (const rank_map& ranks, double avrg)
//{
//  rank_map normalized(hash_func,equal_func);
//  for (std::pair<sp_movie, double> rank:ranks)
//  {
////    normalized.insert({rank.first, rank.second - avrg});
//    normalized[rank.first] = rank.second - avrg;
//  }
//  return normalized;
//}

/**
 *
 * @param vec vector
 * @param s scalar
 * @return vector multiplied by the scalar
 */
std::vector<double> mult_vec_scalar (const
                                     std::vector<double> &vec, double s)
{
  std::vector<double> mult_vec;
  for (size_t i = 0; i < vec.size (); ++i)
  {
    mult_vec.push_back (vec[i] * s);
  }
  return mult_vec;
}

/**
 *
 * @param vec1
 * @param vec2
 * @return vector of vec1+vec2
 */
std::vector<double> adding_vecs (const std::vector<double> &vec1,
                                 const std::vector<double> &vec2)
{
  std::vector<double> sum_vec;
  for (size_t i = 0; i < vec1.size (); ++i)
  {
    sum_vec.push_back (vec1[i] + vec2[i]);
  }
  return sum_vec;
}

/**
 *
 * @param vec1
 * @param vec2
 * @return vector of vec1*vec2
 */
double mult_vecs (const std::vector<double> &vec1, const
std::vector<double> &vec2)
{
  double sum = 0;
  std::vector<double> sum_vec;
  for (size_t i = 0; i < vec1.size (); ++i)
  {
    sum += vec1[i] * vec2[i];
  }
  return sum;
}

/**
 *
 * @param normalized normalized ranks vec
 * @return pref features of user
 */
std::vector<double> RecommenderSystem::pref_vec (const rank_map &ranks,
                                                 double avrg)
{
  std::vector<double> res;
  std::vector<double> prefs ={0};//(FEATURES_SIZE, 0.0);
  for (std::pair<sp_movie, double> rank: ranks)
  {
    res = _movies_set[rank.first];
    res = mult_vec_scalar (res, rank.second - avrg);
    prefs = adding_vecs (prefs, res);
  }
  return prefs;
}

/**
 *
 * @param vec
 * @return norm of vec
 */
double get_norm (const std::vector<double> &vec)
{
  double sum_squered = 0.0;
  for (double elem: vec)
  {
    sum_squered += elem * elem;
  }
  return sqrt (sum_squered);
}

/**
 *
 * @param vec1
 * @param vec2
 * @return similarity dig between vec1, vec2
 */
double
similarity (const std::vector<double> &vec1, const std::vector<double> &vec2)
{
  double up = mult_vecs (vec1, vec2);
  double down = get_norm (vec1) * get_norm (vec2);
  return up / down;
}

/**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
sp_movie RecommenderSystem::add_movie (const std::string &name, int year,
                                       const std::vector<double> &features)
{
  sp_movie movie = std::make_shared<Movie> (Movie (name, year));
  _movies_set[movie] = features;
  return movie;
}

/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  sp_movie movie = std::make_shared<Movie> (name, year);
  if (_movies_set.find (movie) != _movies_set.end ())
  {
    return _movies_set.find (movie)->first;
  }
  return nullptr;

//  for (std::pair<sp_movie, std::vector<double>> movie:_movies_set)
//  {
//    if (movie.first->get_name() == name && movie.first->get_year() == year)
//    {
//      return movie.first;
//    }
//  }
//  return nullptr;
}

/**
 * a function that calculates the movie with highest score based on movie
 * features
 * @param ranks user ranking to use for algorithm
 * @return shared pointer to movie in system
 */
sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  double avrg = average (user.get_ranks ());
//  rank_map normalized = normalize (user.get_ranks(), avrg);
  std::vector<double> prefs = this->pref_vec (user.get_ranks (), avrg);

  sp_movie max_movie = nullptr;
  double max = 0, res = 0;
  for (std::pair<sp_movie, std::vector<double>> movie: _movies_set)
  {
    if (user.get_ranks ().find (movie.first) == user.get_ranks ().end ())
    {
      res = similarity (prefs, _movies_set[movie.first]);
      if (res > max)
      {
        max_movie = movie.first;
        max = res;
      }
    }
  }
  return max_movie;
}

/**
 * a function that calculates the movie with highest predicted score based
 * on ranking of other movies
 * @param ranks user ranking to use for algorithm
 * @param k
 * @return shared pointer to movie in system
 */
sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  sp_movie max_movie = nullptr;
  double max = 0, res;
  for (std::pair<sp_movie, std::vector<double>> movie: _movies_set)
  {
    if (user.get_ranks ().find (movie.first) == user.get_ranks ().end ())
    {
      res = predict_movie_score (user, movie.first, k);
      if (res > max)
      {
        max_movie = movie.first;
        max = res;
      }
    }
  }
  return max_movie;
}

double
RecommenderSystem::predict_movie_score (const RSUser &user,
                                        const sp_movie &movie, int k)
{
  double res = 0, sum_up = 0, sum_down = 0;
  std::multimap<double, sp_movie, std::greater<>> k_similar;
  for (std::pair<sp_movie, double> rank: user.get_ranks ())
  {
    res = similarity (_movies_set[movie], _movies_set[rank.first]);
    k_similar.insert ({res, rank.first});
  }
  int i = 0;
  for (std::pair<double, sp_movie> similar: k_similar)
  {
    i++;
    sum_up += similar.first * user.get_ranks ()[similar.second];
    sum_down += similar.first;
    if (i >= k)
    {
      break;
    }
  }
  return sum_up / sum_down;
}

/**
 *
 * @param os
 * @param rs
 * @return
 */
std::ostream &operator<< (std::ostream &os, const RecommenderSystem &rs)
{
  for (const auto &movie: rs._movies_set)
  {
    os << *(movie.first);
  }
  return os;
}



