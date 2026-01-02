#ifndef DRONE_H
#define DRONE_H

#include <string>
#include <iostream>
#include <vector>
#include "Order.h"

// 定义无人机状态枚举
enum class DroneStatus {
    IDLE,       // 空闲
    DELIVERING, // 配送中
    CHARGING,   // 充电中
    FAULTY,     // 故障
    INSPECTING  // 巡视中
};

// 任务历史记录的简单结构（需求要求：动态内存存储历史任务）
struct TaskHistory {
    std::string description;
    std::string timestamp;
};

class Drone {
protected: // 使用protected，让子类可以访问这些属性
    int id;                     // 编号
    std::string model;          // 型号
    std::string location;       // 位置 (简化为字符串，如 "A区")
    double battery;             // 电量 (0-100)
    DroneStatus status;         // 状态
    std::vector<TaskHistory> history;   // 任务历史记录

public:
    // 构造函数
    Drone(int id, std::string model, double battery, std::string location = "深圳大学致信楼(基地)");

    // 虚析构函数
    virtual ~Drone(){
        // 自动清理 vector，不需要手动 delete
    };

    // 纯虚函数：子类必须实现
    virtual void displayInfo() const = 0;                       // 用于多态显示信息
    virtual bool canHandleTask(const Order& order) = 0;         // 判断是否能胜任任务
    virtual void dropTask() = 0;                                // 放弃当前任务
    virtual std::string serialize() const = 0;                  // 将对象转换为一行字符串以便保存
    virtual void displayTaskHistory() const = 0;                // 显示任务历史


    // 通用方法
    void charge();                                      // 充电逻辑
    bool needsCharging() const;                         // 判断是否低电量
    static std::string statusToString(DroneStatus s);   // 状态转字符串
    
    // Getters
    int getId() const { return id; }                    // 获取ID
    DroneStatus getStatus() const { return status; }    // 获取状态
    std::string getLocation() const { return location; } // 获取位置
    double getBattery() const { return battery; }       // 获取电量
    
    // Setters
    void setId(int newId) { id = newId; }               // 设置ID  
    void setModel(const std::string& m) { model = m; }  // 设置型号
    void setLocation(const std::string& loc) { location = loc; }        // 设置位置
    void setBattery(double b) { battery = (b > 100.0) ? 100.0 : b; }    // 设置电量
    void setStatus(DroneStatus newStatus){ status = newStatus; };       // 设置状态
    
    
    
    void addHistory(const std::string& taskDesc);   // 添加任务历史
};

#endif // DRONE_H