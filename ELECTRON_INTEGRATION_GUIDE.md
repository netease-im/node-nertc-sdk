# NERTC Electron SDK 集成指南

## 概述

本指南详细说明如何将 NERTC Electron SDK 集成到您的 Electron 应用中。SDK 已预编译为 Linux x64 和 ARM64 两个架构的完整包，可直接使用。

## 快速开始

### 1. 获取 SDK 包

SDK 包已打包完成，位于：
- `out/nertc-electron-sdk-linux-x64.tar.gz` - Linux x64 版本
- `out/nertc-electron-sdk-linux-arm64.tar.gz` - Linux ARM64 版本

### 2. 解压并安装

```bash
# 解压对应架构的包
tar -xzf nertc-electron-sdk-linux-x64.tar.gz

# 在您的 Electron 项目根目录中安装
cd your-electron-project
npm install ./nertc-electron-sdk-linux-x64
```

**重要提醒**：
- ✅ 必须在 Electron 项目的**根目录**执行 `npm install`
- ✅ 确保项目根目录有 `package.json` 文件
- ✅ SDK 包已经预编译，无需额外编译步骤

### 3. 在代码中使用

#### 主进程集成 (main.js)

```javascript
const { app, BrowserWindow } = require('electron');
const NERtcSDK = require('nertc-electron-sdk-linux-x64').default;

let mainWindow;
let nertcEngine;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false
    }
  });

  mainWindow.loadFile('index.html');
}

app.whenReady().then(() => {
  createWindow();
  
  // 初始化 NERTC SDK
  nertcEngine = new NERtcSDK.NERtcEngine();
  
  const context = {
    app_key: 'your_app_key_here',
    log_dir_path: './logs'
  };
  
  const result = nertcEngine.initialize(context);
  console.log('NERTC SDK 初始化结果:', result);
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});
```

#### 渲染进程集成 (renderer.js)

```javascript
const NERtcSDK = require('nertc-electron-sdk-linux-x64').default;

// 创建引擎实例
const engine = new NERtcSDK.NERtcEngine();

// 设置事件监听
engine.on('onJoinChannel', (result) => {
  console.log('加入频道成功:', result);
});

engine.on('onUserJoined', (uid) => {
  console.log('用户加入:', uid);
});

engine.on('onUserLeft', (uid) => {
  console.log('用户离开:', uid);
});

// 加入频道
function joinChannel() {
  const channelName = 'test_channel';
  const uid = Math.floor(Math.random() * 100000);
  
  engine.joinChannel(null, channelName, uid);
}

// 离开频道
function leaveChannel() {
  engine.leaveChannel();
}
```

#### HTML 界面 (index.html)

```html
<!DOCTYPE html>
<html>
<head>
    <title>NERTC Electron Demo</title>
    <style>
        body { font-family: Arial, sans-serif; padding: 20px; }
        button { padding: 10px 20px; margin: 10px; font-size: 16px; }
        #status { margin: 20px 0; padding: 10px; background: #f0f0f0; }
    </style>
</head>
<body>
    <h1>NERTC Electron SDK Demo</h1>
    
    <div id="status">状态: 未连接</div>
    
    <button onclick="joinChannel()">加入频道</button>
    <button onclick="leaveChannel()">离开频道</button>
    <button onclick="enableVideo()">开启视频</button>
    <button onclick="disableVideo()">关闭视频</button>
    
    <div id="localVideo" style="width: 320px; height: 240px; border: 1px solid #ccc; margin: 20px 0;"></div>
    <div id="remoteVideos"></div>

    <script src="renderer.js"></script>
</body>
</html>
```

## 完整示例项目结构

```
your-electron-app/
├── package.json
├── main.js                 # 主进程
├── index.html             # 主界面
├── renderer.js            # 渲染进程脚本
├── node_modules/
│   └── nertc-electron-sdk-linux-x64/  # SDK 包
└── logs/                  # SDK 日志目录
```

## 环境要求

