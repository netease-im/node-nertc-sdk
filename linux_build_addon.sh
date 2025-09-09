#!/usr/bin/env bash

m# 一键：下载对应版本的 native SDK + 编译 Electron add-on（Linux: 同时构建 x64 与 arm64）
# 依赖：当前目录中的 pre-build.js 与 binding.gyp 已实现具体逻辑

# 若在非 bash 下执行（例如 sh），自动切换到 bash
if [ -z "${BASH_VERSION:-}" ]; then
    exec bash "$0" "$@"
fi

set -euo pipefail

echo "🧱 NERTC Electron SDK 构建开始 (in submodule)"

# 切换到脚本所在目录（即子模块根目录）
cd "$(dirname "$0")"

# 仅 Linux 下支持双架构并行顺序构建；非 Linux 沿用原逻辑
PLATFORM="$(uname -s | tr '[:upper:]' '[:lower:]')"

if [ "$PLATFORM" != "linux" ]; then
  echo "⚠️ 非 Linux 平台，按单架构构建逻辑运行"
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
      if ! npm ci --no-audit --no-fund; then
          echo "⚠️ npm ci 失败，尝试 npm install 同步 lockfile"
          npm install --no-audit --no-fund
      fi
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
  exit 0
fi

# ===== Linux 双架构构建逻辑 =====

ARCHS=("x64" "arm64")
# 产物输出到 build/Release/linux-<arch>/
OUT_ROOT="build/Release"

if [ ! -f "package.json" ]; then
    echo "❌ 未在子模块目录内（缺少 package.json）"
    exit 1
fi

if [ ! -d node_modules ]; then
    echo "📥 安装依赖: npm ci"
    if ! npm ci --no-audit --no-fund; then
        echo "⚠️ npm ci 失败，尝试 npm install 同步 lockfile"
        npm install --no-audit --no-fund
    fi
fi

