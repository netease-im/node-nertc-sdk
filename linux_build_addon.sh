#!/usr/bin/env bash

# 一键：下载对应版本的 native SDK + 编译 Electron add-on
# 依赖：当前目录中的 pre-build.js 与 binding.gyp 已实现具体逻辑

# 若在非 bash 下执行（例如 sh），自动切换到 bash
if [ -z "${BASH_VERSION:-}" ]; then
    exec bash "$0" "$@"
fi

set -euo pipefail

echo "🧱 NERTC Electron SDK 构建开始 (in submodule)"

# 切换到脚本所在目录（即子模块根目录）
cd "$(dirname "$0")"

# 架构检测，选择对应安装脚本
ARCH="$(uname -m)"
SCRIPT="install:linux:x64"
if [ "$ARCH" = "aarch64" ] || [ "$ARCH" = "arm64" ]; then
    SCRIPT="install:linux:arm64"
fi

if [ ! -f "package.json" ]; then
    echo "❌ 未在子模块目录内（缺少 package.json）"
    exit 1
fi

if [ ! -d node_modules ]; then
    echo "📥 安装依赖: npm ci"
    npm ci --no-audit --no-fund
fi

echo "🧹 清理旧构建"
npm run clean || true

echo "⬇️ 下载安装 + 构建（$SCRIPT）"
npm run "$SCRIPT"

BUILD_DIR="build/Release"
if [ -f "$BUILD_DIR/libnertc_sdk.so" ] && [ -f "$BUILD_DIR/nertc-electron-sdk.node" ]; then
    echo "🎉 产物已就绪："
    echo " - $(pwd)/$BUILD_DIR/libnertc_sdk.so"
    echo " - $(pwd)/$BUILD_DIR/nertc-electron-sdk.node"
else
    echo "❌ 构建完成后未找到必要产物，请检查日志"
    exit 1
fi

echo "🧱 NERTC Electron SDK 构建结束"


