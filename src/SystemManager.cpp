#include "SystemManager.h"

// 析构函数，负责清理动态分配的内存，防止内存泄漏
SystemManager::~SystemManager() {
    for (auto d : drones) {
        delete d;
    }
}




// 添加无人机
void SystemManager::addDrone(Drone* d) {
    drones.push_back(d);
}

// 删除无人机：必须先 delete 内存，再从 vector 中移除
bool SystemManager::removeDrone(int id) {
    for (auto it = drones.begin(); it != drones.end(); ++it) {
        if ((*it)->getId() == id) {
            delete *it;        // 释放堆内存，这是工程师的必修课
            drones.erase(it);  // 从容器中移除
            return true;
        }
    }
    return false;
}

// 根据ID查找无人机指针
Drone* SystemManager::findDroneById(int id) const {
    for (auto d : drones) {
        if (d->getId() == id) return d;
    }
    return nullptr;
}

// 实现显示所有无人机状态的功能
void SystemManager::displayAllDrones() const {
    std::cout << "\n------ 当前所有无人机状态快照 ------" << std::endl;
    if (drones.empty()) {
        std::cout << "[提示] 机队目前为空。" << std::endl;
        return;
    }

    for (const auto& drone : drones) {
        // 利用多态调用不同类型无人机的 displayInfo
        drone->displayInfo(); 
    }
    std::cout << "------------------------------------" << std::endl;
}

// 通过ID为特定无人机充电
void SystemManager::chargeDroneById(int droneId) {
    Drone* d = findDroneById(droneId); 
    if (d) {
        // 逻辑检查：正在派送的无人机不能强制充电
        if (d->getStatus() == DroneStatus::DELIVERING) {
            std::cout << "[警告] 无人机 " << droneId << " 正在派送订单，无法充电！" << std::endl;
            return;
        }
        d->charge(); // 调用 Drone 类中的充电逻辑
    } else {
        std::cout << "[错误] 未找到 ID 为 " << droneId << " 的无人机。" << std::endl;
    }
}

// 实现显示所有订单及其分配状态的功能
void SystemManager::displayAllOrders() const {
    std::cout << "\n------ 订单列表快照 ------" << std::endl;
    if (orders.empty()) {
        std::cout << "[提示] 目前没有订单。" << std::endl;
        return;
    }
    for (const auto& o : orders) {
        std::string statusStr;
        switch (o.status) {
            case OrderStatus::UNASSIGNED: statusStr = "待分配"; break;
            case OrderStatus::DELIVERING: statusStr = "配送中"; break;
            case OrderStatus::COMPLETED:  statusStr = "已完成"; break;
        }
        std::cout << "ID: " << o.orderId 
                  << " | 寄件人: " << o.sender 
                  << " | 收件位置: " << o.receiverLoc
                  << " | 重量: " << o.weight << "kg"
                  << " | 优先级: " << o.priority 
                  << " | 状态: " << statusStr << std::endl;
    }
    std::cout << "--------------------------" << std::endl;
}

// 查询特定订单详情
void SystemManager::queryOrderDetails(int orderId) {
    Order* targetOrder = nullptr;
    for (auto& o : orders) {
        if (o.orderId == orderId) {
            targetOrder = &o;
            break;
        }
    }

    if (!targetOrder) {
        std::cout << "[错误] 未找到编号为 " << orderId << " 的订单。" << std::endl;
        return;
    }

    // 1. 显示订单基本状态
    std::cout << "\n=== 订单详情 (ID: " << orderId << ") ===" << std::endl;
    std::cout << "当前状态: " << (targetOrder->status == OrderStatus::UNASSIGNED ? "待分配" : 
                                  (targetOrder->status == OrderStatus::DELIVERING ? "派送中" : "已送达")) << std::endl;
    std::cout << "重量: " << targetOrder->weight << "kg | 目的地: " << targetOrder->receiverLoc << std::endl;

    // 2. 如果是正在配送或已送达，显示无人机信息
    if (targetOrder->status == OrderStatus::DELIVERING || targetOrder->status == OrderStatus::COMPLETED) {
        if (targetOrder->assignedDroneId != -1) {
            std::cout << ">>> 分配的无人机信息如下:" << std::endl;
            Drone* d = findDroneById(targetOrder->assignedDroneId);
            if (d) {
                d->displayInfo(); // 调用你之前写好的显示函数
            } else {
                std::cout << "[提示] 该无人机可能已被移除出系统。" << std::endl;
            }
        }
    }
}



    
// 接收新订单
void SystemManager::receiveOrder(const Order& o) {
    orders.push_back(o);
    std::cout << "[系统] 收到新订单 ID: " << o.orderId << "，重量: " << o.weight << "kg" << std::endl;
}

