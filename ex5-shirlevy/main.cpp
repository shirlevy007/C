#include "RSUser.h"
#include "RSUsersLoader.h"
#include "RecommenderSystem.h"
#include "RecommenderSystemLoader.h"
#include "Movie.h"

int main ()
{
  std::cout << "** Testing rec system **" << std::endl;
  std::shared_ptr<RecommenderSystem> rec_system =
      std::make_shared<RecommenderSystem> ();
  rec_system->add_movie ("Twilight", 2008, std::vector<double> {3,4,6,5});
  rec_system->add_movie ("Batman", 2022, std::vector<double> {2,6,4,8});
  rec_system->add_movie ("StarWars", 1977, std::vector<double> {3,3,4,9});

  std::cout << (*rec_system);
  std::cout << std::endl;

  /** sofia */
  rank_map rank_map1 = rank_map {0, sp_movie_hash, sp_movie_equal};
  std::string user_name = "sofia";
  RSUser user = RSUser (user_name, rank_map1, rec_system);

  user.add_movie_to_rs ("Titanic", 1992, std::vector<double> {7,2,9,1}, 4);
  user.add_movie_to_rs ("ForestGump", 1994, std::vector<double> {1,7,7,6}, 8);

  std::cout << "** Testing rec by content **" << std::endl;
  sp_movie m = user.get_recommendation_by_content();
  std::cout << m->get_name() << std::endl;
  std::cout << "**" << std::endl;

  std::cout << user << std::endl;
  std::cout << std::endl;


  /** nicole */
  user_name = "nicole";
  rank_map rank_map2 = rank_map {0, sp_movie_hash, sp_movie_equal};
  RSUser nicole = RSUser (user_name, rank_map2, rec_system);

  nicole.add_movie_to_rs ("StarWars", 1977, std::vector<double> {3,3,4,9}, 6);
  nicole.add_movie_to_rs ("Batman", 2022, std::vector<double> {2,6,4,8}, 4);
  nicole.add_movie_to_rs ("ForestGump", 1994, std::vector<double> {1,7,7,6}, 5);

  std::cout << "** Testing predict **" << std::endl;
  double pred = nicole.get_prediction_score_for_movie ("Titanic", 1992, 2);
  std::cout << pred << std::endl;
  std::cout << std::endl;

  std::cout << "** Testing rec by cf **" << std::endl;
  m = nicole.get_recommendation_by_cf (2);
  std::cout << m->get_name() << std::endl;
  std::cout << std::endl;

  std::cout << "** Test loaders **" << std::endl;
  std::unique_ptr<RecommenderSystem> rs =
      RecommenderSystemLoader::create_rs_from_movies_file
      ("/mnt/c/RecommenderSystemLoader_input.txt");
  std::cout << (*rs);
  std::cout << "**" << std::endl;
  std::vector<RSUser> users = RSUsersLoader::create_users_from_file
      ("/mnt/c/RSUsersLoader_input.txt", std::move(rs));

  for (const auto& user_v : users)
  {
    std::cout << user_v;
    std::cout << "**" << std::endl;
  }
  return 0;
}