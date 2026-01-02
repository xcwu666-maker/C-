#ifndef ORDER_H
#define ORDER_H

#include <string>

// 订单状态枚举
enum class OrderStatus {
    UNASSIGNED, // 待分配
    DELIVERING, // 配送中
    COMPLETED   // 已完成
};

class Order {
public:
    int orderId;               // 订单号
    std::string sender;        // 寄件人
    std::string receiverLoc;   // 收件人位置
    double weight;             // 货物重量
    int priority;              // 优先级
    OrderStatus status;        // 订单状态
    int assignedDroneId;      // 分配的无人机ID (-1表示未分配)
public:
    // 构造函数
    Order(int id, std::string s, std::string r, double w, int p)
        : orderId(id), sender(s), receiverLoc(r), weight(w), priority(p), status(OrderStatus::UNASSIGNED), assignedDroneId(-1) {}

    // 将对象转换为一行字符串以便保存
    std::string serialize() const {
        // 格式：ID,寄件人,收件人,重量,优先级,状态,分配的无人机ID
        return std::to_string(orderId) + "," + sender + "," + receiverLoc + "," +
            std::to_string(weight) + "," + std::to_string(priority) + "," +
            std::to_string((int)status) + "," + std::to_string(assignedDroneId);
    }

    // Setters
    void setStatus(OrderStatus s) { status = s; }
    void setAssignedDroneId(int id) { assignedDroneId = id; }

};

#endif