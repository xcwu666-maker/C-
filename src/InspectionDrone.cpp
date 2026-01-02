#include "InspectionDrone.h"
#include <iostream>

InspectionDrone::InspectionDrone(int id, std::string model, double battery, std::string area)
    : Drone(id, model, battery), inspectionArea(area), cameraStatus(true) {}

// 实现基类的纯虚函数，展示巡检无人机特有信息
void InspectionDrone::displayInfo() const {
    std::cout << "[巡检型] ID: " << id 
            << " | 型号: " << model 
            << " | 巡检区域: " << inspectionArea 
            << " | 摄像头: " << (cameraStatus ? "正常" : "故障") << std::endl;
}

// 实现基类的纯虚函数，判断是否能胜任该订单
bool InspectionDrone::canHandleTask(const Order& order) {
    return false; // 巡检机不参与物流配送
}

// 实现基类的纯虚函数，放弃当前任务
void InspectionDrone::dropTask() {
    addHistory("放弃当前巡检任务");
}

// 将对象转换为一行字符串以便保存
std::string InspectionDrone::serialize() const {
    // 格式：Type,ID,Model,Battery,Status,Area,Camera
    return "Inspection," + std::to_string(id) + "," + model + "," + 
           std::to_string(battery) + "," + std::to_string((int)status) + "," + 
           inspectionArea + "," + (cameraStatus ? "1" : "0");
}

// 实现任务历史显示
void InspectionDrone::displayTaskHistory() const {
    std::cout << "\n=== [巡检型] 无人机 ID: " << id << " 记录 ===" << std::endl;
    std::cout << "负责区域: " << inspectionArea << std::endl;
    if (history.empty()) {
        std::cout << "(尚无巡检日志)" << std::endl;
    } else {
        for (const auto& record : history) {
            std::cout << " [" << record.timestamp << "] " << record.description << std::endl;
        }
    }
}