### 系统要求
- **Linux x64**: Ubuntu 18.04+, CentOS 7+, Debian 9+
- **Linux ARM64**: Ubuntu 20.04+, CentOS 8+, Debian 10+
- **Node.js**: >= 12.17
- **Electron**: >= 16.0.0

### 依赖库
SDK 包已包含所有必要的库文件，但系统需要以下基础库：
```bash
# Ubuntu/Debian
sudo apt-get install libc6 libstdc++6

# CentOS/RHEL
sudo yum install glibc libstdc++
```

## 故障排除

### 1. 模块加载失败

**错误**: `Cannot find module 'nertc-electron-sdk-linux-x64'`

**解决方案**:
```bash
# 检查包是否正确安装
ls -la node_modules/nertc-electron-sdk-linux-x64/

# 重新安装
npm uninstall nertc-electron-sdk-linux-x64
npm install ./nertc-electron-sdk-linux-x64
```

### 2. 库文件依赖问题

**错误**: `libnertc_sdk.so: cannot open shared object file`

**解决方案**:
```bash
# 检查库文件是否存在
ls -la node_modules/nertc-electron-sdk-linux-x64/build/Release/linux/lib/

# 设置环境变量（临时）
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/node_modules/nertc-electron-sdk-linux-x64/build/Release/linux/lib

# 或者在启动脚本中设置
```

### 3. 架构不匹配

**错误**: `wrong ELF class: ELFCLASS64` 或类似

**解决方案**:
```bash
# 检查当前系统架构
uname -m

# 使用对应架构的包
# x86_64 系统使用 nertc-electron-sdk-linux-x64
# aarch64 系统使用 nertc-electron-sdk-linux-arm64
```

## 高级配置

### 1. 自定义日志路径

```javascript
const context = {
  app_key: 'your_app_key',
  log_dir_path: '/path/to/custom/logs',
  log_level: 'info'
};
```

### 2. 视频渲染配置

```javascript
// 设置本地视频画布
const canvas = {
  window: document.getElementById('localVideo'),
  render_mode: 0,  // 0: 填充, 1: 适应
  mirror_mode: 0   // 0: 不镜像, 1: 镜像
};

engine.setupLocalVideoCanvas(canvas);
```

### 3. 音频配置

```javascript
// 启用音频
engine.enableAudio();

// 设置音频质量
engine.setAudioProfile(0, 0);  // 标准质量
```

## 生产部署

### 1. 打包 Electron 应用

```bash
# 安装 electron-builder
npm install --save-dev electron-builder

# 配置 package.json
{
  "build": {
    "appId": "com.yourcompany.yourapp",
    "productName": "Your App",
    "directories": {
      "output": "dist"
    },
    "linux": {
      "target": "AppImage",
      "category": "Network"
    }
  }
}

# 构建应用
npm run build
```

### 2. 包含 SDK 文件

确保在构建配置中包含 SDK 的所有文件：

```json
{
  "build": {
    "extraResources": [
      {
        "from": "node_modules/nertc-electron-sdk-linux-x64/build/Release/linux/lib/",
        "to": "lib/"
      }
    ]
  }
}
```

## 技术支持

### 调试信息

```javascript
// 获取 SDK 版本信息
console.log('SDK 版本:', engine.getVersion());

// 获取系统信息
console.log('系统信息:', {
  platform: process.platform,
  arch: process.arch,
  nodeVersion: process.version,
  electronVersion: process.versions.electron
});
```

### 日志分析

SDK 日志文件位于 `logs/` 目录下，包含详细的运行信息，可用于问题诊断。

## 总结

通过本指南，您可以：

1. ✅ **快速集成** NERTC Electron SDK 到您的应用中
2. ✅ **正确配置** 音频、视频和网络功能
3. ✅ **解决常见问题** 和故障排除
4. ✅ **部署生产应用** 包含完整的 SDK 功能

SDK 包已包含所有必要的文件，无需额外编译，可直接在 Electron 应用中使用。

