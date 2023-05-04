#include <sstream>
#include "RecommenderSystem.h"
#include "RecommenderSystemLoader.h"
#include "RSUsersLoader.h"
#include <cmath>

#define FUNC(name) std::make_pair(#name, name)
#define IS_TRUE(x) IS_TRUE_MSG(x, "")
#define IS_TRUE_MSG(x, msg) if (!(x)) { std::cout << __FUNCTION__ << " failed on line " << __LINE__ << ". Message: " << msg << std::endl; return false; }
#define RAISES_ERROR(error, function, ...) try { function ( __VA_ARGS__ ); return false; } catch (const error &Error) { std::cout << "Got Expected error: " << Error.what() << std::endl; }
#define EPSILON 1e-7
#define COMP_FLOAT(x, y, epsilon) IS_TRUE_MSG(std::fabs (x - y) < epsilon, x << " !~= " << y << " (" << epsilon << " precision)")
#define SUCCESS "\033[1;32mSUCCESS\033[0m"
#define FAILURE "\033[1;31mFAILURE\033[0m"
#define MAGNETA_S "\033[1;35m"
#define MAGNETA_E "\033[0m"
#define BLUE_START "\033[1;34m"
#define BLUE_END "\033[0m"

typedef std::vector<double> feature_vec;
typedef std::shared_ptr<rank_map> sp_rank_map;
typedef std::shared_ptr<RecommenderSystem> sp_rs;
typedef std::unique_ptr<RecommenderSystem> up_rs;
// Change this according to where you run this
#define FILE_FORMAT(name) ("/mnt/c/" name "")

RecommenderSystem* create_recommender_system () {
  auto rs = std::make_unique<RecommenderSystem>();
  auto titanic = std::vector<double> { 7, 2, 9, 1};
  rs->add_movie ("Titanic", 1997, titanic);
  auto forest = std::vector<double> { 1, 7, 7, 6};
  rs->add_movie ("ForestGump", 1994, forest);
  auto batman = std::vector<double> { 2, 6, 4, 8};
  rs->add_movie ("Batman", 2022, batman);
  auto twilight = std::vector<double> { 3, 4, 6, 5};
  rs->add_movie ("Twilight", 2008, twilight);
  auto star_wars = std::vector<double> { 3, 3, 4, 9};
  rs->add_movie ("StarWars", 1977, star_wars);
  return rs.release();
}

rank_map *create_rank_map () {
  auto ranks = std::make_unique<rank_map>(0, sp_movie_hash, sp_movie_equal);
  return ranks.release();
}


bool test_recommender_system_print () {
  std::unique_ptr<RecommenderSystem> rs ( create_recommender_system () );
  // Check lexicographic order of upper case vs lower case
  rs->add_movie ("abcd", 1977, feature_vec {8, 7, 6, 5});
  std::stringstream os;
  os << *rs;
  IS_TRUE_MSG(os.str() == "StarWars (1977)\nabcd (1977)\nForestGump (1994)\nTitanic (1997)\nTwilight (2008)\nBatman (2022)\n",
              "Unexpected output, got: " << os.str())
  return true;
}

bool test_movies_are_added () {
  std::unique_ptr<RecommenderSystem> rs ( create_recommender_system () );
  sp_movie titanic = rs->get_movie ("Titanic", 1997);
  IS_TRUE_MSG(titanic != nullptr, "Returned null, expected to get Titanic")
  IS_TRUE_MSG(titanic->get_name() == "Titanic", "Name is " << titanic->get_name() << " instead of Titanic")
  IS_TRUE_MSG(titanic->get_year() == 1997, "Year is " << titanic->get_year() << " instead of 1997")
  return true;
}

bool test_get_non_existent () {
  std::unique_ptr<RecommenderSystem> rs ( create_recommender_system () );
  sp_movie titanic = rs->get_movie ("titanic", 1997);
  IS_TRUE_MSG(titanic == nullptr, "Did not return null, got " << titanic->get_name())
  titanic = rs->get_movie ("Titanic", 1996);
  IS_TRUE_MSG(titanic == nullptr, "Did not return null, got " << titanic->get_name())
  return true;
}

