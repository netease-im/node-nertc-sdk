#! /usr/bin/env node
const shell = require('shelljs');
const fetch = require('node-fetch');
const path = require('path');
const tar = require('tar');
const glob = require('glob');
const download = require('download');
const fse = require('fs-extra');
const {Command} = require('commander');
const program = new Command();
const package_json = require(process.cwd() + '/package.json');
if (!package_json.node_pre_build) {
  package_json.node_pre_build = {};
}
const name_addon = package_json.node_pre_build['name'] ?
  package_json.node_pre_build['name'] :
  (package_json.node_pre_build['name-addon'] ? package_json.node_pre_build['name-addon'] : package_json.name);
const name_sdk = package_json.node_pre_build['name'] ?
  package_json.node_pre_build['name'] :
  (package_json.node_pre_build['name-sdk'] ? package_json.node_pre_build['name-sdk'] : package_json.name);
const sdk_path = path.join(process.cwd(), package_json.node_pre_build['sdk-dir'] ? package_json.node_pre_build['sdk-dir'] : 'sdk');
const temp_path = path.join(process.cwd(), 'temporary');
const sdk_group = package_json.node_pre_build['sdk-group'];
const addon_group = package_json.node_pre_build['addon-group'];
const binary_dir = package_json.node_pre_build['binary-dir'] ? package_json.node_pre_build['binary-dir'] : 'build/Release';
const package_dir = package_json.node_pre_build['package-dir'] ? package_json.node_pre_build['package-dir'] : 'packages';
const platform = process.platform;
if (!sdk_group || !addon_group) {
  console.error('[node_pre_build] please specify \'sdk-group\' and \'addon-group\' in field \'node_pre_build\'.');
}
// check if project has electron dependency
const node_modules = require('node_modules-path');
const { arch } = require('os');

let is_electron = false;
let electron_version;
let electron_path = node_modules('electron', process.cwd().split(path.sep));
if (!electron_path && process.env.INIT_CWD) {
  electron_path = node_modules('electron', process.env.INIT_CWD.split(path.sep));
}
if (electron_path) {
  is_electron = true;
  electron_version = require(path.join(electron_path, 'electron', 'package.json')).version;
}
console.log(`[node_pre_build] is_electron: ${is_electron}, electron_version: ${electron_version}`)

function removeNativeSdk() {
  if(fse.pathExistsSync(sdk_path)){
    fse.rmdirSync(sdk_path, { recursive: true })
    console.log(`[node_pre_build] delecte NertcSdk end`)
  }
}

function copySDKToBinaryDir() {
  const temp = glob.sync('/**/+(*.dll|*.framework|*.xcframework|*.dylib|*.so|*.node)', {
    root: sdk_path
  })
  const files = []
  temp.forEach((filepath) => {
    console.log('pre copySync file:' + path.basename(filepath))
    if(!filepath.includes('dSYM')){
      files.push(filepath)
    }
  });
  if (!fse.pathExistsSync(path.join(process.cwd(), binary_dir))) {
    fse.mkdirSync(path.join(process.cwd(), binary_dir), {recursive: true});
  }
  files.forEach((filepath) => {
    console.log('after copySync file:' + path.basename(filepath))
    fse.copySync(filepath, path.join(process.cwd(), binary_dir, path.basename(filepath)))
  });
  console.log(`[node_pre_build] copySDKToBinaryDir end`)
}