# 逐架构执行：清理 -> 安装/下载 -> 构建 -> 归档产物
for ARCH in "${ARCHS[@]}"; do
  echo "\n===== 🧩 开始处理架构: ${ARCH} ====="

  echo "🧹 清理旧构建"
  npm run clean || true

  if [ "$ARCH" = "arm64" ]; then
    SCRIPT="install:linux:arm64"
    # 参考 build_meeting_linux.sh：检测并设置交叉编译工具链
    TOOLCHAIN_PREFIX=aarch64-linux-gnu
    if ! command -v "${TOOLCHAIN_PREFIX}-g++" >/dev/null 2>&1; then
      echo "⚠️ 未找到 ${TOOLCHAIN_PREFIX}-g++，跳过 arm64 构建。"
      echo "  请先安装交叉工具链，例如：sudo apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu libc6-dev-arm64-cross libstdc++-12-dev-arm64-cross"
      echo "===== ⚠️ 跳过架构: ${ARCH} =====\n"
      continue
    fi
    export CC=${TOOLCHAIN_PREFIX}-gcc
    export CXX=${TOOLCHAIN_PREFIX}-g++
    export AR=${TOOLCHAIN_PREFIX}-ar
    export LD=${TOOLCHAIN_PREFIX}-g++
    export STRIP=${TOOLCHAIN_PREFIX}-strip
    export RANLIB=${TOOLCHAIN_PREFIX}-ranlib
  else
    SCRIPT="install:linux:x64"
    # 确保不会携带 arm64 交叉编译环境变量
    unset CC CXX AR LD STRIP RANLIB || true
  fi

  echo "⬇️ 下载安装 + 构建（$SCRIPT）"
  npm run "$SCRIPT"

  # 验证默认产物位置
  DEFAULT_DIR="build/Release"
  if [ ! -f "$DEFAULT_DIR/nertc-electron-sdk.node" ]; then
    echo "❌ 未找到 $DEFAULT_DIR/nertc-electron-sdk.node (arch=${ARCH})"
    exit 1
  fi

  # 迁移至架构专属目录（复制，避免后续 clean 清掉）
  DEST_DIR="${OUT_ROOT}/linux-${ARCH}"
  mkdir -p "$DEST_DIR"
  cp -f "$DEFAULT_DIR"/*.node "$DEST_DIR" 2>/dev/null || true
  
  # 复制所有.so文件：优先从linux/lib目录，然后从根目录
  SO_DIR="$DEFAULT_DIR/linux/lib"
  if [ -d "$SO_DIR" ]; then
    echo "📦 复制 linux/lib 下的 .so 文件"
    cp -f "$SO_DIR"/*.so "$DEST_DIR/" 2>/dev/null || true
  fi
  
  # 如果根目录下还有额外的.so文件，也复制
  if [ -f "$DEFAULT_DIR/libnertc_sdk.so" ]; then
    echo "📦 复制根目录下的 libnertc_sdk.so"
    cp -f "$DEFAULT_DIR/libnertc_sdk.so" "$DEST_DIR/" || true
  fi

  # 为交付便捷性：将 .node/.so 的 RPATH 设为 $ORIGIN，使同目录即可运行
  if command -v patchelf >/dev/null 2>&1; then
    for f in "$DEST_DIR/nertc-electron-sdk.node" "$DEST_DIR/libnertc_sdk.so" "$DEST_DIR"/libNERtc*.so; do
      [ -e "$f" ] || continue
      patchelf --set-rpath '$ORIGIN' "$f" || true
    done
  else
    echo "ℹ️ 未找到 patchelf，跳过 RPATH 设置（建议安装：sudo apt-get install -y patchelf）"
  fi

  # 如果是当前系统架构，复制到标准位置供 JS 直接使用
  CURRENT_ARCH=$(uname -m)
  if [ "$ARCH" = "x64" ] && [ "$CURRENT_ARCH" = "x86_64" ]; then
    echo "📋 复制 x64 产物到标准位置 build/Release/（当前系统架构）"
    cp -f "$DEST_DIR"/*.node "$OUT_ROOT/" 2>/dev/null || true
    cp -f "$DEST_DIR"/*.so "$OUT_ROOT/" 2>/dev/null || true
    # 保持原有的 linux/ 目录结构
    if [ -d "$DEFAULT_DIR/linux" ]; then
      cp -r "$DEFAULT_DIR/linux" "$OUT_ROOT/" 2>/dev/null || true
    fi
  elif [ "$ARCH" = "arm64" ] && [ "$CURRENT_ARCH" = "aarch64" ]; then
    echo "📋 复制 ARM64 产物到标准位置 build/Release/（当前系统架构）"
    cp -f "$DEST_DIR"/*.node "$OUT_ROOT/" 2>/dev/null || true
    cp -f "$DEST_DIR"/*.so "$OUT_ROOT/" 2>/dev/null || true
    # 保持原有的 linux/ 目录结构
    if [ -d "$DEFAULT_DIR/linux" ]; then
      cp -r "$DEFAULT_DIR/linux" "$OUT_ROOT/" 2>/dev/null || true
    fi
  else
    echo "📋 产物已保存到架构特定目录: $DEST_DIR（非当前系统架构）"
  fi

  echo "✅ 架构 ${ARCH} 产物："
  ls -l "$DEST_DIR" || true
  echo "===== ✅ 完成架构: ${ARCH} =====\n"

done

# 最终校验两个架构的产物都存在
X64_DIR="${OUT_ROOT}/linux-x64"
ARM64_DIR="${OUT_ROOT}/linux-arm64"

missing=0
[ -f "$X64_DIR/nertc-electron-sdk.node" ] || { echo "❌ 缺少 x64 .node"; missing=1; }
[ -f "$ARM64_DIR/nertc-electron-sdk.node" ] || { echo "❌ 缺少 arm64 .node"; missing=1; }

if [ "$missing" -ne 0 ]; then
  echo "❌ 双架构产物校验失败"
  exit 1
fi

echo "🎉 双架构构建完成："
echo " - $(pwd)/$X64_DIR/nertc-electron-sdk.node"
echo " - $(pwd)/$ARM64_DIR/nertc-electron-sdk.node"

echo "🧱 NERTC Electron SDK 构建结束"