// 为订单自动分配无人机
void SystemManager::dispatchOrders() {
    std::cout << "\n=== 正在进行自动订单分配 ===" << std::endl;

    for (auto& order : orders) {
        if (order.status == OrderStatus::UNASSIGNED) {
            for (auto& drone : drones) {
                if (drone->canHandleTask(order)) {
                    LogisticsDrone* ld = dynamic_cast<LogisticsDrone*>(drone);
                    if (ld) {
                        // 1. 建立绑定关系
                        ld->setStatus(DroneStatus::DELIVERING);
                        ld->setCurrentOrderId(order.orderId);
                        ld->setCurrentOrderWeight(order.weight); // 锁定重量

                        order.status = OrderStatus::DELIVERING;
                        order.assignedDroneId = drone->getId();
                       
                        // 2. 立即生成“接单历史”
                        std::string desc = "接单成功：开始配送订单 ID: " + std::to_string(order.orderId) + 
                                           " (重量: " + std::to_string(order.weight) + "kg, 目的地: " + order.receiverLoc + ")";
                        ld->addHistory(desc);

                        std::cout << "[调度] 匹配成功：无人机 " << ld->getId() << " 正在前往配送订单 " << order.orderId << std::endl;
                        break; // 订单已分配，跳出无人机循环
                    }
                }
            }
        }
    }
}

// 启动所有已分配物流无人机的配送任务
void SystemManager::startDeliveries() {
    std::cout << "\n=== 开始执行已分配的配送任务 ===" << std::endl;
    int taskCount = 0;

    for (auto& drone : drones) {
        // 尝试转换为物流无人机指针
        LogisticsDrone* ld = dynamic_cast<LogisticsDrone*>(drone);
        
        // 如果是物流机且处于配送状态
        if (ld && ld->getStatus() == DroneStatus::DELIVERING) {
            int orderId = ld->getCurrentOrderId();
            // 找到对应的订单引用
            for (auto& o : orders) {
                if (o.orderId == orderId) {
                    // 调用你定义的完整配送函数（包含 5*重量 秒的延时）
                    ld->performFullMission(o); 
                    taskCount++;
                    break;
                }
            }
        }
    }

    if (taskCount > 0) {
        std::cout << "\n[系统] 本轮所有配送任务已执行完毕，相关状态已更新。" << std::endl;
        // 任务完成后统一保存一次存档
        saveDronesToFile("../data/drones.txt");
        saveOrdersToFile("../data/orders.txt");
    } else {
        std::cout << "[提示] 当前没有待执行的配送任务。" << std::endl;
    }
}

// 完成订单
void SystemManager::finishOrder(int droneId) {
    Drone* drone = findDroneById(droneId);
    if (!drone) return;

    LogisticsDrone* ld = dynamic_cast<LogisticsDrone*>(drone);
    if (ld && ld->getStatus() == DroneStatus::DELIVERING) {
        // 1. 找到对应的订单引用
        for (auto& o : orders) {
            if (o.orderId == ld->getCurrentOrderId()) {
                // 2. 调用封装好的完整物理任务函数
                ld->performFullMission(o); 
                break;
            }
        }
        // 3. 执行完长时任务后，保存数据
        saveDronesToFile("../data/drones.txt");
        saveOrdersToFile("../data/orders.txt");
    } else {
        std::cout << "[提示] 该无人机未处于配送任务中。" << std::endl;
    }
}



// 保存无人机状态到文件
void SystemManager::saveDronesToFile(const std::string& filename) const {
    std::ofstream outFile(filename); // 打开文件准备写入

    if (!outFile.is_open()) {
        std::cerr << "[错误] 无法打开文件进行保存: " << filename << std::endl;
        return;
    }

    // 写入 UTF-8 BOM 标记，防止 Windows 记事本乱码
    outFile << (char)0xEF << (char)0xBB << (char)0xBF;

    for (const auto& drone : drones) {
        outFile << drone->serialize() << "\n"; // 写入每一台无人机的序列化字符串
    }

    outFile.close(); // 记得关闭文件
    std::cout << "[系统] 成功将 " << drones.size() << " 台无人机数据保存至 " << filename << std::endl;
}

