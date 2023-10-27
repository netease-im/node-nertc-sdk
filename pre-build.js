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
  const temp = glob.sync('/**/+(*.dll|*.framework|*.dylib|*.so|*.node)', {
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
    shell.exec(`${gypExec} configure -- -f xcode`, {silent});
    shell.exec(`xcodebuild -project ./build/binding.xcodeproj -configuration Release`, {silent});
  } else {
    const command = [`${gypExec} configure`];
    command.push(`--arch=${arch}`);
    shell.exec(command.join(' '), {silent});
    shell.exec(`${gypExec} build`, {silent});
    // shell.exec(`msbuild build/binding.sln /p:Configuration=Release`, {silent});
  }

}

function downloadSDK(name_sdk, arch, publish_json) {
  return new Promise((resolve, reject) => {
    let sdk_list = [];
    Object.keys(publish_json[sdk_group]).forEach((temp) => {
      if (package_json.version.split('-')[0] === temp) {
        sdk_list = publish_json[sdk_group][temp];
      };
    });
    console.log(`[node_pre_build] downloadSDK name_sdk:${name_sdk}, platform:${platform}, arch:${arch}`)
    let sdk_url;
    sdk_list.forEach((member) => {
      if (member.filename.includes(name_sdk) && member.filename.includes(platform) && member.filename.includes(arch)) {
        sdk_url = member.cdnlink;
      }
    });
    if (!sdk_url) {
      return reject(new Error('[node_pre_build] Failed to get download url of the pre-built sdk'));
    }
    console.info(`[node_pre_build] Downloading prebuilt sdk from ${sdk_url} to ${sdk_path}`);
    download(sdk_url, sdk_path, {
      extract: true,
      filter: (file) => {
        return !file.path.includes('._');
      },
    }).then(() => {
      console.info(`[node_pre_build] Downloading prebuilt sdk complete`);
      return resolve();
    }).catch((err) => {
      console.log(`[node_pre_build] downloadSDK error:${err}`)
      return reject(err);
    });
  });
}

function downloadAddon(name_addon, arch, fallBackToBuild, publish_json) {
  return new Promise((resolve, reject) => {
    let addon_list = [];
    Object.keys(publish_json[addon_group]).forEach((temp) => {
      if (package_json.version.split('-')[0] === temp) {
        addon_list = publish_json[addon_group][temp];
      };
    });
    let addon_url;
    addon_list.forEach((member) => {
      if (member.filename.includes(name_addon) &&
        member.filename.includes(platform) && 
        member.filename.includes(arch)) {
        addon_url = member.cdnlink;
      }
    });
    if (!addon_url) {
      if (!fallBackToBuild) {
        return reject(new Error('[node_pre_build] Failed to get download url of the pre-built addon.'));
      }
      console.info('[node_pre_build] Failed to get download url of the pre-built addon, falling back to build.');
      build(arch);
      return resolve();
    }
    console.info(`[node_pre_build] Downloading prebuilt addon from ${addon_url}`);
    download(addon_url, sdk_path, {
      extract: true,
      filter: (file) => {
        return !file.path.includes('._');
      },
    }).then(() => {
      copySDKToBinaryDir();
      console.log(`[node_pre_build] downloadAddon copySDKToBinaryDir end`)
      removeNativeSdk();
    }).catch((err) => {
      console.log(`[node_pre_build] downloadAddon err:${err}`)
      if (!fallBackToBuild) {
        return reject(err);
      }
      console.info(`[node_pre_build] Failed to download pre-built addon from ${addon_url}, error ${err}, falling back to build.`);
      build(arch);
      return resolve();
    });
  });
}

function install(options) {
  let arch = package_json.node_pre_build['arch'];
  arch = options.arch ? options.arch : arch;
  if (typeof arch === 'undefined') {
    arch = process.env.npm_config_arch;
    console.log(`install optins npm_config_arch:${arch}`)
    if(typeof arch === 'undefined') {
      if (platform === 'darwin') {
        arch = 'universal'
      } else {
        arch = process.arch;
      }
    }
  }
  console.log(`install optins arch:${arch}`)
  // fetch publish list··
  fetch('https://admin.netease.im/public-service/free/publish/list').then((res) => {return res.json()}).then((json) => {
    let res_data = json.data;
    return downloadSDK(name_sdk, arch, res_data).then(() => {
      return downloadAddon(name_addon, arch, options.fallBackToBuild, res_data);
    });
  }).catch((err) => {
    console.error(err);
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
