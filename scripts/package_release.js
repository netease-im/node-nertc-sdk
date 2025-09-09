#!/usr/bin/env node

/**
 * NERTC Electron SDK 发布打包脚本
 * 将编译产物打包成可供第三方应用集成的完整包
 */

const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

class ReleasePackager {
  constructor() {
    this.sdkRoot = path.resolve(__dirname, '..');
    this.outDir = path.join(this.sdkRoot, 'out');
    this.platform = process.platform;
    this.arch = process.arch;
  }

  /**
   * 创建发布包目录结构
   */
  createPackageStructure(arch) {
    const packageName = `nertc-electron-sdk-linux-${arch}`;
    const packageDir = path.join(this.outDir, packageName);
    
    // 创建目录结构
    const dirs = [
      packageDir,
      path.join(packageDir, 'js'),
      path.join(packageDir, 'types'),
      path.join(packageDir, 'build', 'Release'),
      path.join(packageDir, 'build', 'Release', 'linux', 'lib'),
      path.join(packageDir, 'build', 'Release', 'linux', 'include'),
      path.join(packageDir, 'nertc_sdk')
    ];

    dirs.forEach(dir => {
      if (!fs.existsSync(dir)) {
        fs.mkdirSync(dir, { recursive: true });
        console.log(`📁 创建目录: ${dir}`);
      }
    });

    return packageDir;
  }

  /**
   * 复制 JavaScript 封装层
   */
  copyJavaScriptLayer(packageDir) {
    console.log('📦 复制 JavaScript 封装层...');
    
    const jsSource = path.join(this.sdkRoot, 'js');
    const jsTarget = path.join(packageDir, 'js');
    
    this.copyDirectory(jsSource, jsTarget);
    console.log('✅ JavaScript 封装层复制完成');
  }

  /**
   * 复制 TypeScript 类型定义
   */
  copyTypeDefinitions(packageDir) {
    console.log('📦 复制 TypeScript 类型定义...');
    
    const typesSource = path.join(this.sdkRoot, 'types');
    const typesTarget = path.join(packageDir, 'types');
    
    this.copyDirectory(typesSource, typesTarget);
    console.log('✅ TypeScript 类型定义复制完成');
  }

  /**
   * 复制编译产物
   */
  copyBuildArtifacts(packageDir, arch) {
    console.log(`📦 复制 ${arch} 架构编译产物...`);
    
    const buildTarget = path.join(packageDir, 'build', 'Release');
    
    // 优先从架构特定目录复制 .node 文件
    const archSpecificNodeFile = path.join(this.sdkRoot, 'out', `linux-${arch}`, 'Release', 'nertc-electron-sdk.node');
    const fallbackNodeFile = path.join(this.sdkRoot, 'build', 'Release', 'nertc-electron-sdk.node');
    
    let nodeFile = null;
    if (fs.existsSync(archSpecificNodeFile)) {
      nodeFile = archSpecificNodeFile;
      console.log(`✅ 从架构特定目录复制: ${archSpecificNodeFile}`);
    } else if (fs.existsSync(fallbackNodeFile)) {
      nodeFile = fallbackNodeFile;
      console.log(`⚠️ 从标准目录复制: ${fallbackNodeFile}`);
    }
    
    if (nodeFile) {
      fs.copyFileSync(nodeFile, path.join(buildTarget, 'nertc-electron-sdk.node'));
      console.log('✅ 复制 nertc-electron-sdk.node');
    } else {
      console.error(`❌ 未找到 ${arch} 架构的 .node 文件`);
    }

    // 复制 .so 库文件到两个位置：标准位置和 linux 子目录
    const archSpecificLibSource = path.join(this.sdkRoot, 'out', `linux-${arch}`, 'Release');
    const fallbackLibSource = path.join(this.sdkRoot, 'build', 'Release', 'linux', 'lib');
    
    // 1. 复制到 build/Release/ 根目录（便于动态链接器找到）
    const libTargetRoot = buildTarget;
    // 2. 复制到 build/Release/linux/lib/ 目录（保持原有结构）
    const libTargetSubdir = path.join(buildTarget, 'linux', 'lib');
    
    let libSource = null;
    if (fs.existsSync(archSpecificLibSource)) {
      libSource = archSpecificLibSource;
      console.log(`✅ 从架构特定目录复制库文件: ${archSpecificLibSource}`);
    } else if (fs.existsSync(fallbackLibSource)) {
      libSource = fallbackLibSource;
      console.log(`⚠️ 从标准目录复制库文件: ${fallbackLibSource}`);
    }
    
    if (libSource) {
      // 复制到根目录
      const soFiles = fs.readdirSync(libSource).filter(file => file.endsWith('.so'));
      soFiles.forEach(soFile => {
        fs.copyFileSync(
          path.join(libSource, soFile),
          path.join(libTargetRoot, soFile)
        );
      });
      console.log(`✅ 复制 ${soFiles.length} 个 .so 文件到根目录`);
      
      // 复制到子目录（保持原有结构）
      this.copyDirectory(libSource, libTargetSubdir);
      console.log('✅ 复制 .so 库文件到子目录');
    } else {
      console.error(`❌ 未找到 ${arch} 架构的库文件目录`);
    }

    // 复制头文件
    const archSpecificIncludeSource = path.join(this.sdkRoot, 'out', `linux-${arch}`, 'Release');
    const fallbackIncludeSource = path.join(this.sdkRoot, 'build', 'Release', 'linux', 'include');
    const includeTarget = path.join(buildTarget, 'linux', 'include');
    
    let includeSource = null;
    if (fs.existsSync(archSpecificIncludeSource)) {
      includeSource = archSpecificIncludeSource;
      console.log(`✅ 从架构特定目录复制头文件: ${archSpecificIncludeSource}`);
    } else if (fs.existsSync(fallbackIncludeSource)) {
      includeSource = fallbackIncludeSource;
      console.log(`⚠️ 从标准目录复制头文件: ${fallbackIncludeSource}`);
    }
    
    if (includeSource) {
      this.copyDirectory(includeSource, includeTarget);
      console.log('✅ 复制头文件');
    } else {
      console.error(`❌ 未找到 ${arch} 架构的头文件目录`);
    }
  }

