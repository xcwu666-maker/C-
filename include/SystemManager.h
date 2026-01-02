#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Drone.h"
#include "Order.h"
#include "InspectionDrone.h"
#include "LogisticsDrone.h"

class SystemManager {
private:
    // 使用动态分配的数组（std::vector）管理所有无人机
    std::vector<Drone*> drones;
    // 存储所有的物流订单
    std::vector<Order> orders;

public:
    ~SystemManager(); // 析构函数，负责清理动态分配的内存

    // 功能模块
    void addDrone(Drone* d);           // 添加无人机
    bool removeDrone(int id);          // 删除无人机，返回 bool 以告知删除是否成功
    Drone* findDroneById(int id) const;// 用于显示特定无人机信息
    void displayAllDrones() const;     // 显示所有无人机状态
    void chargeDroneById(int droneId);          // 通过ID为特定无人机充电
    void displayAllOrders() const;     // 显示所有订单及其分配状态
    void queryOrderDetails(int orderId); // 查询特定订单详情
    
    // 核心业务逻辑
    void receiveOrder(const Order& o); // 接收新订单
    void dispatchOrders();             // 为订单自动分配无人机
    void startDeliveries(); // 启动所有已分配物流无人机的配送任务
    void finishOrder(int orderId);     // 完成订单
    

    void saveDronesToFile(const std::string& filename) const;   // 保存无人机状态到文件
    void loadDronesFromFile(const std::string& filename);       // 从文件加载无人机状态
    void saveOrdersToFile(const std::string& filename) const;   // 保存订单到文件
    void loadOrdersFromFile(const std::string& filename);       // 从文件加载订单

};

#endif