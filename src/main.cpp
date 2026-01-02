#include <iostream>
#include <vector>
#include <memory> // 建议使用智能指针
#include <limits>
#include <windows.h>

#include "Drone.h"
#include "SystemManager.h"
#include "LogisticsDrone.h"
#include "InspectionDrone.h"

// 清理输入流的工具函数，防止输入字母导致死循环
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 无人机管理子菜单
void droneManagementMenu(SystemManager& manager) {
    while (true) {
        
        std::cout << "\n------ 无人机管理子菜单 ------" << std::endl;
        std::cout << "1. 添加无人机" << std::endl;
        std::cout << "2. 删除无人机" << std::endl;
        std::cout << "3. 查询无人机任务历史" << std::endl; 
        std::cout << "4. 修改无人机信息" << std::endl;   
        std::cout << "5. 显示当前所有无人机状态" << std::endl;
        std::cout << "6. 为无人机充电" << std::endl;
        std::cout << "0. 返回主菜单" << std::endl;
        std::cout << "请选择操作: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                int id, type;
                std::string model;
                double battery;
                std::cout << "类型(1.物流 2.巡检): "; std::cin >> type;
                std::cout << "ID: "; std::cin >> id;
                std::cout << "型号: "; std::cin >> model;
                std::cout << "初始电量: "; std::cin >> battery;

                if (type == 1) {
                    double payload;
                    std::cout << "载重(kg): "; std::cin >> payload;
                    manager.addDrone(new LogisticsDrone(id, model, battery, payload));
                } else if (type == 2) {
                    std::string area;
                    std::cout << "巡检区域: "; std::cin >> area;
                    manager.addDrone(new InspectionDrone(id, model, battery, area));
                } else {
                    std::cout << "[错误] 无效的无人机类型。" << std::endl;
                    break;
                }
                std::cout << "[系统] 添加成功。" << std::endl; 
                
                break; 
            }
            case 2: {
                int id;
                std::cout << "请输入要删除的ID: "; std::cin >> id;
                if (manager.removeDrone(id)) std::cout << "[系统] 删除成功。" << std::endl;
                else std::cout << "[错误] 未找到该无人机。" << std::endl;
                break; 
            }
            case 3: { // 查询任务历史
                int id;
                std::cout << "请输入要查询历史的无人机ID: "; std::cin >> id;
                Drone* d = manager.findDroneById(id);
                if (d) {
                    d->displayTaskHistory(); // 多态调用
                } else {
                    std::cout << "[错误] 未找到该无人机。" << std::endl;
                }
                break;
            }
            case 4: { // 修改无人机信息
                int id;
                std::cout << "请输入要修改的无人机ID: "; std::cin >> id;
                Drone* d = manager.findDroneById(id);
                if (!d) {
                    std::cout << "[错误] 无人机不存在。" << std::endl;
                    break;
                }

                while (true) {
                    std::cout << "\n--- 当前无人机信息 ---" << std::endl;
                    d->displayInfo();
                    std::cout << "\n--- 当前正在修改 ID: " << d->getId() << " ---" << std::endl;
                    std::cout << "1. 修改 ID (慎重)" << std::endl;
                    std::cout << "2. 修改 型号" << std::endl;
                    std::cout << "3. 修改 电池电量" << std::endl;
                    std::cout << "4. 修改 当前位置" << std::endl;
                    std::cout << "5. 修改 运行状态 (0:空闲, 1:配送中, 2:充电中, 3:故障)" << std::endl;

                    // 子类特有项
                    LogisticsDrone* ld = dynamic_cast<LogisticsDrone*>(d);
                    InspectionDrone* idr = dynamic_cast<InspectionDrone*>(d);
                    if (ld) std::cout << "6. 修改 最大载重 (当前: " << ld->getMaxPayload() << "kg)" << std::endl;
                    if (idr) std::cout << "6. 修改 巡检区域 (当前: " << idr->getInspectionArea() << ")" << std::endl;
                    
                    std::cout << "0. 完成并保存" << std::endl;
                    std::cout << "请选择要修改的项: ";
                    
                    int mChoice; std::cin >> mChoice;
                    if (mChoice == 0) break;

                    switch (mChoice) {
                        case 1: {
                            int newId; std::cout << "新 ID: "; std::cin >> newId;
                            d->setId(newId);
                            break;
                        }
                        case 2: {
                            std::string nm; std::cout << "新 型号: "; std::cin >> nm;
                            d->setModel(nm);
                            break;
                        }
                        case 3: {
                            double nb; std::cout << "新 电量 (0-100): "; std::cin >> nb;
                            d->setBattery(nb);
                            break;
                        }
                        case 4: {
                            std::string nl; std::cout << "新 位置: "; std::cin >> nl;
                            d->setLocation(nl);
                            break;
                        }
                        case 5: {
                            int ns; std::cout << "新 状态 (输入数字): "; std::cin >> ns;
                            d->setStatus(static_cast<DroneStatus>(ns));
                            break;
                        }
                        case 6: {
                            if (ld) {
                                double np; std::cout << "新 载重: "; std::cin >> np;
                                ld->setMaxPayload(np);
                            } else if (idr) {
                                std::string na; std::cout << "新 区域: "; std::cin >> na;
                                idr->setInspectionArea(na);
                            }
                            break;
                        }
                    }
                    std::cout << "[提示] 属性已临时修改，请继续或退出以保存。" << std::endl;
                }

                // 统一保存至文件，确保持久化
                manager.saveDronesToFile("../data/drones.txt");
                std::cout << "[系统] 该无人机的所有修改已保存至硬盘。" << std::endl;
                break;
            }
            case 5: {
                manager.displayAllDrones();
                break;
            }
            case 6: {
                int id;
                std::cout << "请输入要充电的无人机ID: "; std::cin >> id;
                manager.chargeDroneById(id);
                std::cout << "[系统] 充电操作已执行" << std::endl;
                std::cout << id << "号无人机的电量现在为"<< manager.findDroneById(id)->getBattery() << std::endl;
                break;
            }
            default:
                std::cout << "[错误] 无效选项。" << std::endl;
                break;
        }
    }
}