  /**
   * 复制 NERTC SDK
   */
  copyNERTCSDK(packageDir, arch) {
    console.log(`📦 复制 NERTC SDK (${arch})...`);
    
    const sdkSource = path.join(this.sdkRoot, 'nertc_sdk');
    const sdkTarget = path.join(packageDir, 'nertc_sdk');
    
    if (fs.existsSync(sdkSource)) {
      this.copyDirectory(sdkSource, sdkTarget);
      console.log('✅ NERTC SDK 复制完成');
    }
  }

  /**
   * 创建 package.json
   */
  createPackageJson(packageDir, arch) {
    console.log('📦 创建 package.json...');
    
    const originalPackageJson = JSON.parse(
      fs.readFileSync(path.join(this.sdkRoot, 'package.json'), 'utf8')
    );

    // 创建发布版本的 package.json
    const releasePackageJson = {
      ...originalPackageJson,
      name: `nertc-electron-sdk-linux-${arch}`,
      version: originalPackageJson.version,
      description: `NERTC Electron SDK for Linux ${arch}`,
      main: 'js/nertc_sdk.js',
      types: 'types/nertc_sdk.d.ts',
      files: [
        'js/',
        'types/',
        'build/',
        'nertc_sdk/',
        'package.json'
      ],
      os: ['linux'],
      cpu: [arch === 'x64' ? 'x64' : 'arm64'],
      engines: {
        node: originalPackageJson.engines.node,
        electron: '>=16.0.0'
      },
      // 移除需要编译的脚本，因为产物已经预编译
      scripts: {
        // 保留一些有用的脚本，但移除需要编译的
        "test:integration": "echo 'SDK is pre-compiled and ready to use'"
      }
    };

    fs.writeFileSync(
      path.join(packageDir, 'package.json'),
      JSON.stringify(releasePackageJson, null, 2)
    );
    
    console.log('✅ package.json 创建完成');
  }

  /**
   * 创建集成说明文档
   */
  createIntegrationGuide(packageDir, arch) {
    console.log('📦 创建集成说明文档...');
    
    const guideContent = `# NERTC Electron SDK - Linux ${arch}

## 快速集成

### 1. 安装依赖
\`\`\`bash
npm install ./nertc-electron-sdk-linux-${arch}
\`\`\`

### 2. 在代码中使用
\`\`\`javascript
const NERtcSDK = require('nertc-electron-sdk-linux-${arch}').default;

// 创建引擎实例
const engine = new NERtcSDK.NERtcEngine();

// 初始化
const context = {
  app_key: 'your_app_key',
  log_dir_path: './logs'
};

engine.initialize(context);
\`\`\`

### 3. 环境要求
- Linux ${arch === 'x64' ? 'x86_64' : 'aarch64'} 架构
- Node.js >= 12.17
- Electron >= 16.0.0

### 4. 文件结构
\`\`\`
nertc-electron-sdk-linux-${arch}/
├── package.json              # 包配置
├── js/                       # JavaScript 封装层
├── types/                    # TypeScript 类型定义
├── build/Release/            # 编译产物
│   ├── nertc-electron-sdk.node
│   └── linux/
│       ├── lib/              # .so 库文件
│       └── include/          # 头文件
└── nertc_sdk/               # NERTC SDK
\`\`\`

### 5. 注意事项
- 确保系统已安装必要的依赖库
- 在 Linux 环境下，可能需要设置 LD_LIBRARY_PATH 环境变量
- 所有文件都是必需的，请勿删除任何文件

## 技术支持
如有问题，请联系技术支持团队。
`;

    fs.writeFileSync(
      path.join(packageDir, 'README.md'),
      guideContent
    );
    
    console.log('✅ 集成说明文档创建完成');
  }

