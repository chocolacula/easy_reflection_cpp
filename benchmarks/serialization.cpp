#include <benchmark/benchmark.h>

#include <fstream>
#include <nlohmann/json.hpp>
#include <string_view>
#include <vector>

#include "er/serialization/binary.h"
#include "er/serialization/json.h"
#include "er/serialization/yaml.h"
#include "generated/reflection.h"

struct SetUp {
  static std::string_view json() {
    static std::string json;

    if (json.empty()) {
      std::ifstream input;
      input.open(std::string(PROJECT_ROOT) + "/data/boba_fett.json");

      std::string str((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
      input.close();

      json = std::move(str);
    }

    return json;
  }

  static std::string_view yaml() {
    static std::string yaml;

    if (yaml.empty()) {
      std::ifstream input;
      input.open(std::string(PROJECT_ROOT) + "/data/boba_fett.yaml");

      std::string str((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
      input.close();

      yaml = std::move(str);
    }

    return yaml;
  }

  static const std::vector<uint8_t>& binary() {
    static std::vector<uint8_t> bin;

    if (bin.empty()) {
      std::ifstream input;
      input.open(std::string(PROJECT_ROOT) + "/data/boba_fett.bin");

      std::vector<uint8_t> vec((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
      input.close();

      bin = std::move(vec);
    }

    return bin;
  }
};

static void json_er_serialization(benchmark::State& state) {
  auto profile = er::serialization::json::from_string<UserProfile>(SetUp::json()).unwrap();

  for (auto _ : state) {
    auto str = er::serialization::json::to_string(&profile).unwrap();
  }
}
BENCHMARK(json_er_serialization);

static void json_er_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto profile = er::serialization::json::from_string<UserProfile>(SetUp::json()).unwrap();
  }
}
BENCHMARK(json_er_deserialization);

static void yaml_er_serialization(benchmark::State& state) {
  auto profile = er::serialization::yaml::from_string<UserProfile>(SetUp::yaml()).unwrap();

  for (auto _ : state) {
    auto str = er::serialization::yaml::to_string(&profile).unwrap();
  }
}
BENCHMARK(yaml_er_serialization);

static void yaml_er_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto profile = er::serialization::yaml::from_string<UserProfile>(SetUp::yaml()).unwrap();
  }
}
BENCHMARK(yaml_er_deserialization);

static void binary_er_serialization(benchmark::State& state) {
  auto profile = er::serialization::json::from_string<UserProfile>(SetUp::json()).unwrap();

  for (auto _ : state) {
    auto vec = er::serialization::binary::to_vector(&profile).unwrap();
  }
}
BENCHMARK(binary_er_serialization);

static void binary_er_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto profile = er::serialization::binary::from_vector<UserProfile>(SetUp::binary()).unwrap();
  }
}
BENCHMARK(binary_er_deserialization);

static void json_nlohmann_serialization(benchmark::State& state) {
  auto json_obj = nlohmann::json::parse(SetUp::json());

  for (auto _ : state) {
    auto str = json_obj.dump();
  }
}
BENCHMARK(json_nlohmann_serialization);

static void json_nlohmann_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto json_obj = nlohmann::json::parse(SetUp::json());
  }
}
BENCHMARK(json_nlohmann_deserialization);