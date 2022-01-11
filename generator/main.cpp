#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "er/serialization/yaml.h"
#include "file_manager.h"
#include "parser_cpp.h"
#include "self_generated/reflection.h"
#include "to_snake_case.h"

// tclap
#include "tclap/CmdLine.h"

// clang
#include "clang/Tooling/CompilationDatabase.h"

// inja
#include "inja/inja.hpp"
#include "inja/template.hpp"

int main(int argc, const char** argv) {

  TCLAP::CmdLine cmd("Easy Reflection code generator", ' ', VERSION);

  ::FileManager file_manager;

  TCLAP::ValueArg<std::string> c_arg("c", "config", "Explicitly specify path to the config file",  //
                                     false, file_manager.root() + "config.yaml", "path");

  TCLAP::SwitchArg p_arg("p", "perf", "Print performance report", false);

  cmd.add(c_arg);
  cmd.add(p_arg);
  cmd.parse(argc, argv);

  std::string c_path = c_arg.getValue();

  std::ifstream input;
  input.open(c_path);

  if (!input.is_open()) {
    std::cerr << "Cannot find the config file, aborted" << std::endl;
    return -1;
  }

  auto conf = er::serialization::yaml::from_stream<Config>(input).unwrap();
  input.close();

  auto time_1 = std::chrono::steady_clock::now();

  std::string err;
  auto compdb = CompilationDatabase::autoDetectFromDirectory(file_manager.root() + conf.compdb_dir, err);

  if (compdb == nullptr) {
    std::cerr << "Cannot find compilation database, aborted" << std::endl;
    return -1;
  }

  // correct pathes and find all files recursive inside input folders
  file_manager.correct_config(&conf);

  // check output directory
  if (!std::filesystem::exists(conf.output_dir)) {
    std::filesystem::create_directory(conf.output_dir);
  }

  // parse source files
  ParserCpp parser(*compdb,     //
                   conf.input,  //
                   std::filesystem::path(conf.output_dir));
  auto parsed = parser.parse();

  auto time_2 = std::chrono::steady_clock::now();

  // load templates
  inja::Environment inja_env;
  auto template_header = inja_env.parse_template(conf.templates.header);
  auto template_object = inja_env.parse_template(conf.templates.object);
  auto template_enum = inja_env.parse_template(conf.templates.for_enum);

  // create main files
  std::ofstream ref_h;
  ref_h.open(conf.output_dir + "/reflection.h");

  ref_h << R"(#pragma once

#include "er/reflection/reflection.h"
#include "er/types/all_types.h"

// generated:
)";

  std::ofstream ref_cpp;
  ref_cpp.open(conf.output_dir + "/reflection.cpp");

  ref_cpp << R"(#include "reflection.h"

// clang-format off
)";

  // clear and make 'reflected_types' directory
  auto reflected_dir = conf.output_dir + "/reflected_types/";

  if (std::filesystem::exists(reflected_dir)) {
    std::filesystem::remove_all(reflected_dir);
  }
  std::filesystem::create_directory(reflected_dir);

  // generate templates
  for (auto&& item : parsed) {
    auto file_name = to_snake_case(item.first);

    auto& json = item.second;
    json["file_name"] = file_name;

    file_name += ".er";

    std::ofstream output_h;
    output_h.open(reflected_dir + file_name + ".h");

    std::ofstream output_cpp;
    output_cpp.open(reflected_dir + file_name + ".cpp");

    inja_env.render_to(output_h, template_header, json);
    output_h.close();

    if (json["id"].get<int>() == 0) {
      inja_env.render_to(output_cpp, template_object, json);
    } else {
      inja_env.render_to(output_cpp, template_enum, json);
    }
    output_cpp.close();

    const std::string_view include_str = "#include \"reflected_types/";

    ref_h << include_str << file_name << ".h\"\n";
    ref_cpp << include_str << file_name << ".cpp\" //NOLINT\n";
  }

  ref_cpp << "// clang-format on\n";
  ref_cpp.close();

  ref_h.close();

  auto time_3 = std::chrono::steady_clock::now();

  if (!p_arg.getValue()) {
    return 0;
  }

  auto analysis = std::chrono::duration<double>(time_2 - time_1).count();
  auto generation = std::chrono::duration_cast<std::chrono::milliseconds>(time_3 - time_2).count();
  auto all = std::chrono::duration<double>(time_3 - time_1).count();

  std::cout << er::format("Takes for analysis {} sec, generation {} ms, all {} sec\n",  //
                          analysis, generation, all);

  return 0;
}