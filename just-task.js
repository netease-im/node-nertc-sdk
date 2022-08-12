const { task, option, logger, argv } = require('just-task')
const download = require('download')
const path = require('path')
const fsExtra = require('fs-extra')
const nodeAbi = require('node-abi')
const fetch = require('node-fetch')
const glob = require('glob');
const shell = require('shelljs')
const tar = require('tar')

option('target')
option('target_platform', { default: process.platform, choices: ['darwin', 'win32', 'linux'] })
option('target_arch', { default: process.arch, choices: ['ia32', 'x64'] })
option('runtime', { default: 'electron', choices: ['electron', 'node'] })
option('debug', { default: false, boolean: true })
option('silent', { default: false, boolean: true })
option('download_url')

const sdkPackageJson = require(path.join(__dirname, 'package.json'))
const sdkPath = path.join(process.cwd(), sdkPackageJson.node_pre_build['sdk-dir'])
const binary_dir = `build/Release`
const package_dir = `packages`

function packageAddon(sdkVersion, target, runtime, platform, arch) {
  return new Promise((resolve, reject) => {
    if (!fsExtra.pathExistsSync(process.cwd() + '/' + package_dir)) {
      fsExtra.mkdirSync(process.cwd() + '/' + package_dir);
    }
    const abi_version = nodeAbi.getAbi(target, runtime);
    tar.create({
      gzip: true,
      sync: true,
      cwd: process.cwd() + '/' + binary_dir,
      file: `${process.cwd() + '/' + package_dir}/nertc-v${sdkVersion}-abi${abi_version}-${platform}-${arch}.tar.gz`,
      filter: (path, stat) => {
        if (path.match(/\.pdb|\.node/g) !== null) { //pre /\.pdb|\.dll|\.node|\.framework|\.dylib/g
          console.info(`addon path:${path} packed.`)
          return true
        }
      },
    }, fsExtra.readdirSync(process.cwd() + '/' + binary_dir))
    return resolve()
  })
}

function removeNativeSdk() {
  if(fsExtra.pathExistsSync(sdkPath)){
    fsExtra.rmdirSync(sdkPath, { recursive: true })
    console.log(`[node_pre_build] delecte NertcSdk end`)
  }
  let bindingGyp = path.join(process.cwd(), 'binding.gyp')
  fsExtra.pathExists(bindingGyp, (err, exists) => {
    if (exists) {
      fsExtra.removeSync(bindingGyp)
      console.log(`[node_pre_build] delecte bindingGyp`)
    }
  })
}

function copySDKToBinaryDir() {
  const temp = glob.sync('/**/+(*.dll|*.framework|*.dylib|*.so|*.node)', {
    root: sdkPath
  })
  const files = []
  temp.forEach((filepath) => {
    console.log('pre copySync file:' + path.basename(filepath))
    if(!filepath.includes('dSYM')){
      files.push(filepath)
    }
  })
  if (!fsExtra.pathExistsSync(path.join(process.cwd(), binary_dir))) {
    fsExtra.mkdirSync(path.join(process.cwd(), binary_dir), {recursive: true});
  }
  files.forEach((filepath) => {
    console.log('after copySync file:' + path.basename(filepath))
    fsExtra.copySync(filepath, path.join(process.cwd(), binary_dir, path.basename(filepath)))
  })
  console.log(`[node_pre_buidld] copySDKToBinaryDir end`)
}

function build(runtime, version, arch) {
  return new Promise((resolve, reject) => {
    console.log(`build addon frome sdk`)
    const msvcVersion = '2017';
    const silent = false;
    const distUrl = 'https://electronjs.org/headers';
    const gypPath = path.join(process.cwd(), '/node_modules/node-gyp/bin/node-gyp.js');
    const gypExec = `node ${gypPath}`;
    const command = [`${gypExec} configure`];
    command.push(`--arch=${arch} --msvs_version=${msvcVersion}`);
    if (!runtime || !version) {
      runtime = 'node'
      version = process.versions.node
    }
    if('electron' == runtime){
      command.push(`--target=${version} --dist-url=${distUrl}`);
    }
    command.push(' --openssl_fips=X')
    shell.exec(`${gypExec} clean`, {silent})
    shell.exec(command.join(' '), {silent})
    shell.exec(`${gypExec} build`, {silent})
    return resolve()
  })
}

