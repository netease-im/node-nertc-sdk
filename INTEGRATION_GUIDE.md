# NERTC Electron SDK 集成指南

## 概述

本指南详细说明了如何系统性地集成 NERTC Electron SDK 到您的 Electron 应用中，包括交叉编译、产物管理和智能加载。

## 核心特性

### ✅ 已解决的问题

1. **交叉编译支持**: 支持 Linux x64 和 ARM64 双架构交叉编译
2. **智能路径解析**: 自动根据当前架构选择正确的产物路径
3. **统一产物管理**: 产物统一放在 `build/Release/` 目录下
4. **便捷集成**: 提供一键集成脚本和完整的集成流程

### 🏗️ 架构设计

```
nertc-electron-sdk/
├── build/Release/                    # 统一产物目录
│   ├── nertc-electron-sdk.node      # 当前架构的主模块
│   ├── linux/                       # 库文件和头文件
│   │   ├── lib/                     # .so 库文件
│   │   └── include/                 # 头文件
│   ├── linux-x64/                   # x64 架构产物
│   │   ├── nertc-electron-sdk.node
│   │   └── *.so
│   └── linux-arm64/                 # ARM64 架构产物
│       ├── nertc-electron-sdk.node
│       └── *.so
├── js/                              # JavaScript 封装层
│   ├── module_loader.js             # 智能模块加载器
│   ├── api/index.js                 # 主 API 封装
│   └── ...
└── scripts/
    └── integrate.js                 # 集成脚本
```

## 快速开始

### 1. 一键集成（推荐）

```bash
# 进入 SDK 目录
cd src/third_party/nertc-electron-sdk

# 运行集成脚本
npm run integrate
```

集成脚本会自动：
- 检查构建状态
- 构建缺失的产物
- 测试模块加载
- 生成集成报告

### 2. 手动构建

```bash
# 安装依赖
npm install

# 构建 Linux 双架构
npm run build:linux:x64
npm run build:linux:arm64

# 或者使用构建脚本
./linux_build_addon.sh
```

### 3. 验证集成

```bash
# 测试集成
npm run test:integration

# 手动测试模块加载
node -e "
const ModuleLoader = require('./js/module_loader');
const loader = new ModuleLoader();
console.log('调试信息:', loader.getDebugInfo());
const nertc = loader.loadAddon();
console.log('可用类:', Object.keys(nertc));
"
```

## 详细集成步骤

### 步骤 1: 环境准备

#### Linux 交叉编译环境

```bash
# 安装交叉编译工具链
sudo apt-get update
sudo apt-get install -y \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    binutils-aarch64-linux-gnu \
    libc6-dev-arm64-cross \
    libstdc++-12-dev-arm64-cross

# 安装 patchelf（可选，用于优化 RPATH）
sudo apt-get install -y patchelf
```

#### 验证工具链

```bash
# 检查交叉编译工具
aarch64-linux-gnu-gcc --version
aarch64-linux-gnu-g++ --version
```

### 步骤 2: 构建产物

#### 方式一：使用构建脚本（推荐）

```bash
# 一键构建双架构
./linux_build_addon.sh
```

#### 方式二：分步构建

```bash
# 构建 x64 架构
npm run build:linux:x64

# 构建 ARM64 架构
npm run build:linux:arm64
```

#### 方式三：使用集成脚本

```bash
# 自动检测并构建缺失的产物
npm run integrate
```

### 步骤 3: 产物验证

构建完成后，检查产物结构：

```bash
# 检查产物目录
ls -la build/Release/

# 检查架构特定产物
ls -la build/Release/linux-x64/
ls -la build/Release/linux-arm64/

# 检查库文件
ls -la build/Release/linux/lib/
```

### 步骤 4: 上层应用集成

#### 方式一：本地包安装

```bash
# 在您的 Electron 项目中
npm install ./path/to/nertc-electron-sdk
```

#### 方式二：直接复制

```bash
# 复制整个 SDK 目录
cp -r src/third_party/nertc-electron-sdk ./node_modules/nertc-electron-sdk
```

#### 方式三：符号链接（开发环境）

```bash
# 创建符号链接
ln -s /path/to/nertc-electron-sdk ./node_modules/nertc-electron-sdk
```

### 步骤 5: 代码集成

#### 主进程集成

```javascript
// main.js
const NERtcSDK = require('nertc-electron-sdk').default;

// 创建引擎实例
const engine = new NERtcSDK.NERtcEngine();

// 初始化
const context = {
  app_key: 'your_app_key',
  log_dir_path: './logs'
};

engine.initialize(context);
```

#### 渲染进程集成

