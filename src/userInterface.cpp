//
// Created by ian_n on 2026/11/1.
//

#include "userInterface.h"

#include <memory>
#include <string>
// #include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <add.h>
#include "dataManage.h"

namespace ui {
namespace {

using namespace ftxui;

// ---------- 小工具 ----------

// 文字居中的按钮样式
ButtonOption CenteredButtonOption() {
  ButtonOption option = ButtonOption::Simple();
  option.transform = [](const EntryState& s) {
    auto element = text(s.label);
    if (s.focused) {
      element |= inverted;
    }
    return element | center | border;
  };
  return option;
}

// 标题：Welcome to my app，my app 用 #E9BB4B
Element RenderTitle() {
  return hbox({
             text("Welcome to "),
             text("MoliMoli") | color(Color::RGB(0xF0, 0xD1, 0x99)),
         }) |
         bold | center;
}

// 把内容整体水平+垂直居中
Element CenterVertically(Element content) {
  return vbox({
      filler(),
      hbox({filler(), content, filler()}),
      filler(),
  });
}

// ---------- App ----------

class App {
  public:
  App() {
    data::Load();
    Build();
  }
  void Run() { screen_.Loop(root_); }

 private:
  // 页面索引：0=Home, 1=Create, 2=Manage
  int page_ = 0;

  // Input 组件实时绑定的临时变量
  std::string name_input_;

  ScreenInteractive screen_ = ScreenInteractive::Fullscreen();

  Component home_page_;
  Component create_page_;
  Component home_create_btn_;
  Component home_manage_btn_;
  Component home_add_btn_;
  Component create_input_;
  Component create_ok_;
  Component manage_content_;

  std::unique_ptr<AddPage> add_page_;
  Component add_page_comp_;

  Component tab_;
  Component root_;

  void Build() {
    BuildHomePage();
    BuildCreatePage();
    BuildManagePage();

    add_page_ = std::make_unique<AddPage>([this] { page_ = 0; });
    add_page_comp_ = add_page_->Get();

    tab_ = Container::Tab(
        {home_page_, create_page_, manage_content_, add_page_comp_}, &page_);

    root_ = Renderer(tab_, [this] {
              switch (page_) {
                case 0:
                  return RenderHome();
                case 1:
                  return RenderCreate();
                case 2:
                  return RenderManage();
                default:
                  return add_page_comp_->Render();
              }
            }) |
            CatchEvent([this](Event event) {
              if (event == Event::Character('q')) {
                screen_.Exit();
                return true;
              }
              return false;
            });
  }

  // ---------- Home ----------
  void BuildHomePage() {
    home_create_btn_ = Button(
        "Create a piggy bank",
        [this] {
          name_input_.clear();
          page_ = 1;
        },
        CenteredButtonOption());
    home_manage_btn_ = Button(
        "Manage piggy banks",
        [this] {
          RefreshManageContent();
          page_ = 2;
        },
        CenteredButtonOption());
    home_add_btn_ = Button(
        "Put money in piggy banks",
        [this] {
          add_page_->Refresh();
          page_ = 3;
        },
        CenteredButtonOption());

    home_page_ = Container::Vertical(
        {home_create_btn_, home_manage_btn_, home_add_btn_});
  }

  Element RenderHome() {
    auto main = CenterVertically(vbox({
        RenderTitle(),
        text(" "),
        text("number of piggy banks: " + std::to_string(data::GetPiggies().size())) | dim | center,
        text(" "),
        home_create_btn_->Render() | size(WIDTH, EQUAL, 25),
        text(" "),
        home_manage_btn_->Render() | size(WIDTH, EQUAL, 25),
        text(" "),
        home_add_btn_->Render() | size(WIDTH, EQUAL, 25),
    }));
    return vbox({
        main | flex,
        text("Press 'q' to quit") | dim,
    });
  }

  // ---------- Create ----------

  void BuildCreatePage() {
    create_input_ = Input(&name_input_, "Give your piggy bank a name...");
    create_ok_ = Button(
        "This One!",
        [this] {
          if (!name_input_.empty()) {
            data::AddPiggy(name_input_);
          }
          name_input_.clear();
          page_ = 0;            // 返回 Home
        },
        CenteredButtonOption());

    create_page_ = Container::Vertical({create_input_, create_ok_});
  }

  Element RenderCreate() {
    return CenterVertically(vbox({
        text("A New Piggy Bank") | bold | center,
        text(" "),
        create_input_->Render() | size(WIDTH, EQUAL, 30),
        text(" "),
        hbox({
            filler(),
            create_ok_->Render() | size(WIDTH, EQUAL, 16),
            filler(),
        }),
    }));
  }

  // ---------- Manage ----------

  void BuildManagePage() {
    manage_content_ = Container::Vertical({});
    RefreshManageContent();
  }

  void RefreshManageContent() {
    manage_content_->DetachAllChildren();

    for (const auto& piggy : data::GetPiggies()) {
      auto del_btn = Button(
          "-",
          [this, name = piggy.name] {
            data::RemovePiggy(name);
            RefreshManageContent();
          });

      auto row = Renderer(del_btn, [del_btn, name = piggy.name] {
        return hbox({
                   text(name),
                   filler(),
                   del_btn->Render(),
               }) |
               size(WIDTH, EQUAL, 30);
      });

      manage_content_->Add(row);
    }

    auto back = Button("Back", [this] { page_ = 0; }, CenteredButtonOption());
    auto styled_back = Renderer(back, [back] {
      return hbox({filler(), back -> Render() | size(WIDTH, EQUAL, 16), filler()});
    });
    manage_content_->Add(styled_back);
  }

  Element RenderManage() {
    Elements items;
    items.push_back(text("Delete") | bold | center);
    items.push_back(text(" "));
    if (data::GetPiggies().empty()) {
      items.push_back(text("(no piggy banks yet)") | dim | center);
      items.push_back(text(" "));
    }
    items.push_back(manage_content_->Render());
    return CenterVertically(vbox(std::move(items)));
  }
};

}

void Start() {
  App app;
  app.Run();
}

}  // ui