function build(arch) {
  console.log(`build arch:${arch}`)
  const silent = false;
  const gypPath = path.join(process.cwd(), '/node_modules/node-gyp/bin/node-gyp.js');
  const gypExec = `node ${gypPath}`;
  shell.exec(`${gypExec} clean`, {silent});

  if(platform === 'darwin') {
    // macOS 平台编译逻辑
    const command = [`${gypExec} configure`];
    if (arch) {
      command.push(`--arch=${arch}`);
    }
    command.push('-- -f xcode');
    shell.exec(command.join(' '), {silent});
    shell.exec(`xcodebuild -project ./build/binding.xcodeproj -configuration Release -arch x86_64 -arch arm64`, {silent});
  } else if(platform === 'linux') {
    // Linux 平台编译逻辑
    console.log(`[node_pre_build] Building for Linux platform, arch: ${arch}`);
    
    // 为 Linux 平台添加架构映射
    let target_arch = arch;
    if (arch === 'x64' || arch === 'x86_64') {
      target_arch = 'x64';
    } else if (arch === 'arm64' || arch === 'aarch64') {
      target_arch = 'arm64';
    } else if (arch === 'ia32' || arch === 'x86') {
      target_arch = 'ia32';
    }

    // 若为 arm64，设置交叉编译工具链（需要系统已安装 aarch64-linux-gnu-*）
    if (target_arch === 'arm64') {
      process.env.CC = process.env.CC || 'aarch64-linux-gnu-gcc';
      process.env.CXX = process.env.CXX || 'aarch64-linux-gnu-g++';
      process.env.AR = process.env.AR || 'aarch64-linux-gnu-ar';
      process.env.LD = process.env.LD || 'aarch64-linux-gnu-g++';
      process.env.LINK = process.env.LINK || 'aarch64-linux-gnu-g++';
      console.log(`[node_pre_build] arm64 cross toolchain: CC=${process.env.CC}, CXX=${process.env.CXX}, LD=${process.env.LD}`);
    }
    
    console.log(`[node_pre_build] Linux build: mapped arch ${arch} to ${target_arch}`);
    
    const command = [`${gypExec} configure`];
    command.push(`--arch=${target_arch}`);
    
    // Linux 平台特定的编译选项
    if (target_arch === 'arm64') {
      command.push('-- -DCMAKE_CXX_FLAGS="-march=armv8-a"');
    } else if (target_arch === 'x64') {
      command.push('-- -DCMAKE_CXX_FLAGS="-march=x86-64 -mtune=generic"');
    }
    
    console.log(`[node_pre_build] Linux configure command: ${command.join(' ')}`);
    shell.exec(command.join(' '), {silent});
    
    // 执行编译
    console.log(`[node_pre_build] Starting Linux build...`);
    shell.exec(`${gypExec} build`, {silent});
    
    console.log(`[node_pre_build] Linux build completed for arch: ${target_arch}`);
  } else {
    // Windows 平台编译逻辑
    const command = [`${gypExec} configure`];
    command.push(`--arch=${arch}`);
    shell.exec(command.join(' '), {silent});
    shell.exec(`${gypExec} build`, {silent});
    // shell.exec(`msbuild build/binding.sln /p:Configuration=Release`, {silent});
  }
}

