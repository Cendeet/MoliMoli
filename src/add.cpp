//
// Created by ian_n on 2026/11/8.
//

#include "add.h"

#include <cstdio>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>

#include "dataManage.h"

namespace ui {
namespace {

using namespace ftxui;

ButtonOption CenteredButtonOption() {
  ButtonOption option = ButtonOption::Simple();
  option.transform = [](const EntryState& s) {
    auto element = text(s.label);
    if (s.focused) element |= inverted;
    return element | center | border;
  };
  return option;
}

bool ParseFloat(const std::string& s, float* out) {
  if (s.empty()) return false;
  try {
    size_t idx = 0;
    float v = std::stof(s, &idx);
    if (idx != s.size()) return false;
    *out = v;
    return true;
  } catch (...) {
    return false;
  }
}

Element CenterVertically(Element content) {
  return vbox({
      filler(),
      hbox({filler(), content, filler()}),
      filler(),
  });
}

}  // namespace

AddPage::AddPage(std::function<void()> on_back)
    : on_back_(std::move(on_back)) {
  list_ = Container::Vertical({});

  // ----- 弹窗内容 -----
  auto amount_field = Input(&amount_input_, "amount");
  auto ok_btn = Button(
      "OK",
      [this] {
        float v;
        if (ParseFloat(amount_input_, &v)) {
          data::AddMoney(target_name_, v);
        }
        amount_input_ = "0";
        show_dialog_ = false;
        RebuildList();
      },
      CenteredButtonOption());
  auto cancel_btn = Button(
      "Cancel", [this] { show_dialog_ = false; }, CenteredButtonOption());

  dialog_ = Renderer(
      Container::Vertical({amount_field, ok_btn, cancel_btn}),
      [this, amount_field, ok_btn, cancel_btn] {
        return vbox({
                   text("Add money to " + target_name_) | bold | center,
                   text(" "),
                   amount_field->Render() | size(WIDTH, EQUAL, 20),
                   text(" "),
                   hbox({
                       ok_btn->Render() | size(WIDTH, EQUAL, 10),
                       text(" "),
                       cancel_btn->Render() | size(WIDTH, EQUAL, 10),
                   }) | center,
               }) |
               border;
      });

  // ----- 列表本体（Add 标题 + 名字行 + Back） -----
  auto inner = Renderer(list_, [this] {
    Elements items;
    items.push_back(text("Add") | bold | center);
    items.push_back(text(" "));
    if (data::GetPiggies().empty()) {
      items.push_back(text("(no piggies yet)") | dim | center);
      items.push_back(text(" "));
    }
    items.push_back(list_->Render());
    return CenterVertically(vbox(std::move(items)));
  });

  // ----- 用 Modal 把 dialog 叠加在 inner 之上 -----
  root_ = Modal(inner, dialog_, &show_dialog_);

  RebuildList();
}

void AddPage::Refresh() {
  RebuildList();
}

void AddPage::RebuildList() {
  list_->DetachAllChildren();

  for (const auto& piggy : data::GetPiggies()) {
    auto plus_btn = Button(
        "+",
        [this, name = piggy.name] { OpenDialogFor(name); });

    auto row = Renderer(
        plus_btn, [plus_btn, name = piggy.name, money = piggy.money] {
          char buf[128];
          std::snprintf(buf, sizeof(buf), "%s  $%.2f", name.c_str(), money);
          return hbox({
                     text(buf),
                     filler(),
                     plus_btn->Render(),
                 }) |
                 size(WIDTH, EQUAL, 40);
        });

    list_->Add(row);
  }

  auto back = Button(
      "Back", [this] { on_back_(); }, CenteredButtonOption());
  auto styled_back = Renderer(back, [back] {
    return hbox({
        filler(),
        back->Render() | size(WIDTH, EQUAL, 20),
        filler(),
    });
  });
  list_->Add(styled_back);
}

void AddPage::OpenDialogFor(const std::string& name) {
  target_name_ = name;
  amount_input_ = "0";
  show_dialog_ = true;
}

}  // namespace ui