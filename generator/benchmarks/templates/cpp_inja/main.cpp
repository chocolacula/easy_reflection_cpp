#include <chrono>
#include <fstream>
#include <iostream>

#include "inja/inja.hpp"
#include "inja/template.hpp"

using namespace inja;

int main() {
  auto start_time = std::chrono::steady_clock::now();

  Environment env;
  auto temp = env.parse_template("../enum.inja");

  std::ofstream output;
  output.open("../enum.er.h");

  nlohmann::json data;

  data["name"] = "TheEnumClass";
  data["enums"] = {"White", "Red", "Green", "Blue", "Black"};

  env.render_to(output, temp, data);

  auto end_time = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed = end_time - start_time;

  std::cout.precision(1);
  std::cout << "Execution takes " << elapsed.count() << " seconds" << std::endl;
}