// 订单管理子菜单
void orderManagementMenu(SystemManager& manager) {
    while (true) {
        std::cout << "\n------ 订单管理 ------" << std::endl;
        std::cout << "1. 手动输入新订单" << std::endl;
        std::cout << "2. 显示所有订单状态" << std::endl;
        std::cout << "3. 立即触发自动调度分配" << std::endl;
        std::cout << "4. 开始配送订单" << std::endl;
        std::cout << "5. 查询特定订单详情" << std::endl;

        std::cout << "0. 返回主菜单" << std::endl;
        std::cout << "请选择操作: ";
        
        int choice;
        if (!(std::cin >> choice)) { clearInput(); continue; }
        if (choice == 0) break;

        switch (choice) {
            case 1: {
                int id, priority;
                std::string sender, receiver;
                double weight;
                std::cout << "输入订单ID: "; std::cin >> id;
                std::cout << "寄件人: "; std::cin >> sender;
                std::cout << "收件位置: "; std::cin >> receiver;
                std::cout << "重量(kg): "; std::cin >> weight;
                std::cout << "优先级(1-5): "; std::cin >> priority;

                // 创建订单对象并加入系统
                Order newOrder(id, sender, receiver, weight, priority);
                manager.receiveOrder(newOrder);
                std::cout << "[成功] 订单已录入系统。" << std::endl;
                break;
            }
            case 2:
                manager.displayAllOrders();
                break;
            case 3:
                std::cout << "[系统] 正在为待分配订单匹配最合适的无人机..." << std::endl;
                manager.dispatchOrders(); // 触发你之前实现的调度算法
                break;
            case 4:
                manager.startDeliveries(); // 启动所有已分配物流无人机进行配送
            case 5: {
                int orderId;
                std::cout << "请输入要查询的订单ID: "; 
                std::cin >> orderId;
                manager.queryOrderDetails(orderId);
                break;
            }
            default:
                std::cout << "[错误] 无效选项。" << std::endl;
        }
    }
}

