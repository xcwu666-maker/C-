#include "../include/Drone.h"

Drone::Drone(int id, std::string model, double battery, std::string location)
    : id(id), model(model), battery(battery), location(location), status(DroneStatus::IDLE) {
}

// 充电逻辑
void Drone::charge() {
    if (status != DroneStatus::CHARGING) {
        status = DroneStatus::CHARGING;
        std::cout << "[系统] 无人机 " << id << " 开始充电。" << std::endl;
        battery += 10.0; // 每次调用充电增加10%
        std::cout << "[充电中] 无人机 " << id << " 当前电量: " << (battery > 100.0 ? 100.0 : battery) << "%" << std::endl;
        if (battery >= 100.0) {
            battery = 100.0;
            status = DroneStatus::IDLE;
            std::cout << "[系统] 无人机 " << id << " 充电完毕，已恢复空闲。" << std::endl;
        }
    }
}

// 状态转字符串，用于保存
std::string Drone::statusToString(DroneStatus s) {
    switch (s) {
        case DroneStatus::IDLE:       return "空闲";
        case DroneStatus::DELIVERING: return "配送中";
        case DroneStatus::CHARGING:   return "充电中";
        case DroneStatus::FAULTY:     return "故障";
        case DroneStatus::INSPECTING: return "巡视中";
        default:                      return "未知状态";
    }
}


void Drone::addHistory(const std::string& taskDesc) {
    TaskHistory record;
    record.description = taskDesc;
    record.timestamp = "2025-01-01"; // 暂时硬编码，后续可优化
    history.push_back(record);
}
