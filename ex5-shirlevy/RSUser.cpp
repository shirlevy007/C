//
// Created on 2/20/2022.
//

// don't change those includes
#include "RSUser.h"

#include <utility>
#include "RecommenderSystem.h"

/**
 *
 * @param username
 * @param rating
 * @param RecommenderSystem
 */
RSUser::RSUser (const std::string &username, rank_map &ranks,
                std::shared_ptr<RecommenderSystem> rs) : _username (username),
                                                         _ranks (ranks), _rs (std::move (rs))
{}

/**
 * a getter for the user's name
 * @return the username
 */
std::string RSUser::get_name () const
{
  return _username;
}

/**
	 * function for adding a movie to the DB
	 * @param name name of movie
     * @param year year it was made
	 * @param features a vector of the movie's features
	 * @param rate the user rate for this movie
	 */
void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const std::vector<double> &features, double rate)
{
  sp_movie movie;
  if (_rs->get_movie (name, year) != nullptr)
  {
    movie = _rs->get_movie (name, year);
  }
  else
  {
    movie = _rs->add_movie (name, year, features);
  }
//  _ranks.insert({movie, rate});
//  _rs->_movies_set[movie] = features;
  _ranks[movie] = rate;
}

/**
 * a getter for the ranks map
 * @return rank map of user
 */
rank_map RSUser::get_ranks () const
{
  return _ranks;
}

/**
 * returns a recommendation according to the movie's content
 * @return recommendation
 */
sp_movie RSUser::get_recommendation_by_content () const
{
  return _rs->recommend_by_content (*this);
}

/**
 *
 * returns a recommendation according to the similarity recommendation method
 * @param k the number of the most similar movies to calculate by
 * @return recommendation
 */
sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return _rs->recommend_by_cf (*this, k);
}

/**
 * predicts the score for a given movie
 * @param name the name of the movie
 * @param year the year the movie was created
 * @param k the parameter which represents the number of the most similar
 * movies to predict the score by
 * @return predicted score for the given movie
 */
double
RSUser::get_prediction_score_for_movie (const std::string &name, int year,
                                        int k) const
{
  sp_movie movie = std::make_shared<Movie> (Movie (name, year));
  return _rs->predict_movie_score (*this, movie, k);
}
/**
 * output stream operator
 * @param os the output stream
 * @param user the user
 * @return output stream
 */
std::ostream &operator<< (std::ostream &os, const RSUser &user)
{
  os << "name: " << user.get_name () << std::endl;
//  for (std::pair<sp_movie, double> movie: user._rs)
//  {
//    os << *(movie.first);
//  }
  os << *(user._rs);
  os << std::endl;
  return os;
}





