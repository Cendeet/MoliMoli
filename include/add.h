//
// Created by ian_n on 2026/11/8.
//

#ifndef MOLIMOLI_ADD_H
#define MOLIMOLI_ADD_H

#include <functional>
#include <string>

#include <ftxui/component/component.hpp>

namespace ui {

    // 独立封装的 Add 页：列表 + 每行的 "+" 按钮 + 弹窗输入金额
    class AddPage {
    public:
        explicit AddPage(std::function<void()> on_back);

        ftxui::Component Get() const { return root_; }

        // 根据最新 data 重建列表（进入页面时调用）
        void Refresh();

    private:
        void RebuildList();
        void OpenDialogFor(const std::string& name);

        std::function<void()> on_back_;

        ftxui::Component list_;
        ftxui::Component dialog_;
        ftxui::Component root_;

        std::string target_name_;
        std::string amount_input_ = "0";
        bool show_dialog_ = false;
    };

}  //  ui

#endif //MOLIMOLI_ADD_H
