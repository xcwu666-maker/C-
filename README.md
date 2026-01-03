城市低空物流调度系统 (City Low-Altitude Logistics Scheduling System)

📖 项目简介
这是一个基于 C++ 开发的控制台应用程序，旨在模拟现代智慧城市中的低空无人机物流调度场景。系统采用面向对象编程 (OOP) 思想设计，能够管理不同类型的无人机机队（物流型、巡检型），处理物流订单，并实现自动化的任务调度与分配。

本项目重点展示了 C++ 的继承、多态、封装以及文件持久化操作等核心特性。

✨ 主要功能
1. 无人机管理 (Drone Management)
多机型支持：

物流无人机 (LogisticsDrone)：具备最大载重限制，专用于货物配送。

巡检无人机 (InspectionDrone)：具备特定巡检区域，专用于城市巡视（不可接单）。

全生命周期管理：支持无人机的录入、删除、属性修改（ID、型号、位置、状态等）。

状态监控：实时查看机队状态（空闲、配送中、充电中、故障等）。

充电模拟：支持对低电量无人机进行充电操作。

2. 订单管理 (Order Management)
订单录入：支持手动输入订单信息，包括寄件人、收件地、货物重量及优先级。

订单追踪：实时查询订单状态（待分配、配送中、已完成）。

历史记录：自动记录任务执行历史。

3. 智能调度系统 (Intelligent Dispatch)
自动匹配：根据订单重量与无人机载重能力，自动筛选合适的空闲物流无人机。

任务执行：模拟配送全过程，自动更新订单与无人机状态，并记录任务日志。

4. 数据持久化 (Data Persistence)
自动存档：系统退出时自动将无人机和订单数据保存至本地文件 (data/drones.txt, data/orders.txt)。

断点续传：系统启动时自动加载上次的运行数据，保证业务连续性。

🛠️ 技术栈与环境
编程语言：C++ (推荐 C++11 及以上标准)

构建工具：CMake

开发环境：Visual Studio Code / Visual Studio / CLion

操作系统：Windows (代码中包含 Windows 控制台编码设置 windows.h)

🚀 快速开始
1. 编译项目
本项目使用 CMake 进行构建。请确保您的系统已安装 CMake 和 C++ 编译器（如 GCC/MinGW 或 MSVC）。

Bash

# 1. 在项目根目录下创建构建文件夹
mkdir build
cd build

# 2. 生成构建文件
cmake ..

# 3. 编译可执行文件
cmake --build .
2. 运行程序
编译成功后，在 build 目录下找到可执行文件（如 DroneApp.exe）并运行。

Bash

./DroneApp.exe
# 或者直接在文件管理器中双击运行
注意：程序首次运行时，如果 data 目录下没有数据文件，系统会自动初始化空数据库。

📂 项目结构
Plaintext

.
├── CMakeLists.txt          # CMake 构建配置
├── include/                # 头文件目录
│   ├── Drone.h             # 无人机基类
│   ├── LogisticsDrone.h    # 物流无人机类
│   ├── InspectionDrone.h   # 巡检无人机类
│   ├── Order.h             # 订单类
│   └── SystemManager.h     # 系统管理核心类
├── src/                    # 源文件目录
│   ├── main.cpp            # 程序入口与菜单逻辑
│   ├── SystemManager.cpp   # 系统业务逻辑实现
│   ├── Drone.cpp           # 基类实现
│   ├── LogisticsDrone.cpp  # 物流无人机实现
│   ├── InspectionDrone.cpp # 巡检无人机实现
│   └── Order.cpp           # 订单逻辑实现
└── data/                   # 数据存储目录
    ├── drones.txt          # 无人机存档文件
    └── orders.txt          # 订单存档文件

📝 设计模式说明
继承 (Inheritance)：LogisticsDrone 和 InspectionDrone 均继承自抽象基类 Drone，复用了 ID、型号、电量等通用属性。

多态 (Polymorphism)：系统使用 std::vector<Drone*> 容器统一管理机队。通过虚函数 displayInfo()、canHandleTask() 等实现不同机型的差异化行为。

封装 (Encapsulation)：所有类的成员变量均为 private 或 protected，通过 public 的 Getter/Setter 接口进行访问，保证数据安全。

⚠️ 注意事项
控制台编码：main.cpp 中使用了 SetConsoleOutputCP(65001) 以支持 UTF-8 中文输出。请在 Windows 终端（如 CMD 或 PowerShell）中运行，以获得最佳显示效果。

文件路径：程序默认在 ../data/ 路径下读取和保存文件，请确保目录结构正确，或根据运行位置调整代码中的路径。
