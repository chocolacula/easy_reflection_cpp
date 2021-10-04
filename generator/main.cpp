#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "config.h"
#include "er/serialization/yaml.h"
#include "file_manager.h"
#include "parser_cpp.h"
#include "self_generated/reflection.h"

// tclap
#include "tclap/CmdLine.h"

// clang
#include "clang/Tooling/CompilationDatabase.h"

// inja
#include "inja/inja.hpp"
#include "inja/template.hpp"

int main(int argc, const char** argv) {

  TCLAP::CmdLine cmd("Required Reflection code generator", ' ', VERSION);

  ::FileManager file_manager;

  TCLAP::ValueArg<std::string> c_arg("c", "config", "Explicitly specify path to the config file",  //
                                     false, file_manager.root() + "config.yaml", "path");

  TCLAP::SwitchArg p_arg("p", "perf", "Print performance report",  //
                         false);

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

  auto conf = rr::serialization::yaml::from_stream<Config>(input).unwrap();
  input.close();

  auto time_1 = std::chrono::steady_clock::now();

  std::string err;
  auto compdb = CompilationDatabase::autoDetectFromDirectory(file_manager.root() + conf.compdb_dir, err);

  if (compdb == nullptr) {
    std::cerr << "Cannot find compilation database, aborted" << std::endl;
    return -1;
  }

  ParserCpp parser(*compdb,                              //
                   file_manager.find_files(conf.input),  //
                   std::filesystem::path(file_manager.correct_path(conf.output_dir)));
  auto parsed = parser.parse();

  auto time_2 = std::chrono::steady_clock::now();

  inja::Environment env;
  auto template_object = env.parse_template(file_manager.correct_path(conf.templates.object));
  auto template_enum = env.parse_template(file_manager.correct_path(conf.templates.for_enum));

  auto the_header = file_manager.create_reflection_header(conf.output_dir);

  auto reflected_dir = file_manager.correct_path(conf.output_dir) + "/reflected_types/";

  if (std::filesystem::exists(reflected_dir)) {
    std::filesystem::remove_all(reflected_dir);
  }

  std::filesystem::create_directory(reflected_dir);

  for (auto&& item : parsed) {
    auto file_name = clear_name(item.first) + ".er.h";

    std::ofstream output;
    output.open(reflected_dir + file_name);

    const auto& json = item.second;
    if (json["id"].get<int>() == 0) {
      env.render_to(output, template_object, json);
    } else {
      env.render_to(output, template_enum, json);
    }
    the_header << "#include \"reflected_types/" << file_name << "\"\n";
    output.close();
  }

  the_header.close();

  auto time_3 = std::chrono::steady_clock::now();

  if (!p_arg.getValue()) {
    return 0;
  }

  auto analysis = std::chrono::duration<double>(time_2 - time_1).count();
  auto generation = std::chrono::duration<double>(time_3 - time_2).count();

  std::cout << rr::format("Takes for analysis {} sec, generation {}  sec, all {}  sec\n",  //
                          analysis, generation, analysis + generation);

  return 0;
}