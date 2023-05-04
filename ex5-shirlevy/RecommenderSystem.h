//
// Created on 2/20/2022.
//

#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include <set>
#include <map>
#include <vector>
#include <functional>
#include "RSUser.h"

struct comp_movie
{
    bool operator() (const sp_movie &lhs, const sp_movie &rhs) const
    {
      return *lhs < *rhs;
    }
};

class RecommenderSystem
{
//  double average (const rank_map& ranks);
//  rank_map normalize (const rank_map& ranks, double avrg);
//  friend std::vector<double> mult_vec_scalar(const std::vector<double>& vec,
//                                       double s)
//


  std::vector<double> pref_vec (const rank_map &ranks, double avrg);
//  double average (const rank_map& ranks);
//  rank_map normalize (const rank_map& ranks, double avrg);
  ///TODO: CHECK IF HUSH FUNC
  std::map<sp_movie, std::vector<double>, comp_movie> _movies_set;
//  std::vector<double> pref_vec(const rank_map& normalized);
 public:

  /**
   * Constructor
   */
  RecommenderSystem ();

  //explicit RecommenderSystem()


  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year,
             const std::vector<double> &features);

  /**
  * a function that calculates the movie with highest score based on movie
   * features
  * @param ranks user ranking to use for algorithm
  * @return shared pointer to movie in system
  */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score
   * based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf
   * procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  /**
   *
   * @param os
   * @param rs
   * @return
   */
  friend std::ostream &operator<< (std::ostream &os, const RecommenderSystem &
  rs);
};

#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