bool test_content_prediction_multi_ranking () {
  std::shared_ptr<RecommenderSystem> rs ( create_recommender_system () );
  std::shared_ptr<rank_map> ranks = std::make_shared<rank_map>(1,
                                                               sp_movie_hash, sp_movie_equal);
  sp_movie t = rs->get_movie ("Titanic", 1997);
  (*ranks)[t] = 4;
  sp_movie f = rs->get_movie ("ForestGump", 1994);
  (*ranks)[f] = 8;
  RSUser bob ("Bob", *ranks, rs);
  IS_TRUE_MSG(bob.get_ranks().size() == 2, "Wrong rank size for bob")
  sp_movie rec = bob.get_recommendation_by_content();
  IS_TRUE_MSG(rec != nullptr, "Got null instead of recommendation")
  IS_TRUE_MSG(rec->get_name() == "Batman", "Did not return batman, got " << rec->get_name())
  return true;
}

bool test_cf_recommendation () {
  std::shared_ptr<RecommenderSystem> rs ( create_recommender_system () );
  sp_rank_map ranks ( create_rank_map() );

  sp_movie m = rs->get_movie ("StarWars", 1977);
  (*ranks)[m] = 6;
  m = rs->get_movie ("ForestGump", 1994);
  (*ranks)[m] = 5;
  m = rs->get_movie ("Batman", 2022);
  (*ranks)[m] = 3;
  RSUser bob ("Nicole", *ranks, rs);
  IS_TRUE_MSG(bob.get_ranks().size() == 3, "Wrong rank size for Nicole")
  sp_movie rec = bob.get_recommendation_by_cf(2);
  IS_TRUE_MSG(rec != nullptr, "Got null instead of recommendation")
  IS_TRUE_MSG(rec->get_name() == "Titanic", "Did not return titanic, got " << rec->get_name())
  return true;
}

bool test_movie_score () {
  std::shared_ptr<RecommenderSystem> rs ( create_recommender_system () );
  sp_rank_map ranks ( create_rank_map() );

  sp_movie m = rs->get_movie ("StarWars", 1977);
  (*ranks)[m] = 6;
  m = rs->get_movie ("ForestGump", 1994);
  (*ranks)[m] = 5;
  m = rs->get_movie ("Batman", 2022);
  (*ranks)[m] = 3;
  RSUser bob ("Nicole", *ranks, rs);
  IS_TRUE_MSG(bob.get_ranks().size() == 3, "Wrong rank size for Nicole")
  double pred = bob.get_prediction_score_for_movie("Titanic", 1997, 2);
  COMP_FLOAT(pred, 5.47, 0.1)
  pred = bob.get_prediction_score_for_movie("Titanic", 1997, 1);
  COMP_FLOAT(pred, 5, 0.1)
  return true;
}

bool test_movie_score_non_existent () {
  std::shared_ptr<RecommenderSystem> rs ( create_recommender_system () );
  sp_rank_map ranks ( create_rank_map() );

  sp_movie m = rs->get_movie ("StarWars", 1977);
  (*ranks)[m] = 6;
  RSUser bob ("Nicole", *ranks, rs);
  // Wrong year
  double pred = bob.get_prediction_score_for_movie("Titanic", 1992, 2);
  IS_TRUE_MSG(pred == 0, "Expected 0 because Titanic is from 1997")
  return true;
}