function downloadSDK(name_sdk, arch, publish_json) {
  return new Promise((resolve, reject) => {
    console.log(`[node_pre_build] ===== downloadSDK 开始 =====`);
    console.log(`[node_pre_build] 输入参数: name_sdk=${name_sdk}, arch=${arch}, sdk_group=${sdk_group}`);
    console.log(`[node_pre_build] 当前平台: platform=${platform}, package_json.version=${package_json.version}`);
    
    let sdk_list = [];
    Object.keys(publish_json[sdk_group]).forEach((temp) => {
      console.log(`[node_pre_build] 检查版本: ${temp} vs ${package_json.version.split('-')[0]}`);
      if (package_json.version.split('-')[0] === temp) {
        sdk_list = publish_json[sdk_group][temp];
        console.log(`[node_pre_build] 找到匹配版本 ${temp}, SDK 列表长度: ${sdk_list.length}`);
      };
    });
    
    if (sdk_list.length === 0) {
      console.log(`[node_pre_build] ⚠️  警告: 未找到版本 ${package_json.version.split('-')[0]} 的 SDK 列表`);
      console.log(`[node_pre_build] 可用的版本: ${Object.keys(publish_json[sdk_group]).join(', ')}`);
    }
    
    // 为 Linux 平台添加架构映射
    let target_platform = platform;
    let target_arch = arch;
    
    if (platform === 'linux') {
      // Linux 平台架构映射
      if (arch === 'x64' || arch === 'x86_64') {
        target_arch = 'x64';
      } else if (arch === 'arm64' || arch === 'aarch64') {
        target_arch = 'arm64';
      } else if (arch === 'ia32' || arch === 'x86') {
        target_arch = 'ia32';
      }
      console.log(`[node_pre_build] Linux 平台检测到，架构映射: ${arch} -> ${target_arch}`);
    }
    
    console.log(`[node_pre_build] 目标参数: platform=${target_platform}, arch=${target_arch}`);
    console.log(`[node_pre_build] 开始遍历 ${sdk_list.length} 个 SDK 条目...`);
    
    let sdk_url;
    let match_count = 0;
    sdk_list.forEach((member, index) => {
      console.log(`\n[node_pre_build] --- 检查第 ${index + 1} 个 SDK ---`);
      console.log(`[node_pre_build] 条目信息: filename="${member.filename}", client="${member.client}", application="${member.application}"`);
      
      // 检查文件名是否匹配
      const filename = member.filename;
      const matches_sdk = filename.includes(name_sdk);
      const matches_platform = filename.includes(target_platform);
      const matches_arch = filename.includes(target_arch);
      
      console.log(`[node_pre_build] 匹配结果:`);
      console.log(`  - SDK名称匹配: filename="${filename}" 包含 "${name_sdk}" -> ${matches_sdk}`);
      console.log(`  - 平台匹配: filename="${filename}" 包含 "${target_platform}" -> ${matches_platform}`);
      console.log(`  - 架构匹配: filename="${filename}" 包含 "${target_arch}" -> ${matches_arch}`);
      
      if (matches_sdk && matches_platform && matches_arch) {
        sdk_url = member.cdnlink;
        console.log(`✅ [node_pre_build] 找到匹配的 SDK: ${filename}`);
        console.log(`   下载链接: ${sdk_url}`);
        match_count++;
      } else {
        console.log(`❌ [node_pre_build] 不匹配: ${matches_sdk ? '✓' : '✗'}SDK ${matches_platform ? '✓' : '✗'}平台 ${matches_arch ? '✓' : '✗'}架构`);
      }
    });
    
    console.log(`\n[node_pre_build] 匹配统计: 总共 ${sdk_list.length} 个条目，匹配 ${match_count} 个`);
    
    if (!sdk_url) {
      console.log(`\n[node_pre_build] ⚠️  未找到精确匹配的 SDK，开始尝试备用策略...`);
      
      // 如果找不到匹配的 SDK，尝试查找通用版本
      if (platform === 'linux') {
        console.log(`[node_pre_build] 🔍 尝试查找通用 Linux SDK (不要求架构匹配)...`);
        let generic_found = false;
        sdk_list.forEach((member) => {
          const filename = member.filename;
          console.log(`[node_pre_build] 检查通用匹配: filename="${filename}"`);
          if (filename.includes(name_sdk) && filename.includes('linux') && !filename.includes('win') && !filename.includes('darwin')) {
            sdk_url = member.cdnlink;
            console.log(`✅ [node_pre_build] 找到通用 Linux SDK: ${filename}`);
            console.log(`   下载链接: ${sdk_url}`);
            generic_found = true;
          }
        });
        
        if (!generic_found) {
          console.log(`❌ [node_pre_build] 通用 Linux SDK 也未找到`);
        }
      }
      
      if (!sdk_url) {
        console.log(`\n[node_pre_build] 💥 所有策略都失败，无法找到合适的 SDK`);
        console.log(`[node_pre_build] 失败原因分析:`);
        console.log(`  - 目标平台: ${target_platform}`);
        console.log(`  - 目标架构: ${target_arch}`);
        console.log(`  - SDK名称: ${name_sdk}`);
        console.log(`  - 可用版本: ${Object.keys(publish_json[sdk_group]).join(', ')}`);
        console.log(`  - 当前版本SDK数量: ${sdk_list.length}`);
        
        return reject(new Error(`[node_pre_build] Failed to get download url of the pre-built sdk for platform:${target_platform}, arch:${target_arch}`));
      }
    }
    
    console.log(`\n[node_pre_build] 🚀 开始下载 SDK...`);
    console.info(`[node_pre_build] 下载地址: ${sdk_url}`);
    console.info(`[node_pre_build] 目标路径: ${sdk_path}`);
    
    download(sdk_url, sdk_path, {
      extract: true,
      // strip: 1,
      filter: (file) => {
        return !file.path.includes('._');
      },
    }).then(() => {
      console.info(`✅ [node_pre_build] SDK 下载完成`);
      console.log(`[node_pre_build] ===== downloadSDK 结束 =====\n`);
      return resolve();
    }).catch((err) => {
      console.log(`💥 [node_pre_build] downloadSDK 下载失败: ${err}`);
      console.log(`[node_pre_build] ===== downloadSDK 结束(失败) =====\n`);
      return reject(err);
    });
  });
}