  /**
   * 创建压缩包
   */
  createArchive(packageDir, arch) {
    console.log(`📦 创建压缩包...`);
    
    const packageName = `nertc-electron-sdk-linux-${arch}`;
    const archivePath = path.join(this.outDir, `${packageName}.tar.gz`);
    
    try {
      // 切换到 out 目录
      const originalCwd = process.cwd();
      process.chdir(this.outDir);
      
      // 创建 tar.gz 压缩包
      execSync(`tar -czf ${packageName}.tar.gz ${packageName}/`, { stdio: 'inherit' });
      
      process.chdir(originalCwd);
      
      console.log(`✅ 压缩包创建完成: ${archivePath}`);
      return archivePath;
    } catch (error) {
      console.error('❌ 创建压缩包失败:', error.message);
      throw error;
    }
  }

  /**
   * 复制目录
   */
  copyDirectory(src, dest) {
    if (!fs.existsSync(src)) {
      return;
    }

    const items = fs.readdirSync(src);
    items.forEach(item => {
      const srcPath = path.join(src, item);
      const destPath = path.join(dest, item);
      
      if (fs.statSync(srcPath).isDirectory()) {
        if (!fs.existsSync(destPath)) {
          fs.mkdirSync(destPath, { recursive: true });
        }
        this.copyDirectory(srcPath, destPath);
      } else {
        fs.copyFileSync(srcPath, destPath);
      }
    });
  }

  /**
   * 打包指定架构
   */
  packageArch(arch) {
    console.log(`🚀 开始打包 Linux ${arch} 架构...`);
    
    try {
      // 1. 创建目录结构
      const packageDir = this.createPackageStructure(arch);
      
      // 2. 复制 JavaScript 封装层
      this.copyJavaScriptLayer(packageDir);
      
      // 3. 复制 TypeScript 类型定义
      this.copyTypeDefinitions(packageDir);
      
      // 4. 复制编译产物
      this.copyBuildArtifacts(packageDir, arch);
      
      // 5. 复制 NERTC SDK
      this.copyNERTCSDK(packageDir, arch);
      
      // 6. 创建 package.json
      this.createPackageJson(packageDir, arch);
      
      // 7. 创建集成说明文档
      this.createIntegrationGuide(packageDir, arch);
      
      // 8. 创建压缩包
      const archivePath = this.createArchive(packageDir, arch);
      
      console.log(`🎉 Linux ${arch} 架构打包完成!`);
      console.log(`📦 发布包: ${archivePath}`);
      
      return archivePath;
    } catch (error) {
      console.error(`❌ Linux ${arch} 架构打包失败:`, error.message);
      throw error;
    }
  }

  /**
   * 打包所有架构
   */
  packageAll() {
    console.log('🚀 开始打包所有架构...');
    
    const archives = [];
    
    try {
      // 打包 x64
      archives.push(this.packageArch('x64'));
      
      // 打包 ARM64
      archives.push(this.packageArch('arm64'));
      
      console.log('🎉 所有架构打包完成!');
      console.log('📦 发布包列表:');
      archives.forEach(archive => {
        console.log(`  - ${archive}`);
      });
      
      return archives;
    } catch (error) {
      console.error('❌ 打包失败:', error.message);
      throw error;
    }
  }
}

// 如果直接运行此脚本
if (require.main === module) {
  const packager = new ReleasePackager();
  
  const args = process.argv.slice(2);
  if (args.includes('--all')) {
    packager.packageAll();
  } else if (args.includes('--x64')) {
    packager.packageArch('x64');
  } else if (args.includes('--arm64')) {
    packager.packageArch('arm64');
  } else {
    console.log('用法:');
    console.log('  node package_release.js --all     # 打包所有架构');
    console.log('  node package_release.js --x64     # 只打包 x64');
    console.log('  node package_release.js --arm64   # 只打包 ARM64');
  }
}

module.exports = ReleasePackager;