bool test_user_print () {
  std::shared_ptr<RecommenderSystem> rs ( create_recommender_system () );
  sp_rank_map ranks ( create_rank_map() );
  RSUser bob ("Bob", *ranks, rs);
  bob.add_movie_to_rs ("Pulp Fiction", 1994, feature_vec {8, 7, 6, 5}, 9);
  bob.add_movie_to_rs ("Kill Bill", 2003, feature_vec {1, 2, 3, 4}, 7);
  std::stringstream os;
  os << bob;
  IS_TRUE_MSG(os.str() == "name: Bob\nStarWars (1977)\nForestGump (1994)\nPulp Fiction (1994)\nTitanic (1997)\nKill Bill (2003)\nTwilight (2008)\nBatman (2022)\n\n",
              "Unexpected output, got: " << os.str())
  IS_TRUE_MSG(bob.get_name() == "Bob", "Got wrong name for bob != " << bob.get_name())
  return true;
}

bool test_load_from_file_negative () {
  RAISES_ERROR(std::exception,
               RecommenderSystemLoader::create_rs_from_movies_file,
               "wrong_file.txt")
  RAISES_ERROR(std::exception,
               RecommenderSystemLoader::create_rs_from_movies_file,
               FILE_FORMAT("RecommenderSystemLoader_bad_input1.txt"))
  RAISES_ERROR(std::exception,
               RecommenderSystemLoader::create_rs_from_movies_file,
               FILE_FORMAT("RecommenderSystemLoader_bad_input2.txt"))
  return true;
}

bool test_load_from_file_positive () {
  std::unique_ptr<RecommenderSystem> rs_1 =
      RecommenderSystemLoader::create_rs_from_movies_file(
      FILE_FORMAT("RecommenderSystemLoader_input.txt"));
  sp_rs rs = std::move(rs_1);
  std::stringstream os;
  os << *rs;
  IS_TRUE_MSG(os.str() == "StarWars (1977)\nForestGump (1994)\nTitanic (1997)\nTwilight (2008)\nBatman (2022)\n",
              "Unexpected output, got: " << os.str())
  sp_rank_map ranks ( create_rank_map() );
  RSUser bob ("Bob", *ranks, rs);
  bob.add_movie_to_rs ("Pulp Fiction", 1994, feature_vec {1, 1, 1, 1}, 10);
  bob.add_movie_to_rs ("Kill Bill", 2003, feature_vec {1, 1, 2, 2}, 9);
  // Check features are set correctly
  double score = bob.get_prediction_score_for_movie ("StarWars", 1977, 2);
  COMP_FLOAT(score, 9.484, 0.001)
  score = bob.get_prediction_score_for_movie ("Titanic", 1997, 2);
  COMP_FLOAT(score, 9.508, 0.001)
  return true;
}

bool test_load_from_file_long_feature_vector () {
  std::unique_ptr<RecommenderSystem> rs_1 =
      RecommenderSystemLoader::create_rs_from_movies_file(
      FILE_FORMAT("RecommenderSystemLoader_input_long_features.txt"));
  sp_rs rs = std::move(rs_1);
  std::stringstream os;
  os << *rs;
  IS_TRUE_MSG(os.str() == "StarWars (1977)\nForestGump (1994)\nTitanic (1997)\nTwilight (2008)\nBatman (2022)\n",
              "Unexpected output, got: " << os.str())
  sp_rank_map ranks ( create_rank_map() );
  RSUser bob ("Bob", *ranks, rs);
  bob.add_movie_to_rs ("Pulp Fiction", 1994, feature_vec {1, 1, 1, 1, 1}, 10);
  bob.add_movie_to_rs ("Kill Bill", 2003, feature_vec {1, 1, 1, 2, 2}, 9);
  // Check features are set correctly
  double score = bob.get_prediction_score_for_movie ("StarWars", 1977, 2);
  COMP_FLOAT(score, 9.48368, 0.001)
  score = bob.get_prediction_score_for_movie ("Titanic", 1997, 2);
  COMP_FLOAT(score, 9.522, 0.001)
  return true;
}