function downloadAddon(name_addon, arch, fallBackToBuild, publish_json) {
  return new Promise((resolve, reject) => {
    console.log(`[node_pre_build] ===== downloadAddon 开始 =====`);
    console.log(`[node_pre_build] 输入参数: name_addon=${name_addon}, arch=${arch}, fallBackToBuild=${fallBackToBuild}, addon_group=${addon_group}`);
    console.log(`[node_pre_build] 当前平台: platform=${platform}, package_json.version=${package_json.version}`);
    
    let addon_list = [];
    Object.keys(publish_json[addon_group]).forEach((temp) => {
      console.log(`[node_pre_build] 检查版本: ${temp} vs ${package_json.version.split('-')[0]}`);
      if (package_json.version.split('-')[0] === temp) {
        addon_list = publish_json[addon_group][temp];
        console.log(`[node_pre_build] 找到匹配版本 ${temp}, Addon 列表长度: ${addon_list.length}`);
      };
    });
    
    if (addon_list.length === 0) {
      console.log(`[node_pre_build] ⚠️  警告: 未找到版本 ${package_json.version.split('-')[0]} 的 Addon 列表`);
      console.log(`[node_pre_build] 可用的版本: ${Object.keys(publish_json[addon_group]).join(', ')}`);
    }
    
    // 为 Linux 平台添加架构映射（与 downloadSDK 保持一致）
    let target_platform = platform;
    let target_arch = arch;
    
    if (platform === 'linux') {
      // Linux 平台架构映射
      if (arch === 'x64' || arch === 'x86_64') {
        target_arch = 'x64';
      } else if (arch === 'arm64' || arch === 'aarch64') {
        target_arch = 'arm64';
      } else if (arch === 'ia32' || arch === 'x86') {
        target_arch = 'ia32';
      }
      console.log(`[node_pre_build] downloadAddon: Linux 平台检测到，架构映射: ${arch} -> ${target_arch}`);
    }
    
    console.log(`[node_pre_build] 目标参数: platform=${target_platform}, arch=${target_arch}`);
    console.log(`[node_pre_build] 开始遍历 ${addon_list.length} 个 Addon 条目...`);
    
    let addon_url;
    let match_count = 0;
    addon_list.forEach((member, index) => {
      console.log(`\n[node_pre_build] --- 检查第 ${index + 1} 个 Addon ---`);
      console.log(`[node_pre_build] 条目信息: filename="${member.filename}", client="${member.client}", application="${member.application}"`);
      
      // 检查文件名是否匹配
      const filename = member.filename;
      const matches_addon = filename.includes(name_addon);
      const matches_platform = filename.includes(target_platform);
      const matches_arch = filename.includes(target_arch);
      
      console.log(`[node_pre_build] 匹配结果:`);
      console.log(`  - Addon名称匹配: filename="${filename}" 包含 "${name_addon}" -> ${matches_addon}`);
      console.log(`  - 平台匹配: filename="${filename}" 包含 "${target_platform}" -> ${matches_platform}`);
      console.log(`  - 架构匹配: filename="${filename}" 包含 "${target_arch}" -> ${matches_arch}`);
      
      if (matches_addon && matches_platform && matches_arch) {
        addon_url = member.cdnlink;
        console.log(`✅ [node_pre_build] 找到匹配的 Addon: ${filename}`);
        console.log(`   下载链接: ${addon_url}`);
        match_count++;
      } else {
        console.log(`❌ [node_pre_build] 不匹配: ${matches_addon ? '✓' : '✗'}Addon ${matches_platform ? '✓' : '✗'}平台 ${matches_arch ? '✓' : '✗'}架构`);
      }
    });
    
    console.log(`\n[node_pre_build] 匹配统计: 总共 ${addon_list.length} 个条目，匹配 ${match_count} 个`);
    
    if (!addon_url) {
      console.log(`\n[node_pre_build] ⚠️  未找到精确匹配的 Addon，开始尝试备用策略...`);
      
      // 如果找不到匹配的 addon，尝试查找通用版本
      if (platform === 'linux') {
        console.log(`[node_pre_build] 🔍 尝试查找通用 Linux Addon (不要求架构匹配)...`);
        let generic_found = false;
        addon_list.forEach((member) => {
          const filename = member.filename;
          console.log(`[node_pre_build] 检查通用匹配: filename="${filename}"`);
          if (filename.includes(name_addon) && filename.includes('linux') && !filename.includes('win') && !filename.includes('darwin')) {
            addon_url = member.cdnlink;
            console.log(`✅ [node_pre_build] 找到通用 Linux Addon: ${filename}`);
            console.log(`   下载链接: ${addon_url}`);
            generic_found = true;
          }
        });
        
        if (!generic_found) {
          console.log(`❌ [node_pre_build] 通用 Linux Addon 也未找到`);
        }
      }
      
      if (!addon_url) {
        if (!fallBackToBuild) {
          console.log(`\n[node_pre_build] 💥 所有策略都失败，无法找到合适的 Addon`);
          console.log(`[node_pre_build] 失败原因分析:`);
          console.log(`  - 目标平台: ${target_platform}`);
          console.log(`  - 目标架构: ${target_arch}`);
          console.log(`  - Addon名称: ${name_addon}`);
          console.log(`  - 可用版本: ${Object.keys(publish_json[addon_group]).join(', ')}`);
          console.log(`  - 当前版本Addon数量: ${addon_list.length}`);
          console.log(`  - fallBackToBuild: ${fallBackToBuild}`);
          
          return reject(new Error(`[node_pre_build] Failed to get download url of the pre-built addon for platform:${target_platform}, arch:${target_arch}`));
        }
        console.info(`[node_pre_build] 🔄 无法找到预构建的 Addon，回退到编译模式`);
        console.log(`[node_pre_build] 开始编译 Addon...`);
        build(arch);
        return resolve();
      }
    }
    
    console.log(`\n[node_pre_build] 🚀 开始下载 Addon...`);
    console.info(`[node_pre_build] 下载地址: ${addon_url}`);
    console.info(`[node_pre_build] 目标路径: ${sdk_path}`);
    
    download(addon_url, sdk_path, {
      extract: true,
      filter: (file) => {
        return !file.path.includes('._');
      },
    }).then(() => {
      console.info(`✅ [node_pre_build] Addon 下载完成`);
      console.log(`[node_pre_build] 开始复制 SDK 文件到二进制目录...`);
      copySDKToBinaryDir();
      console.log(`[node_pre_build] downloadAddon copySDKToBinaryDir 完成`);
      console.log(`[node_pre_build] ===== downloadAddon 结束 =====\n`);
      // removeNativeSdk();
      return resolve();
    }).catch((err) => {
      console.log(`💥 [node_pre_build] downloadAddon 下载失败: ${err}`);
      if (!fallBackToBuild) {
        console.log(`[node_pre_build] ===== downloadAddon 结束(失败) =====\n`);
        return reject(err);
      }
      console.info(`[node_pre_build] 🔄 下载失败，回退到编译模式`);
      console.log(`[node_pre_build] 开始编译 Addon...`);
      build(arch);
      console.log(`[node_pre_build] ===== downloadAddon 结束(回退编译) =====\n`);
      return resolve();
    });
  });
}

