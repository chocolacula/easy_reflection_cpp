#include <benchmark/benchmark.h>

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <vector>

#include "er/serialization/binary.h"
#include "er/serialization/json.h"
#include "er/serialization/simd_json.h"
#include "er/serialization/yaml.h"
#include "generated/reflection.h"

// rapidjson
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

namespace {

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

void json_er_serialization(benchmark::State& state) {
  auto profile = er::serialization::json::from_string<UserProfile>(SetUp::json()).unwrap();

  for (auto _ : state) {
    auto str = er::serialization::json::to_string(&profile).unwrap();

    benchmark::DoNotOptimize(str);
  }
}
BENCHMARK(json_er_serialization);

void json_er_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto profile = er::serialization::json::from_string<UserProfile>(SetUp::json()).unwrap();

    benchmark::DoNotOptimize(profile);
  }
}
BENCHMARK(json_er_deserialization);

#ifdef USE_SIMD_JSON
void simd_json_er_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto profile = er::serialization::simd_json::from_string<UserProfile>(SetUp::json()).unwrap();

    benchmark::DoNotOptimize(profile);
  }
}
BENCHMARK(simd_json_er_deserialization);
#endif

void yaml_er_serialization(benchmark::State& state) {
  auto profile = er::serialization::yaml::from_string<UserProfile>(SetUp::yaml()).unwrap();

  std::string str;
  for (auto _ : state) {
    str = er::serialization::yaml::to_string(&profile).unwrap();

    benchmark::DoNotOptimize(str);
  }
}
BENCHMARK(yaml_er_serialization);

void yaml_er_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto profile = er::serialization::yaml::from_string<UserProfile>(SetUp::yaml()).unwrap();

    benchmark::DoNotOptimize(profile);
  }
}
BENCHMARK(yaml_er_deserialization);

void binary_er_serialization(benchmark::State& state) {
  auto profile = er::serialization::json::from_string<UserProfile>(SetUp::json()).unwrap();

  for (auto _ : state) {
    auto vec = er::serialization::binary::to_vector(&profile).unwrap();

    benchmark::DoNotOptimize(vec);
  }
}
BENCHMARK(binary_er_serialization);

void binary_er_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto profile = er::serialization::binary::from_vector<UserProfile>(SetUp::binary()).unwrap();

    benchmark::DoNotOptimize(profile);
  }
}
BENCHMARK(binary_er_deserialization);

void json_nlohmann_serialization(benchmark::State& state) {
  auto json_obj = nlohmann::json::parse(SetUp::json());

  for (auto _ : state) {
    auto str = json_obj.dump();

    benchmark::DoNotOptimize(str);
  }
}
BENCHMARK(json_nlohmann_serialization);

void json_nlohmann_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto json_obj = nlohmann::json::parse(SetUp::json());

    benchmark::DoNotOptimize(json_obj);
  }
}
BENCHMARK(json_nlohmann_deserialization);

void rapid_json_serialization(benchmark::State& state) {
  rapidjson::Document json_obj;
  json_obj.Parse(SetUp::json().data());

  for (auto _ : state) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json_obj.Accept(writer);

    auto str = std::string(buffer.GetString());

    benchmark::DoNotOptimize(str);
  }
}
BENCHMARK(rapid_json_serialization);

void rapid_json_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    rapidjson::Document json_obj;
    json_obj.Parse(SetUp::json().data());

    benchmark::DoNotOptimize(json_obj);
  }
}
BENCHMARK(rapid_json_deserialization);

void yaml_cpp_serialization(benchmark::State& state) {
  auto yaml_obj = YAML::Load(SetUp::yaml().data());

  std::string str;
  er::AppendBuf buf(&str);
  std::ostream stream(&buf);

  for (auto _ : state) {
    stream << yaml_obj;
    str.clear();
  }

  benchmark::DoNotOptimize(str);
}
BENCHMARK(yaml_cpp_serialization);

void yaml_cpp_deserialization(benchmark::State& state) {
  for (auto _ : state) {
    auto yaml_obj = YAML::Load(SetUp::yaml().data());

    benchmark::DoNotOptimize(yaml_obj);
  }
}
BENCHMARK(yaml_cpp_deserialization);

}  // namespace