bool test_rs_user_loader_positive () {
  up_rs rs = RecommenderSystemLoader::create_rs_from_movies_file(
      FILE_FORMAT("RecommenderSystemLoader_input.txt"));
  RecommenderSystem my_rs = *rs;
  std::vector<RSUser> users = RSUsersLoader::create_users_from_file(
      FILE_FORMAT("RSUsersLoader_input.txt"), std::move(rs));
  IS_TRUE_MSG(users.size() == 4, "Wrong users size, expected 4 got " << users.size())

  IS_TRUE_MSG(users[0].get_name() == "Sofia", "Expected Sofia got " << users[0].get_name())
  rank_map ranks = users[0].get_ranks();
  IS_TRUE(ranks.size() == 2)
  IS_TRUE(ranks[my_rs.get_movie ("Twilight", 2008)] == 4)
  IS_TRUE(ranks[my_rs.get_movie ("ForestGump", 1994)] == 8)

  IS_TRUE_MSG(users[2].get_name() == "Nicole", "Expected Nicole got " << users[2].get_name())
  ranks = users[2].get_ranks();
  IS_TRUE(ranks.size() == 3)
  IS_TRUE(ranks[my_rs.get_movie ("ForestGump", 1994)] == 5)
  IS_TRUE(ranks[my_rs.get_movie ("Batman", 2022)] == 2)
  IS_TRUE(ranks[my_rs.get_movie ("StarWars", 1977)] == 6)

  IS_TRUE_MSG(users[3].get_name() == "Arik", "Expected Arik got " << users[3].get_name())
  ranks = users[3].get_ranks();
  IS_TRUE(ranks.size() == 2)
  IS_TRUE(ranks[my_rs.get_movie ("Titanic", 1997)] == 8)
  IS_TRUE(ranks[my_rs.get_movie ("Batman", 2022)] == 3)

  return true;
}

bool test_load_users_negative() {
  up_rs rs = RecommenderSystemLoader::create_rs_from_movies_file(
      FILE_FORMAT("RecommenderSystemLoader_input.txt"));
  RecommenderSystem my_rs = *rs;
  RAISES_ERROR(std::exception,
               RSUsersLoader::create_users_from_file,
               "wrong_file.txt", std::move (rs))
  IS_TRUE_MSG(my_rs.get_movie ("Titanic", 1997) != nullptr, "RS is not null ")
//  RAISES_ERROR(std::exception,
//               RecommenderSystemLoader::create_rs_from_movies_file,
//               FILE_FORMAT("RecommenderSystemLoader_bad_input1.txt"))
//  RAISES_ERROR(std::exception,
//               RecommenderSystemLoader::create_rs_from_movies_file,
//               FILE_FORMAT("RecommenderSystemLoader_bad_input2.txt"))
  return true;
}

typedef bool (*testFunc) ();

int main () {
  std::vector<std::pair<std::string, testFunc>> _tests {
    FUNC(test_movies_are_added),
    FUNC(test_get_non_existent),
    FUNC(test_recommender_system_print),
    FUNC(test_content_prediction_multi_ranking),
    FUNC(test_cf_recommendation),
    FUNC(test_movie_score),
    //FUNC(test_movie_score_non_existent),
    FUNC(test_user_print),
    FUNC(test_load_from_file_negative),
    FUNC(test_load_from_file_positive),
    FUNC(test_load_from_file_long_feature_vector),
    FUNC(test_rs_user_loader_positive),
    FUNC(test_load_users_negative),
  };
  int passed = 0;
  int failed = 0;
  for (std::pair<std::string, testFunc> &p : _tests) {
    std::cout << "\t- " << BLUE_START << p.first << BLUE_END << "\n";
    bool res = p.second();
    passed += (res ? 1 : 0);
    failed += (res ? 0 : 1);
    std::cout << BLUE_START << "\t\tTest summary - " << BLUE_END << (res ? SUCCESS : FAILURE) << std::endl ;
  }
  std::cout << MAGNETA_S << "\nSummary: " << (failed == 0 ? SUCCESS : FAILURE
  ) << MAGNETA_S << "\nPassed: " << passed << "\nFailed: " << failed << MAGNETA_E << std::endl ;
  return 0;
}
