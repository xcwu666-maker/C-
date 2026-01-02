#ifndef INSPECTION_DRONE_H
#define INSPECTION_DRONE_H

#include "Drone.h"

// 巡检无人机类：由无人机类派生
class InspectionDrone : public Drone {
private:
    std::string inspectionArea; // 巡检区域
    bool cameraStatus;          // 摄像头状态（true为正常）

public:
    InspectionDrone(int id, std::string model, double battery, std::string area);
    
    // 实现基类的纯虚函数
    void displayInfo() const override;                      // 多态显示信息子类无人机信息
    bool canHandleTask(const Order& order) override;        // 判断是否能胜任该订单
    void dropTask() override;                               // 放弃当前任务
    std::string serialize() const override;                 // 将对象转换为一行字符串以便保存
    void displayTaskHistory() const override;               // 显示任务历史

    // Setters
    void setInspectionArea(const std::string& area) { inspectionArea = area; }

    // Getters
    std::string getInspectionArea() const { return inspectionArea; }
};

#endif