# TailoredSketch

> 本项目修改自 [QZRong/TailoredSketch](https://github.com/QZRong/TailoredSketch)，主要修改内容为适配Windows系统下的GCC编译环境，包括：
> - 替换了Linux特有的时间测量函数为Windows原生高精度计时器
> - 调整了CMake配置以支持MinGW-w64编译器
> - 优化了项目结构和编译流程

TailoredSketch是一个基于C++的数据流测量系统，用于高效地统计和分析网络流量。

## 环境要求

### 编译环境
- Windows 10/11
- CMake 3.25+
- MinGW-w64 14.0+（需要支持C++17）
- PowerShell

### 依赖库
- Windows SDK（用于高精度计时器）
- C++17标准库

## 项目结构

### 目录结构
```
TailoredSketch/
├── Data/                 # 数据集目录
│   └── zipf_example.dat  # 示例数据
├── Sketch/              # Sketch算法实现
│   ├── Sketch.h        # 基类
│   ├── CM_Sketch.h     # CM-Sketch实现
│   └── ...             # 其他Sketch实现
├── main.cpp            # 主程序
├── params.h            # 参数配置
├── MurmurHash.h        # 哈希函数实现
├── Choose_Ske.h        # Sketch选择器
└── CMakeLists.txt      # CMake配置文件
```

### 头文件位置
- 根目录
  - params.h：全局参数配置
  - MurmurHash.h：哈希函数实现
- Sketch/目录
  - Sketch.h：基础Sketch类
  - CM_Sketch.h：Count-Min Sketch实现
  - 其他Sketch相关头文件

## 编译和运行

### 编译步骤
1. 在PowerShell中执行以下命令：
```powershell
# 清理并创建build目录
Remove-Item -Recurse -Force build
mkdir build
cd build

# 配置CMake项目
cmake ..

# 编译项目
cmake --build .
```

### 运行程序
```powershell
# 在build目录下运行
./TailoredSketch_Code
```

## 功能说明

### 支持的Sketch类型
- StingyCM (0)
- StingyCU (1)
- StingyCM_Base (2)
- StingyCU_Base (3)
- StingyCM_Sample (4)
- StingyC (5)
- CM_Sketch (10)
- CU_Sketch (20)
- C_Sketch (30)
- CountLess (40)
- O_Tailored (50)
- Tailored (51)

### 性能测量
- Windows系统使用高精度计时器
  - QueryPerformanceCounter和QueryPerformanceFrequency
  - 精度可达纳秒级别
  - 自动记录插入和查询性能

### 输出结果
- 程序运行后会生成sheet.csv文件，包含以下指标：
  - AAE (Average Absolute Error)
  - ARE (Average Relative Error)
  - Insert Throughput
  - Query Throughput
  - CON (Conflict Rate)

## 数据集
- 默认使用zipf分布的示例数据
- 数据文件路径：`../Data/zipf_example.dat`
- 支持自定义数据集，需要按照二进制格式存储

## 参数配置
- 内存使用范围：0.2MB ~ 1MB
- 哈希函数数量(D)：默认为3
- 测试循环次数：由testcycles定义
- Sketch类型：通过test_BCM函数的第二个参数选择