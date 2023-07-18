#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct [[er::reflect]] Twitter {
  struct Status {
    struct Metadata {
      std::string result_type;
      std::string iso_language_code;
    } metadata;

    std::string created_at;
    uint64_t id;
    std::string id_str;
    std::string text;
    std::string source;
    bool truncated;
    uint64_t in_reply_to_status_id;
    std::string in_reply_to_status_id_str;
    uint64_t in_reply_to_user_id;
    std::string in_reply_to_user_id_str;
    std::string in_reply_to_screen_name;

    struct Url {
      std::string url;
      std::string expanded_url;
      std::string display_url;
      std::vector<uint32_t> indices;
    };

    struct User {
      uint64_t id;
      std::string id_str;
      std::string name;
      std::string screen_name;
      std::string location;
      std::string description;
      std::string url;

      struct Entities {
        struct Url {
          std::vector<Status::Url> urls;
        } url;

        struct Description {
          std::vector<Status::Url> urls;
        } description;
      } entities;

      [[er::alias("protected")]] bool is_protected;

      uint32_t followers_count;
      uint32_t friends_count;
      uint32_t listed_count;
      std::string created_at;
      uint32_t favourites_count;
      uint64_t utc_offset;
      std::string time_zone;
      bool geo_enabled;
      bool verified;
      uint32_t statuses_count;
      std::string lang;
      bool contributors_enabled;
      bool is_translator;
      bool is_translation_enabled;
      std::string profile_background_color;
      std::string profile_background_image_url;
      std::string profile_background_image_url_https;
      bool profile_background_tile;
      std::string profile_image_url;
      std::string profile_image_url_https;
      std::string profile_banner_url;
      std::string profile_link_color;
      std::string profile_sidebar_border_color;
      std::string profile_sidebar_fill_color;
      std::string profile_text_color;
      bool profile_use_background_image;
      bool default_profile;
      bool default_profile_image;
      bool following;
      bool follow_request_sent;
      bool notifications;
    } user;

    std::string geo;
    std::string coordinates;
    std::string place;
    std::vector<std::string> contributors;
    std::shared_ptr<Status> retweeted_status;
    uint32_t retweet_count;
    uint32_t favorite_count;

    struct Entity {
      struct Hashtag {
        std::string text;
        std::vector<uint32_t> indices;
      };
      std::vector<Hashtag> hashtags;

      struct Symbol {};
      std::vector<Symbol> symbols;

      std::vector<Url> urls;

      struct UserMension {
        std::string screen_name;
        std::string name;
        uint64_t id;
        std::string id_str;
        std::vector<uint32_t> indices;
      };
      std::vector<UserMension> user_mentions;

      struct Media {
        uint64_t id;
        std::string id_str;
        std::vector<uint32_t> indices;
        std::string media_url;
        std::string media_url_https;
        std::string url;
        std::string display_url;
        std::string expanded_url;
        std::string type;
        struct Sizes {
          struct Size {
            uint32_t w;
            uint32_t h;
            std::string resize;
          };

          Size medium;
          Size small;
          Size thumb;
          Size large;
        } sizes;
        uint64_t source_status_id;
        std::string source_status_id_str;
      };

      std::vector<Media> media;
    };

    std::vector<Entity> entities;

    bool favorited;
    bool retweeted;
    std::string lang;
  };
  std::vector<Status> statuses;

  struct SearchMetadata {
    float completed_in;
    uint64_t max_id;
    std::string max_id_str;
    std::string next_results;
    std::string query;
    std::string refresh_url;
    uint32_t count;
    uint64_t since_id;
    std::string since_id_str;
  } search_metadata;
};