// 从文件加载无人机状态
void SystemManager::loadDronesFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cout << "[提示] 未找到无人机存档文件，将初始化空系统。" << std::endl;
        return;
    }

    // 跳过可能存在的 BOM 标记（前3个字节）
    char bom[3];
    if (inFile.peek() == 0xEF) {
        inFile.read(bom, 3);
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue;

        std::stringstream ss(line);
        std::string type, idStr, model, batteryStr, statusStr, extra;

        // 按照序列化时的顺序提取：Type,ID,Model,Battery,Status,Extra
        std::getline(ss, type, ',');
        std::getline(ss, idStr, ',');
        std::getline(ss, model, ',');
        std::getline(ss, batteryStr, ',');
        std::getline(ss, statusStr, ',');
        std::getline(ss, extra, ',');

        // 2. 健壮性检查：确保核心数字字段不为空
        if (idStr.empty() || batteryStr.empty() || statusStr.empty()) {
            continue; 
        }

        // 转换数据类型
        try {
            int id = std::stoi(idStr);
            double battery = std::stod(batteryStr);
            int statusInt = std::stoi(statusStr);

            Drone* newDrone = nullptr;

            if (type == "Logistics") {
                double payload = std::stod(extra);
                newDrone = new LogisticsDrone(id, model, battery, payload);
            } else if (type == "Inspection") {
                newDrone = new InspectionDrone(id, model, battery, extra);
            }

            if (newDrone) {
                // 恢复保存时的状态
                newDrone->setStatus(static_cast<DroneStatus>(statusInt));
                drones.push_back(newDrone);
            }
        } catch (const std::exception& e) {
            // 3. 异常捕获：即使这一行格式坏了，也不要让整个程序自杀
            std::cerr << "[数据跳过] 发现无效行: " << line << " (原因: " << e.what() << ")" << std::endl;
        }
    }
    inFile.close();
    std::cout << "[系统] 成功从文件载入 " << drones.size() << " 台无人机数据。" << std::endl;
}

// 保存订单到文件
void SystemManager::saveOrdersToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return;

    // 写入 UTF-8 BOM 标记，防止 Windows 记事本乱码
    outFile << (char)0xEF << (char)0xBB << (char)0xBF;

    for (const auto& o : orders) {
        outFile << o.serialize() << "\n";
    }
    outFile.close();
}

// 从文件加载订单
void SystemManager::loadOrdersFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cout << "[提示] 未找到订单存档文件，将初始化空订单列表。" << std::endl;
        return;
    }
    // 跳过可能存在的 BOM 标记（前3个字节）
    char bom[3];
    if (inFile.peek() == 0xEF) {
        inFile.read(bom, 3);
    }

    std::string line;
    orders.clear(); // 清空当前内存中的订单，防止重复
    
    while (std::getline(inFile, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        std::stringstream ss(line);
        std::string idStr, s, r, wStr, pStr, stStr;

        // 按逗号解析字段
        std::getline(ss, idStr, ',');
        std::getline(ss, s, ',');
        std::getline(ss, r, ',');
        std::getline(ss, wStr, ',');
        std::getline(ss, pStr, ',');
        std::getline(ss, stStr, ',');

        // 3. 核心字段空值检查：防止 stoi/stod 崩溃
        if (idStr.empty() || wStr.empty() || pStr.empty() || stStr.empty()) {
            continue;
        }

        try {
            // 4. 异常保护转换逻辑
            int id = std::stoi(idStr);
            double weight = std::stod(wStr);
            int priority = std::stoi(pStr);
            int statusInt = std::stoi(stStr);

            Order o(id, s, r, weight, priority);
            o.status = static_cast<OrderStatus>(statusInt);
            orders.push_back(o);
        } catch (const std::exception& e) {
            // 5. 捕获转换错误，确保一行坏数据不会杀死整个程序
            std::cerr << "[数据跳过] 订单解析异常: " << e.what() << " | 内容: " << line << std::endl;
        }
    }

    inFile.close();
    std::cout << "[系统] 成功载入 " << orders.size() << " 条订单数据。" << std::endl;
}

