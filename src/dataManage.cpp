//
// Created by ian_n on 2026/11/1.
//

#include "dataManage.h"

#include <algorithm>
#include <fstream>
#include <set>

#include <nlohmann/json.hpp>

namespace data {
namespace {

using json = nlohmann::json;

constexpr char kFileName[] = "data.json";

std::vector<Piggy> g_piggies;

std::string UniqueName(const std::string& base) {
  std::set<std::string> existing;
  for (const auto& p : g_piggies) existing.insert(p.name);
  if (!existing.count(base)) return base;
  for (int i = 1;; ++i) {
    std::string candidate = base + "(" + std::to_string(i) + ")";
    if (!existing.count(candidate)) return candidate;
  }
}

void Save() {
  json arr = json::array();
  for (const auto& p : g_piggies) {
    arr.push_back({{"name", p.name}, {"money", p.money}});
  }
  json j;
  j["piggies"] = arr;
  std::ofstream out(kFileName);
  out << j.dump(2);
}

}  // namespace

std::vector<Piggy> GetPiggies() {
  return g_piggies;
}

void Load() {
  g_piggies.clear();
  std::ifstream in(kFileName);
  if (!in) return;
  try {
    json j;
    in >> j;
    if (j.contains("piggies") && j["piggies"].is_array()) {
      for (const auto& item : j["piggies"]) {
        if (!item.is_object()) continue;
        Piggy p;
        p.name = item.value("name", std::string{});
        p.money = item.value("money", 0.0f);
        if (!p.name.empty()) g_piggies.push_back(p);
      }
    } else if (j.contains("names") && j["names"].is_array()) {
      // 兼容旧格式：只有名字，money 默认 0
      for (const auto& n : j["names"]) {
        if (n.is_string()) {
          Piggy p;
          p.name = n.get<std::string>();
          g_piggies.push_back(p);
        }
      }
    }
  } catch (const std::exception&) {
    g_piggies.clear();
  }
}

std::string AddPiggy(const std::string& name) {
  std::string final_name = UniqueName(name);
  Piggy p;
  p.name = final_name;
  g_piggies.push_back(p);
  Save();
  return final_name;
}

void RemovePiggy(const std::string& name) {
  auto it = std::find_if(g_piggies.begin(), g_piggies.end(),
                         [&](const Piggy& p) { return p.name == name; });
  if (it != g_piggies.end()) {
    g_piggies.erase(it);
    Save();
  }
}

void AddMoney(const std::string& name, float amount) {
  for (auto& p : g_piggies) {
    if (p.name == name) {
      p.money += amount;
      Save();
      return;
    }
  }
}

}  // data