function downloadSDK (publishJson, targetPlatform, targetArch) {
  return new Promise((resolve, reject) => {
    let sdkList = []
    Object.keys(publishJson[`NERtcSDK`]).forEach((temp) => {
      if (sdkPackageJson.version.split('-')[0] === temp) {
        sdkList = publishJson[`NERtcSDK`][temp]
      }
    })
    let sdkUrl = ``
    sdkList.forEach((member) => {
      if (member.filename.includes(`nertc`) && 
        member.filename.includes(targetPlatform) && 
        member.filename.includes(targetArch)) {
        sdkUrl = member.cdnlink
      }
    })
    console.log(`sdk url:${sdkUrl}`)
    download(sdkUrl, sdkPath, {
      extract: true,
      filter: (file) => {
          return !file.path.includes('._')
      }
    })
    .then(() => {
      console.info(`Downloading sdk complete`)
      return resolve()
    })
    .catch((err) => {
      console.error(`Downloading sdk error:${err}`)
      return reject()
    })
  })
}

function downlaodAddon(publishJson, abiVersion, targetPlatform, targetArch) {
  return new Promise((resolve, reject) => {
    //download addon
    let addon_list = []
    Object.keys(publishJson[`electron`]).forEach((temp) => {
      if (sdkPackageJson.version.split('-')[0] === temp) {
        addon_list = publishJson[`electron`][temp]
      }
    })
    let addon_url
    console.log(`platform:${platform} arch:${arch} abiVersion:${abiVersion}`)
    addon_list.forEach((member) => {
      if (member.filename.includes(`nertc`) &&
        member.filename.includes(targetPlatform) && 
        member.filename.includes(targetArch) &&
        member.filename.includes(abiVersion)) {
        addon_url = member.cdnlink;
      }
    })
    console.log(`addon url:${addon_url}`)
    if(!addon_url) {
      return reject()
    }
    download(addon_url, sdkPath, {
      extract: true,
      filter: (file) => {
          return !file.path.includes('._')
      }
    })
    .then(() => {
      console.info(`Downloading prebuilt addon complete`)
      copySDKToBinaryDir()
      removeNativeSdk()
      return resolve()
    })
    .catch((err) => {
      console.error(`Downloading prebuilt addon error:${err}`)
      return reject()
    })
  })
}

task('fetch-wrapper', () => {
  const platform = argv().target_platform
  const arch = argv().target_arch
  fetch('https://admin.netease.im/public-service/free/publish/list')
    .then((res) => res.json())
    .then((json) => {
      let publishJson =json.data
      downloadSDK(publishJson, platform, arch)
    })
})

task('build', () => {
  const target = argv().target
  const platform = argv().target_platform
  const arch = argv().target_arch
  const runtime = argv().runtime
  const sdkVersion = sdkPackageJson.version
  logger.info(JSON.stringify(argv()))
  build(runtime, target, arch).then(() => {
    packageAddon(sdkVersion,target,runtime, platform, arch)
  })
})

task('clean', () => {
  if(fsExtra.pathExistsSync(sdkPath)){
    fsExtra.rmdirSync(sdkPath, { recursive: true })
    console.log(`[node_pre_build] delecte NertcSdk end`)
  }
})

task('install', () => {
  if ((process.env.npm_config_skip_install || false)) {
    logger.info('[install] Skip downlaod prebuilt libraries.')
    return
  }
  let target = '13.0.0'
  let runtime = 'electron'
  logger.info('nertc just install')
  const appPackageJson = require(path.join(process.env.INIT_CWD, 'package.json'))
  if (appPackageJson.devDependencies && appPackageJson.devDependencies.electron) {
    // v13.1.2 => 13.1.2, remove prefix 'v'
    target = appPackageJson.devDependencies.electron.replace(/^.*?(\d+.+\d).*/, '$1')
  } else {
    target = process.version.match(/^v(\d+\.\d+)/)[1]
    runtime = 'node'
  }
  let abiVersion = ''
  if (runtime === 'electron') {
    abiVersion = nodeAbi.getAbi(target, 'electron')
  } else {
    abiVersion = nodeAbi.getAbi(process.versions.node, 'node')
  }
  logger.info(`abi version:${abiVersion}`)
  fetch('https://admin.netease.im/public-service/free/publish/list')
    .then((res) => res.json())
    .then((json) => {
      let publishJson =json.data
      const platform = process.env.npm_config_target_platform || process.platform
      const arch = process.env.npm_config_target_arch || process.arch
      logger.info(`just install platform:${platform} arch:${arch}`)
      downloadSDK(publishJson, platform, arch).then(() => {
        downlaodAddon(publishJson, abiVersion, platform, arch).catch(()=>{
          build(runtime, target, arch).then(() => {
            removeNativeSdk()
          })
        })
      })
    })
})
