"use strict";

const path = require('path');
const fs = require('fs');

/**
 * 智能模块加载器
 * 根据当前系统架构自动选择正确的 native addon 路径
 */
class ModuleLoader {
  constructor() {
    this.platform = process.platform;
    this.arch = process.arch;
    this.sdkRoot = path.resolve(__dirname, '..');
  }

  /**
   * 获取当前架构对应的目录名
   */
  getArchDir() {
    if (this.platform === 'linux') {
      if (this.arch === 'x64' || this.arch === 'x86_64') {
        return 'linux-x64';
      } else if (this.arch === 'arm64' || this.arch === 'aarch64') {
        return 'linux-arm64';
      }
    } else if (this.platform === 'win32') {
      if (this.arch === 'x64') {
        return 'win32-x64';
      } else if (this.arch === 'ia32') {
        return 'win32-ia32';
      }
    } else if (this.platform === 'darwin') {
      return 'darwin-universal';
    }
    return null;
  }

  /**
   * 获取 native addon 的路径
   */
  getAddonPath() {
    const archDir = this.getArchDir();
    
    // 优先尝试架构特定路径
    if (archDir) {
      const archPath = path.join(this.sdkRoot, 'build', 'Release', archDir, 'nertc-electron-sdk.node');
      if (fs.existsSync(archPath)) {
        return archPath;
      }
    }

    // 然后尝试旧的 out/ 目录路径（向后兼容）
    if (archDir) {
      const outPath = path.join(this.sdkRoot, 'out', archDir, 'Release', 'nertc-electron-sdk.node');
      if (fs.existsSync(outPath)) {
        return outPath;
      }
    }

    // 最后尝试标准路径（build/Release/）- 仅当架构匹配时
    const standardPath = path.join(this.sdkRoot, 'build', 'Release', 'nertc-electron-sdk.node');
    if (fs.existsSync(standardPath)) {
      // 检查架构是否匹配
      try {
        const { execSync } = require('child_process');
        const fileInfo = execSync(`file "${standardPath}"`, { encoding: 'utf8' });
        const isCurrentArch = this.isCurrentArchitecture(standardPath, fileInfo);
        if (isCurrentArch) {
          return standardPath;
        }
      } catch (error) {
        // 如果无法检查架构，仍然尝试加载
        console.warn('无法检查 .node 文件架构，尝试加载:', error.message);
        return standardPath;
      }
    }

    // 如果都找不到，返回架构特定路径（让 require 抛出错误）
    return archDir ? 
      path.join(this.sdkRoot, 'build', 'Release', archDir, 'nertc-electron-sdk.node') :
      standardPath;
  }

  /**
   * 检查文件是否匹配当前架构
   */
  isCurrentArchitecture(filePath, fileInfo) {
    if (this.platform === 'linux') {
      if (this.arch === 'x64' || this.arch === 'x86_64') {
        return fileInfo.includes('x86-64');
      } else if (this.arch === 'arm64' || this.arch === 'aarch64') {
        return fileInfo.includes('aarch64');
      }
    } else if (this.platform === 'win32') {
      if (this.arch === 'x64') {
        return fileInfo.includes('x86-64') || fileInfo.includes('AMD64');
      } else if (this.arch === 'ia32') {
        return fileInfo.includes('Intel 80386');
      }
    } else if (this.platform === 'darwin') {
      return fileInfo.includes('x86_64') || fileInfo.includes('arm64');
    }
    return false;
  }

  /**
   * 获取库文件目录
   */
  getLibDir() {
    const archDir = this.getArchDir();
    
    // 优先尝试架构特定路径
    if (archDir) {
      const archLibDir = path.join(this.sdkRoot, 'build', 'Release', archDir);
      if (fs.existsSync(archLibDir)) {
        return archLibDir;
      }
    }

    // 然后尝试旧的 out/ 目录路径
    if (archDir) {
      const outLibDir = path.join(this.sdkRoot, 'out', archDir, 'Release');
      if (fs.existsSync(outLibDir)) {
        return outLibDir;
      }
    }

    // 最后尝试标准路径
    const standardLibDir = path.join(this.sdkRoot, 'build', 'Release', 'linux', 'lib');
    if (fs.existsSync(standardLibDir)) {
      return standardLibDir;
    }

    return null;
  }

  /**
   * 加载 native addon
   */
  loadAddon() {
    const addonPath = this.getAddonPath();
    const libDir = this.getLibDir();

    // 设置 LD_LIBRARY_PATH（仅 Linux）
    if (this.platform === 'linux' && libDir) {
      const currentLdPath = process.env.LD_LIBRARY_PATH || '';
      if (!currentLdPath.includes(libDir)) {
        process.env.LD_LIBRARY_PATH = currentLdPath ? `${currentLdPath}:${libDir}` : libDir;
      }
    }

    try {
      return require(addonPath);
    } catch (error) {
      console.error(`Failed to load native addon from: ${addonPath}`);
      console.error(`Library directory: ${libDir}`);
      console.error(`LD_LIBRARY_PATH: ${process.env.LD_LIBRARY_PATH}`);
      throw error;
    }
  }

  /**
   * 获取调试信息
   */
  getDebugInfo() {
    return {
      platform: this.platform,
      arch: this.arch,
      archDir: this.getArchDir(),
      addonPath: this.getAddonPath(),
      libDir: this.getLibDir(),
      ldLibraryPath: process.env.LD_LIBRARY_PATH,
      sdkRoot: this.sdkRoot
    };
  }
}

module.exports = ModuleLoader;
