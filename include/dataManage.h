//
// Created by ian_n on 2026/11/1.
//

#ifndef MOLIMOLI_DATAMANAGE_H
#define MOLIMOLI_DATAMANAGE_H

#include <string>
#include <vector>

namespace data {

    struct Piggy {
        std::string name;
        float money = 0.0f;
    };

    std::vector<Piggy> GetPiggies();

    void Load();

    // 添加一个 piggy（自动处理重名），返回实际保存的名字
    std::string AddPiggy(const std::string& name);

    void RemovePiggy(const std::string& name);

    // 给指定名字的 piggy 增加 money
    void AddMoney(const std::string& name, float amount);

} // data

#endif //MOLIMOLI_DATAMANAGE_H
