//
// Created by shirl on 08/06/2022.
//

#include <fstream>
#include "RecommenderSystemLoader.h"

#define BAD_FILE "file doesn't open properly"
#define BAD_RANK "rank is not between 1 to 10"
#define HYPHEN '-'
#define SPACE ' '
#define MIN_FEATURE 1
#define MAX_FEATURE 10

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file (const std::string
                                                     &movies_file_path)
noexcept (false)
{
  std::unique_ptr<RecommenderSystem> rs = std::make_unique<RecommenderSystem>
      ();

  std::ifstream input_file (movies_file_path);
//  input_file.open (movies_file_path);
  if (input_file.good ())
  {

    std::string line;
    std::string name, year_st;
    int year;
    while (std::getline (input_file, line))
    {
      std::size_t pos = line.find (HYPHEN);
      name = line.substr (0, line.find (HYPHEN));
      pos = line.find (HYPHEN) + 1;
      year_st = line.substr (pos, line.find (SPACE, pos));
      year = std::stoi (year_st);
      pos = line.find (SPACE, pos) + 1;

      std::vector<double> features;
      std::string feature_st;
      double feature;
      while (line.find (SPACE, pos) != std::string::npos)
      {
        feature_st = line.substr (pos, line.find (SPACE, pos));
        pos = line.find (SPACE, pos) + 1;
        feature = std::stod (feature_st);
        if (feature < MIN_FEATURE || feature > MAX_FEATURE)
        {
          throw std::runtime_error (BAD_RANK);
        }
        features.push_back (feature);
      }
      feature_st = line.substr (pos, line.find (SPACE, pos));
      pos = line.find (SPACE, pos) + 1;
      feature = std::stod (feature_st);
      if (feature < MIN_FEATURE || feature > MAX_FEATURE)
      {
        throw std::runtime_error (BAD_RANK);
      }
      features.push_back (feature);

      rs->add_movie (name, year, features);
    }
    input_file.close ();
    return rs;
  }
  else
  {
    throw std::runtime_error (BAD_FILE);
  }
}

