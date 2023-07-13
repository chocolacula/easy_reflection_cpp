#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "er/serialization/yaml.h"
#include "parser/parser_cpp.h"
#include "self_generated/reflection.h"
#include "tools/files.h"
#include "tools/to_filename.h"

// tclap
#include "tclap/CmdLine.h"

// inja
#include "inja/inja.hpp"
#include "inja/template.hpp"

int main(int argc, const char** argv) {
  TCLAP::CmdLine cmd("Easy Reflection code generator", ' ', VERSION);

  Files file_manager;

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

  // correct paths and find all files recursive inside input folders
  file_manager.correct_config(&conf);

#if defined(_WIN32)
  std::filesystem::path fs_output_dir(Files::from_utf8(conf.output_dir.data(), conf.output_dir.size()));
#else
  std::filesystem::path fs_output_dir(conf.output_dir);
#endif
  // check output directory
  if (!std::filesystem::exists(fs_output_dir)) {
    std::filesystem::create_directory(fs_output_dir);
  }

  auto time_1 = std::chrono::steady_clock::now();

  // parse source files
  ParserCpp parser(conf.compdb_dir, conf.output_dir);

  auto parsed = parser.parse(conf.input);

  for (auto&& [k, v] : parsed) {
    std::cout << k << ": " << v.dump() << std::endl;
  }

  auto time_2 = std::chrono::steady_clock::now();

  // load templates
  inja::Environment inja_env;
  auto template_header = inja_env.parse_template(conf.templates.header);
  auto template_object = inja_env.parse_template(conf.templates.object);
  auto template_enum = inja_env.parse_template(conf.templates.for_enum);

  // create primary files
  std::ofstream reflection_h(fs_output_dir / "reflection.h");

  reflection_h << R"(#pragma once

#include "er/reflection/reflection.h"
#include "er/types/all_types.h"

// generated:
)";

  std::ofstream reflection_cpp(fs_output_dir / "reflection.cpp");

  reflection_cpp << R"(#include "reflection.h"

// clang-format off
)";

  // clear and make 'reflected_types' directory
  auto fs_reflected_dir = fs_output_dir / "reflected_types";

  if (std::filesystem::exists(fs_reflected_dir)) {
    std::filesystem::remove_all(fs_reflected_dir);
  }
  std::filesystem::create_directory(fs_reflected_dir);

  // generate templates
  for (auto&& [object_name, json] : parsed) {
    auto file_name_utf8 = to_filename(object_name);
    json["file_name"] = file_name_utf8;

    file_name_utf8 += ".er";

#if defined(_WIN32)
    auto file_name = Files::from_utf8(file_name_utf8.data(), file_name_utf8.size());

    auto file_name_h = fs_reflected_dir / (file_name + L".h");
    auto file_name_cpp = fs_reflected_dir / (file_name + L".cpp");
#else
    auto& file_name = file_name_utf8;
    auto file_name_h = fs_reflected_dir / (file_name + ".h");
    auto file_name_cpp = fs_reflected_dir / (file_name + ".cpp");
#endif

    std::ofstream output_h(file_name_h);
    std::ofstream output_cpp(file_name_cpp);

    inja_env.render_to(output_h, template_header, json);
    output_h.close();

    if (json["kind"].get<int>() == 0) {
      inja_env.render_to(output_cpp, template_object, json);
    } else {
      inja_env.render_to(output_cpp, template_enum, json);
    }
    output_cpp.close();

    const std::string_view include_str = "#include \"reflected_types/";

    reflection_h << include_str << file_name_utf8 << ".h\"\n";
    reflection_cpp << include_str << file_name_utf8 << ".cpp\" //NOLINT\n";
  }

  reflection_cpp << "// clang-format on\n";
  reflection_cpp.close();

  reflection_h.close();

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