function install(options) {
  console.log(`[node_pre_build] ===== install 开始 =====`);
  console.log(`[node_pre_build] 输入选项: ${JSON.stringify(options)}`);
  console.log(`[node_pre_build] 当前平台: platform=${platform}, process.arch=${process.arch}`);
  
  let arch = package_json.node_pre_build['arch'];
  console.log(`[node_pre_build] 从 package.json 获取的架构: ${arch}`);
  
  arch = options.arch ? options.arch : arch;
  console.log(`[node_pre_build] 从命令行选项获取的架构: ${options.arch}`);
  
  if (typeof arch === 'undefined') {
    arch = process.env.npm_config_arch;
    console.log(`[node_pre_build] 从环境变量 npm_config_arch 获取: ${arch}`);
    
    if(typeof arch === 'undefined') {
      if (platform === 'darwin') {
        arch = 'universal'
        console.log(`[node_pre_build] macOS 平台，设置架构为 universal`);
      } else {
        arch = process.arch;
        console.log(`[node_pre_build] 非 macOS 平台，使用系统架构: ${arch}`);
      }
    }
  }
  
  console.log(`[node_pre_build] 最终确定的架构: ${arch}`);
  console.log(`[node_pre_build] 开始获取发布列表...`);
  
  // fetch publish list··
  fetch('https://admin.netease.im/public-service/free/publish/list')
    .then((res) => {
      console.log(`[node_pre_build] 获取发布列表响应状态: ${res.status}`);
      return res.json();
    })
    .then((json) => {
      console.log(`[node_pre_build] 发布列表获取成功，数据大小: ${JSON.stringify(json).length} 字符`);
      console.log(`[node_pre_build] 可用的顶级键: ${Object.keys(json.data || {}).join(', ')}`);
      
      let res_data = json.data;
      
      console.log(`[node_pre_build] 开始下载 SDK...`);
      return downloadSDK(name_sdk, arch, res_data).then(() => {
        console.log(`[node_pre_build] SDK 下载完成，开始下载 Addon...`);
        return downloadAddon(name_addon, arch, options.fallBackToBuild, res_data);
      });
    })
    .then(() => {
      console.log(`✅ [node_pre_build] 所有下载完成`);
      console.log(`[node_pre_build] ===== install 结束 =====\n`);
    })
    .catch((err) => {
      console.error(`💥 [node_pre_build] install 失败:`, err);
      console.log(`[node_pre_build] ===== install 结束(失败) =====\n`);
    });
}

