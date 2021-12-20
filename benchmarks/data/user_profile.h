#pragma once

#include <string>
#include <vector>

#include "er/attributes.h"

ER_REFLECT()
struct UserProfile {
  uint64_t user_id;

  struct UserName {
    std::string first_name;
    std::string second_name;
    std::string last_name;
  } name;

  std::string homeworld;

  std::vector<std::string> affiliation;

  std::string profile_photo_url;

  std::vector<uint64_t> friends;

  enum class Grade { kLiked, kDisliked };

  struct Activity {
    std::string photo_url;
    Grade grade;
  };

  std::vector<Activity> last24hours_activity;
};
