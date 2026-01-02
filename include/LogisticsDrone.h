#ifndef LOGISTICS_DRONE_H
#define LOGISTICS_DRONE_H

#include "Drone.h"

// 物流无人机类：由无人机类派生
class LogisticsDrone : public Drone {
private:
    double maxPayload;      // 最大载重
    int currentOrderId;     // 当前配送订单号
    double currentOrderWeight; // 记录当前配送订单的重量，用于结算电量

public:
    // 构造函数：利用初始化列表初始化基类和自身成员
    LogisticsDrone(int id, std::string model, double battery, double payload);

    // 实现基类的纯虚函数
    void displayInfo() const override;                      // 多态显示信息子类无人机信息
    bool canHandleTask(const Order& order) override;        // 判断是否能胜任该订单
    void dropTask() override;                               // 放弃当前任务
    std::string serialize() const override;                 // 将对象转换为一行字符串以便保存
    void displayTaskHistory() const override;               // 显示任务历史
   
    // 子类的函数
    void performFullMission(Order& order);
    
    // Getters
    double getMaxPayload() const { return maxPayload; };
    double getBattery() const { return battery; };
    int getCurrentOrderId() const { return currentOrderId; };
    double getCurrentOrderWeight() const { return currentOrderWeight; };

    // Setters
    void setCurrentOrderId(int orderId) { currentOrderId = orderId; };
    void setCurrentOrderWeight(double weight) { currentOrderWeight = weight; };
    void setMaxPayload(double payload) { maxPayload = payload; };
};
#endif