// command-line options
// clean
program
    .command('clean')
    .description('Clean installed pre-built binary')
    .action((options) => {
      console.info(`[node_pre_build] removing ${sdk_path}.`);
      fse.removeSync(sdk_path);
      console.info(`[node_pre_build] removing ${temp_path}.`);
      fse.removeSync(temp_path);
    });

// install
program
    .command('install')
    .description('Install pre-built binary for module')
    .option('-a, --arch <architecture>', 'architecture of the host machine.')
    .option('--fall-back-to-build [build-script]', 'build when download pre-built binary failed.')
    .action((options) => {
      console.log(`[node_pre_build] start install`)
      if (fse.pathExistsSync(sdk_path) && fse.readdirSync(sdk_path).length > 0) {
        console.info(`[node_pre_build] sdk already installed in ${sdk_path}.`);
        return;
      }
      install(options);
    });

// reinstall
program
    .command('reinstall')
    .description('Reinstall pre-built binary for module')
    .option('-a, --arch <architecture>', 'architecture of the host machine.')
    .option('--fall-back-to-build [build-script]', 'build when download pre-built binary failed.')
    .action((options) => {
      console.info(`[node_pre_build] removing ${sdk_path}.`);
      fse.removeSync(sdk_path);
      console.info(`[node_pre_build] removing ${temp_path}.`);
      fse.removeSync(temp_path);
      install(options);
    });

// build
program
    .command('build')
    .description('Build and pack your pre-built binaries.')
    .option('-r, --runtime <runtime...>', 'array of runtimes to build for, such as [electron, node, nw].')
    .option('-rv, --runtime-version <runtime-version...>', 'array of runtime versions to build for, support multiple versions.')
    .option('-a, --arch <arch...>', 'array of architechtures to build for, such as [x64, ia32, arm64, arm].')
    .option('-p, --pack', 'pack the binaries after build.')
    .action((options) => {

      let arch_array = options.arch ? options.arch : package_json.node_pre_build['arch'];
      if (!Array.isArray(arch_array)) {
        arch_array = [arch_array];
      };

      if (!fse.pathExistsSync(process.cwd() + '/' + package_dir)) {
        fse.mkdirSync(process.cwd() + '/' + package_dir);
      };
      
      console.log(`---platform:${platform} arch:${arch_array[0]}-----`)
      build(arch_array[0]);

      if (!options.pack) {
        return;
      };
      let package_name = `${process.cwd() + '/' + package_dir}/${name_addon}-v${package_json.version}-${platform}-${arch_array[0]}.tar.gz`
      console.log(`package_name:${package_name}`)
      tar.create({
        gzip: true,
        sync: true,
        cwd: process.cwd() + '/' + binary_dir,
        file: package_name,
        filter: (path, stat) => {
          if (path.match(/\.pdb|\.node/g) !== null) {
            console.info(`[node_pre_build] ${path} packed.`);
            return true;
          }
        },
      }, fse.readdirSync(process.cwd() + '/' + binary_dir));
      
    });

// parse
program.parse();
