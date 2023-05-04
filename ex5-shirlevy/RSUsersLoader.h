//
// Created on 2/21/2022.
//

#include "RSUser.h"
#include "RecommenderSystem.h"
#include <iostream>
#include <vector>
#include <memory>

#ifndef SCHOOL_SOLUTION_USERFACTORY_H
#define SCHOOL_SOLUTION_USERFACTORY_H

class RSUsersLoader
{
 private:
  friend std::vector<sp_movie> get_movies (std::string& line,

                                    std::shared_ptr<RecommenderSystem>& rs);


 public:
  RSUsersLoader () = delete;
  /**
   *
   * loads users by the given format with their movie's ranks
   * @param users_file_path a path to the file of the users and
   * their movie ranks
   * @param rs RecommendingSystem for the Users
   * @return vector of the users created according to the file
   */
  static std::vector<RSUser>
  create_users_from_file (const std::string &users_file_path,
                          std::unique_ptr<RecommenderSystem> rs)
                          noexcept (false);

};

#endif //SCHOOL_SOLUTION_USERFACTORY_H
