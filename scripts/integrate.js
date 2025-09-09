#!/usr/bin/env node

/**
 * NERTC Electron SDK 集成脚本
 * 帮助上层应用快速集成 SDK
 */

const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

class SDKIntegrator {
  constructor() {
    this.sdkRoot = path.resolve(__dirname, '..');
    this.platform = process.platform;
    this.arch = process.arch;
  }

  /**
   * 检查构建状态
   */
  checkBuildStatus() {
    console.log('🔍 检查构建状态...');
    
    const status = {
      hasStandardBuild: false,
      hasArchSpecificBuild: false,
      hasOutBuild: false,
      currentArch: this.arch,
      platform: this.platform
    };

    // 检查标准构建
    const standardPath = path.join(this.sdkRoot, 'build', 'Release', 'nertc-electron-sdk.node');
    if (fs.existsSync(standardPath)) {
      status.hasStandardBuild = true;
      console.log('✅ 找到标准构建产物');
    }

    // 检查架构特定构建
    const archDir = this.getArchDir();
    if (archDir) {
      const archPath = path.join(this.sdkRoot, 'build', 'Release', archDir, 'nertc-electron-sdk.node');
      if (fs.existsSync(archPath)) {
        status.hasArchSpecificBuild = true;
        console.log(`✅ 找到架构特定构建产物: ${archDir}`);
      }
    }

    // 检查旧的 out/ 目录构建
    if (archDir) {
      const outPath = path.join(this.sdkRoot, 'out', archDir, 'Release', 'nertc-electron-sdk.node');
      if (fs.existsSync(outPath)) {
        status.hasOutBuild = true;
        console.log(`✅ 找到旧的 out/ 目录构建产物: ${archDir}`);
      }
    }

    return status;
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
   * 构建缺失的产物
   */
  async buildMissing() {
    console.log('🔨 开始构建缺失的产物...');
    
    try {
      if (this.platform === 'linux') {
        // Linux 平台构建双架构
        console.log('📦 构建 Linux 双架构产物...');
        execSync('npm run build:linux:x64', { 
          cwd: this.sdkRoot, 
          stdio: 'inherit' 
        });
        execSync('npm run build:linux:arm64', { 
          cwd: this.sdkRoot, 
          stdio: 'inherit' 
        });
      } else if (this.platform === 'win32') {
        // Windows 平台构建
        console.log('📦 构建 Windows 产物...');
        execSync('npm run build:win32:x64', { 
          cwd: this.sdkRoot, 
          stdio: 'inherit' 
        });
      } else if (this.platform === 'darwin') {
        // macOS 平台构建
        console.log('📦 构建 macOS 产物...');
        execSync('npm run build:darwin:universal', { 
          cwd: this.sdkRoot, 
          stdio: 'inherit' 
        });
      }
      
      console.log('✅ 构建完成');
    } catch (error) {
      console.error('❌ 构建失败:', error.message);
      throw error;
    }
  }

  /**
   * 测试模块加载
   */
  testModuleLoad() {
    console.log('🧪 测试模块加载...');
    
    try {
      const ModuleLoader = require(path.join(this.sdkRoot, 'js', 'module_loader'));
      const moduleLoader = new ModuleLoader();
      const debugInfo = moduleLoader.getDebugInfo();
      
      console.log('📊 调试信息:');
      console.log(JSON.stringify(debugInfo, null, 2));
      
      const nertc = moduleLoader.loadAddon();
      console.log('✅ 模块加载成功');
      console.log('📋 可用类:', Object.keys(nertc));
      
      return true;
    } catch (error) {
      console.error('❌ 模块加载失败:', error.message);
      return false;
    }
  }

  /**
   * 生成集成报告
   */
  generateReport() {
    console.log('📋 生成集成报告...');
    
    const status = this.checkBuildStatus();
    const report = {
      timestamp: new Date().toISOString(),
      platform: this.platform,
      arch: this.arch,
      buildStatus: status,
      integrationReady: status.hasStandardBuild || status.hasArchSpecificBuild || status.hasOutBuild
    };

    const reportPath = path.join(this.sdkRoot, 'integration-report.json');
    fs.writeFileSync(reportPath, JSON.stringify(report, null, 2));
    
    console.log(`📄 集成报告已生成: ${reportPath}`);
    return report;
  }

  /**
   * 执行完整集成流程
   */
  async integrate() {
    console.log('🚀 开始 NERTC Electron SDK 集成流程...');
    console.log(`📱 目标平台: ${this.platform} ${this.arch}`);
    
    try {
      // 1. 检查构建状态
      const status = this.checkBuildStatus();
      
      // 2. 如果缺少产物，尝试构建
      if (!status.hasStandardBuild && !status.hasArchSpecificBuild && !status.hasOutBuild) {
        console.log('⚠️ 未找到构建产物，开始构建...');
        await this.buildMissing();
      }
      
      // 3. 测试模块加载
      const loadSuccess = this.testModuleLoad();
      if (!loadSuccess) {
        throw new Error('模块加载测试失败');
      }
      
      // 4. 生成集成报告
      const report = this.generateReport();
      
      console.log('🎉 集成流程完成！');
      console.log('📋 集成状态:', report.integrationReady ? '✅ 就绪' : '❌ 未就绪');
      
      return report;
    } catch (error) {
      console.error('❌ 集成流程失败:', error.message);
      throw error;
    }
  }
}

// 如果直接运行此脚本
if (require.main === module) {
  const integrator = new SDKIntegrator();
  integrator.integrate().catch(process.exit);
}

module.exports = SDKIntegrator;