int main() {
    SetConsoleOutputCP(65001); // 设置输出为 UTF-8
    SetConsoleCP(65001);       // 设置输入为 UTF-8  //解决 Windows 乱码问题
    SystemManager manager;  // 系统管理器实例

    // 启动加载全部数据
    manager.loadDronesFromFile("../data/drones.txt");
    manager.loadOrdersFromFile("../data/orders.txt");

    while (true) {
        std::cout << "\n======= 城市低空物流调度系统 =======" << std::endl;
        std::cout << "1. 无人机管理\n2. 订单管理\n0. 退出系统" << std::endl;
        std::cout << "请选择: ";
        int mainChoice;
        // 如果输入不是整数，清理并重新显示菜单
        if (!(std::cin >> mainChoice)) { clearInput(); continue; }

        if (mainChoice == 0) break;
        switch (mainChoice) {
            case 1: droneManagementMenu(manager); break;
            case 2: orderManagementMenu(manager); break;
            default: std::cout << "无效输入。" << std::endl;
        }
    }

    // 退出前保存全部数据
    manager.saveDronesToFile("../data/drones.txt");
    manager.saveOrdersToFile("../data/orders.txt");
    
    return 0;



    // // // 1. 初始化机队
    // // manager.addDrone(new LogisticsDrone(101, "大疆运载型", 25.0, 30.0)); // id101，model: 大疆运载型，电量25%，载重30kg
    // // manager.addDrone(new InspectionDrone(201, "大疆巡检型", 100, "南山区"));

    // // 2. 模拟收到新订单
    // Order order1(5001, "张三", "深圳大学", 15.0, 1); // 15kg，可以分配
    // Order order2(5002, "李四", "腾讯总部", 50.0, 2); // 50kg，超重，无法分配

    // manager.receiveOrder(order1);
    // manager.receiveOrder(order2);

    // // 3. 执行调度算法
    // std::cout << "\n--- 正在执行自动调度 ---" << std::endl;
    // manager.dispatchOrders();

    // // 4. 模拟电量消耗
    // std::cout << "\n[测试] 开始模拟电量消耗过程..." << std::endl;
    // manager.simulateStep(); // 第一步：电量下降
    // manager.simulateStep(); // 第二步：电量继续下降，直到触发重分配
    // manager.simulateStep(); // 第三步：观察结果
    
    // // 5. 保存当前无人机状态到文件
    // manager.saveDronesToFile("dataFile");
    // return 0;




    // // 使用 std::vector 存储基类指针，这是管理“动态数组”的最佳实践
    // // 这里体现了多态：容器里既可以装物流无人机，也可以装巡检无人机
    // std::vector<Drone*> droneFleet;

    // // 添加一个物流无人机 (ID: 101, 型号: Titan, 初始电量: 90, 载重: 20kg)
    // droneFleet.push_back(new LogisticsDrone(101, "Titan", 90.0, 20.0));

    // // 添加一个巡检无人机 (ID: 201, 型号: Eye, 初始电量: 85, 区域: "A1区")
    // droneFleet.push_back(new InspectionDrone(201, "Eye", 85.0, "A1区"));

    // std::cout << "--- 城市低空物流系统：当前机队状态 ---" << std::endl;
    
    // // 遍历机队，调用 displayInfo()
    // // 注意：这里会根据对象的实际类型调用对应的 displayInfo()，这就是多态！
    // for (const auto& drone : droneFleet) {
    //     drone->displayInfo();
    // }

    // // 释放内存 (实际工程建议用 std::unique_ptr，我们先手动释放以便你理解)
    // for (auto drone : droneFleet) {
    //     delete drone;
    // }

    // return 0;
}





