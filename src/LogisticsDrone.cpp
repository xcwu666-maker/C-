#include "LogisticsDrone.h"
#include <iostream>
#include <thread>
#include <chrono>

// 构造函数：利用初始化列表初始化基类和自身成员
LogisticsDrone::LogisticsDrone(int id, std::string model, double battery, double payload)
    : Drone(id, model, battery), maxPayload(payload), currentOrderId(-1), currentOrderWeight(0) {}

// 实现基类的纯虚函数
// 多态显示信息子类无人机信息
void LogisticsDrone::displayInfo() const {  
    std::cout << "[物流型] ID: " << id 
                  << " | 型号: " << model 
                  << " | 位置: " << location
                  << " | 电量: " << battery << "%"
                  << " | 最大载重: " << maxPayload << "kg" 
                  << " | 状态: " << Drone::statusToString(status) << std::endl;
}

// 判断是否能胜任该订单
bool LogisticsDrone::canHandleTask(const Order& order) {
    // 1. 物理载重检查（拿不动就是拿不动，不触发充电）
    if (order.weight > maxPayload) {
        return false;
    }

    // 2. 状态检查：如果已经在忙（配送中或充电中），直接返回 false
    if (status != DroneStatus::IDLE) {
        return false;
    }

    // 3. 电量与重量的动态判定
    // 公式：如果订单重量比 (当前电量 - 5) 还要大，说明电不够
    if (order.weight > (battery - 5.0)) {
        this->status = DroneStatus::CHARGING; // 触发自动充电
        std::cout << "[自动预警] 无人机 " << id << " 电量(" << battery 
                  << "%) 不足以配送 " << order.weight << "kg 订单，已转入充电状态。" << std::endl;
        return false;
    }

    return true;
}

// 放弃当前任务
void LogisticsDrone::dropTask() {  
    currentOrderId = -1;    // 重置订单号
    addHistory("因电量不足，放弃当前配送任务");
}

// 将对象转换为一行字符串以便保存
std::string LogisticsDrone::serialize() const { 
    // 格式：Type,ID,Model,Battery,Status,MaxPayload
    return "Logistics," + std::to_string(id) + "," + model + "," + 
           std::to_string(battery) + "," + std::to_string((int)status) + "," + 
           std::to_string(maxPayload);
}

// 显示任务历史
void LogisticsDrone::displayTaskHistory() const {
    std::cout << "\n--------------------------------------------------" << std::endl;
    std::cout << ">>> 无人机 ID: " << id << " [" << model << "] 任务流水单 <<<" << std::endl;
    
    if (history.empty()) {
        std::cout << "[空] 尚无任何飞行记录。" << std::endl;
    } else {
        for (const auto& record : history) {
            std::cout << " • [" << record.timestamp << "] " << record.description << std::endl;
        }
    }

    if (status == DroneStatus::DELIVERING) {
        std::cout << ">>> [实时状态]：正在运送订单 " << currentOrderId << "..." << std::endl;
    }
    std::cout << "--------------------------------------------------" << std::endl;
}

// 子类的函数
// 执行完整任务流程
void LogisticsDrone::performFullMission(Order& order) {
    int delaySeconds = static_cast<int>(order.weight); // 简单模拟：重量对应秒数延时
    
    // --- 第一阶段：去程配送 ---
    std::cout << "\n[动作] 无人机 " << id << " 正在前往目的地: " << order.receiverLoc 
              << " (预计耗时: " << delaySeconds << "秒)..." << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(delaySeconds)); // 模拟飞行延时

    // 状态更新
    this->battery -= (0.75 * order.weight);
    if (this->battery < 0) this->battery = 0; // 安全钳位
    this->location = order.receiverLoc;
    order.setStatus(OrderStatus::COMPLETED);
    
    std::cout << ">>> [成功] 编号为 " << order.orderId << " 的订单已被送达！" << std::endl;
    this->displayInfo(); // 显示当前电量和位置状态
    addHistory("送达订单 " + std::to_string(order.orderId) + "，到达: " + location);

    // --- 第二阶段：返航 ---
    std::cout << "\n[动作] 无人机 " << id << " 开始返回基地 (预计耗时: " << delaySeconds << "秒)..." << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(delaySeconds));

    // 状态更新
    this->battery -= (0.25 * order.weight);
    if (this->battery < 0) this->battery = 0;
    this->location = "深圳大学致信楼(基地)";
    this->status = DroneStatus::IDLE;
    this->currentOrderId = -1;

    std::cout << ">>> [系统] " << id << " 号无人机已经返回基地。" << std::endl;
    this->displayInfo();
    addHistory("返回基地，当前剩余电量: " + std::to_string(battery) + "%");
}