```javascript
// renderer.js
const NERtcSDK = require('nertc-electron-sdk').default;

// 创建引擎实例
const engine = new NERtcSDK.NERtcEngine();

// 设置事件监听
engine.on('onJoinChannel', (result) => {
  console.log('加入频道成功:', result);
});
```

## 智能模块加载器

### 工作原理

智能模块加载器 (`js/module_loader.js`) 按以下优先级查找 native addon：

1. **标准路径**: `build/Release/nertc-electron-sdk.node`
2. **架构特定路径**: `build/Release/linux-{arch}/nertc-electron-sdk.node`
3. **向后兼容路径**: `out/linux-{arch}/Release/nertc-electron-sdk.node`

### 自动环境配置

- **Linux**: 自动设置 `LD_LIBRARY_PATH` 指向库文件目录
- **跨平台**: 支持 Windows、macOS、Linux 的架构检测
- **错误处理**: 提供详细的调试信息和错误提示

### 调试信息

```javascript
const ModuleLoader = require('nertc-electron-sdk/js/module_loader');
const loader = new ModuleLoader();
console.log(loader.getDebugInfo());
```

输出示例：
```json
{
  "platform": "linux",
  "arch": "x64",
  "archDir": "linux-x64",
  "addonPath": "/path/to/build/Release/nertc-electron-sdk.node",
  "libDir": "/path/to/build/Release/linux/lib",
  "ldLibraryPath": "/path/to/build/Release/linux/lib",
  "sdkRoot": "/path/to/nertc-electron-sdk"
}
```

## 故障排除

### 常见问题

#### 1. 模块加载失败

**错误**: `Cannot find module 'nertc-electron-sdk.node'`

**解决方案**:
```bash
# 检查产物是否存在
ls -la build/Release/nertc-electron-sdk.node

# 运行集成脚本检查
npm run integrate
```

#### 2. 库文件依赖问题

**错误**: `libnertc_sdk.so: cannot open shared object file`

**解决方案**:
```bash
# 检查库文件
ls -la build/Release/linux/lib/

# 检查 LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH

# 手动设置环境变量
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/build/Release/linux/lib
```

#### 3. 架构不匹配

**错误**: `wrong ELF class: ELFCLASS64` 或类似

**解决方案**:
```bash
# 检查文件架构
file build/Release/nertc-electron-sdk.node

# 检查当前系统架构
uname -m

# 重新构建对应架构
npm run build:linux:x64  # 或 build:linux:arm64
```

#### 4. 交叉编译工具链问题

**错误**: `aarch64-linux-gnu-g++: command not found`

**解决方案**:
```bash
# 安装交叉编译工具链
sudo apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

# 验证安装
aarch64-linux-gnu-g++ --version
```

### 调试技巧

#### 1. 启用详细日志

```bash
# 设置调试环境变量
export DEBUG=*
npm run integrate
```

#### 2. 检查构建产物

```bash
# 查看所有产物
find build/Release -name "*.node" -o -name "*.so"

# 检查文件依赖
ldd build/Release/nertc-electron-sdk.node
```

#### 3. 验证模块加载

```bash
# 使用集成脚本测试
npm run test:integration

# 手动测试
node -e "
try {
  const loader = require('./js/module_loader');
  const nertc = new loader().loadAddon();
  console.log('✅ 加载成功:', Object.keys(nertc));
} catch(e) {
  console.error('❌ 加载失败:', e.message);
}
"
```

## 最佳实践

### 1. 开发环境

- 使用符号链接集成 SDK
- 定期运行 `npm run integrate` 检查状态
- 使用 `npm run test:integration` 验证集成

### 2. 生产环境

- 使用本地包安装方式
- 确保所有目标架构的产物都已构建
- 在生产部署前运行完整的集成测试

### 3. CI/CD 集成

```yaml
# GitHub Actions 示例
- name: Build NERTC SDK
  run: |
    cd src/third_party/nertc-electron-sdk
    npm install
    npm run integrate
    
- name: Test Integration
  run: |
    cd src/third_party/nertc-electron-sdk
    npm run test:integration
```

## 版本兼容性

| Electron 版本 | Node.js 版本 | 支持状态 |
|---------------|--------------|----------|
| 16.x | 16.x | ✅ 完全支持 |
| 17.x | 17.x | ✅ 完全支持 |
| 18.x | 18.x | ✅ 完全支持 |
| 19.x | 19.x | ✅ 完全支持 |
| 20.x | 20.x | ✅ 完全支持 |

## 总结

通过本指南，您可以：

1. **系统性地构建** Linux x64 和 ARM64 双架构产物
2. **智能地加载** 根据当前架构自动选择正确的产物
3. **便捷地集成** 使用一键集成脚本快速完成集成
4. **可靠地部署** 确保生产环境的稳定运行

如有问题，请参考故障排除部分或运行 `npm run integrate` 获取详细的调试信息。

