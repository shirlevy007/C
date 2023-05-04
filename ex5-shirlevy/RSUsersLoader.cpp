//
// Created by shirl on 09/06/2022.
//
#include <fstream>
#include "RSUsersLoader.h"

#define BAD_FILE "file doesn't open properly"
#define BAD_RANK "rank is not between 1 to 10"
#define HYPHEN '-'
#define SPACE ' '
#define ENDOFLINE '\r'
#define MIN_RANK 1
#define MAX_RANK 10
#define NA "NA"
#define YEAR_LEN 4

std::vector<sp_movie> get_movies (std::string &line,
                                  std::shared_ptr<RecommenderSystem> &rs)
{
  std::string name, year_st;
  std::vector<sp_movie> movies;
  int year;
  std::size_t pos = 0;
  while (line.find (SPACE, pos) != std::string::npos)
  {
    name = line.substr (pos, line.find (HYPHEN));
    line.erase (pos, line.find (HYPHEN) + 1);
    year_st = line.substr (pos, YEAR_LEN);
    year = std::stoi (year_st);
    line.erase (pos, YEAR_LEN + 1);
    movies.push_back (rs->get_movie (name, year));
  }
  name = line.substr (pos, line.find (HYPHEN));
  line.erase (pos, line.find (HYPHEN) + 1);
  year_st = line.substr (pos, YEAR_LEN);
  year = std::stoi (year_st);
  line.erase (pos, YEAR_LEN + 1);
  movies.push_back (rs->get_movie (name, year));
  return movies;
}

std::vector<RSUser>
RSUsersLoader::create_users_from_file (const std::string &users_file_path,
                                       std::unique_ptr<RecommenderSystem> _rs)
noexcept (false)
{
  std::shared_ptr<RecommenderSystem> rs = std::move (_rs);
  std::ifstream input_file (users_file_path);
  if (input_file.good ())
  {
    std::string line, name;
    std::getline (input_file, line); ///only first line - movies
    std::vector<sp_movie> movies = get_movies (line, rs);
    std::vector<RSUser> users;
    std::string rank_st;
    double rank;
    int pos = 0;
    while (std::getline (input_file, line))
    {
      rank_map ranks{0, sp_movie_hash, sp_movie_equal};
      int i = 0;
      name = line.substr (pos, line.find (SPACE));
      line.erase (pos, line.find (SPACE) + 1);
      while (line.find (SPACE) != std::string::npos)
      {
        rank_st = line.substr (pos, line.find (SPACE));
        line.erase (pos, line.find (SPACE) + 1);
        if (rank_st != NA)
        {
          rank = std::stod (rank_st);
          if (rank < MIN_RANK || rank > MAX_RANK)
          {
            throw std::runtime_error (BAD_RANK);
          }
          ranks[movies[i]] = rank;
        }
        i++;
      }
      rank_st = line.substr (pos, line.find (ENDOFLINE));
      line.erase (pos, line.find (ENDOFLINE) + 1);
      if (rank_st != NA)
      {
        rank = std::stod (rank_st);
        if (rank < MIN_RANK || rank > MAX_RANK)
        {
          throw std::runtime_error (BAD_RANK);
        }
        ranks[movies[i]] = rank;
      }
      users.emplace_back (name, ranks, rs);
    }
    return users;
  }
  else
  {
    throw std::runtime_error (BAD_FILE);
  }
}
//
//void push_ranks (std::string line, const std::vector<RSUser>& users,
//                 const std::vector<sp_movie>& movies,
//                 std::shared_ptr<RecommenderSystem> rs)
//{
//  rank_map ranks{0, sp_movie_hash, sp_movie_equal};
//  std::string rank_st;
//  double rank;
//  int i = 0, pos=0;
//  std::string name = line.substr (pos, line.find (SPACE));
//  line.erase (pos, line.find (SPACE) + 1);
//  while (line.find (SPACE) != std::string::npos)
//  {
//    rank_st = line.substr (pos, line.find (SPACE));
//    line.erase (pos, line.find (SPACE) + 1);
//    if (rank_st == NA)
//    {
//      i++;
//      continue;
//    }
//    rank = std::stod (rank_st);
//    if (rank < MIN_RANK || rank > MAX_RANK)
//    {
//      throw std::runtime_error (BAD_RANK);
//    }
//    ranks[movies[i]] = rank;
//    i++;
//  }
//  users.emplace_back (name, ranks, rs);
//}



