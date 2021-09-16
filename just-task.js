const { task, option, logger, argv } = require('just-task')
const fs = require('fs')
const download = require('download')
const path = require('path')
const fetchWrapper = require('./scripts/fetch_wrapper')
const buildAddon = require('./scripts/build_addon')
const packAddon = require('./scripts/pack_addon')
const fsExtra = require('fs-extra')

option('target')
option('target_platform', { default: process.platform, choices: ['darwin', 'win32', 'linux'] })
option('target_arch', { default: process.arch, choices: ['ia32', 'x64'] })
option('runtime', { default: 'electron', choices: ['electron', 'node'] })
option('debug', { default: false, boolean: true })
option('silent', { default: false, boolean: true })
option('download_url')

const includePath = 'nertc_sdk'
const tempPath = 'temporary'
const packageMeta = require(path.join(__dirname, 'package.json'))
const nativeWinUrl = `http://yx-web.nos.netease.com/package/1631797164/NeRTC_Windows_SDK_v4.1.113.zip`
const nativeMacUrl = `http://yx-web.nos.netease.com/package/1631797841/NeRTC_macOS_SDK_v4.1.113.zip`

task('fetch-wrapper', () => {
  const platform = argv().target_platform
  const arch = argv().target_arch
  const temporaryPath = path.join(__dirname, tempPath)
  const extractPath = path.join(__dirname, includePath)
  const downloadUrl = argv().download_url
  let fetchUrl
  if (platform === 'win32') {
    fetchUrl = downloadUrl || nativeWinUrl
  } else if (platform === 'darwin') {
    fetchUrl = downloadUrl || nativeMacUrl
  }
  return fetchWrapper({
    platform,
    arch,
    fetchUrl,
    temporaryPath,
    extractPath
  })
})

task('build', () => {
  const target = argv().target
  const platform = argv().target_platform
  const arch = argv().target_arch
  const runtime = argv().runtime
  const version = packageMeta.version
  const packageName = packageMeta.name
  const sourcePath = path.join(__dirname, includePath)

  logger.info(JSON.stringify(argv()))

  return new Promise((resolve, reject) => {
    buildAddon({
      target,
      runtime,
      platform,
      arch
    }).then(() => {
      return packAddon({
        packageName,
        version,
        target,
        platform,
        arch,
        runtime
      })
    }).then(() => resolve())
  })
})

task('package', () => {
  logger.info(JSON.stringify(argv()))
  const target = argv().target
  const platform = argv().target_platform
  const arch = argv().target_arch
  const runtime = argv().runtime
  const version = packageMeta.version
  const packageName = packageMeta.name
  return packAddon({
    packageName,
    version,
    target,
    platform,
    arch,
    runtime
  })
})

task('install', () => {
  if ((process.env.npm_config_skip_install || false)) {
    logger.info('[install] Skip downlaod prebuilt libraries.')
    return
  }
  let target = '5.0.8'
  let runtime = 'electron'
  const targetPlatform = process.env.npm_config_target_platform || process.platform
  const targetArch = process.env.npm_config_target_arch || process.arch
  const downloadUrl = process.env.npm_config_download_url
  const curPkgMeta = require(path.join(__dirname, 'package.json'))
  const rootPkgMeta = require(path.join(process.env.INIT_CWD, 'package.json'))
  logger.info('------------------ just install --------------------')
  if (rootPkgMeta.devDependencies && rootPkgMeta.devDependencies.electron) {
    // v13.1.2 => 13.1.2, remove prefix 'v'
    target = rootPkgMeta.devDependencies.electron.replace(/^.*?(\d+.+\d).*/, '$1')
  } else {
    target = process.version.match(/^v(\d+\.\d+)/)[1]
    runtime = 'node'
  }
  // 13.1.2 => 13.1, match major.minor only
  const nodeAbi = `${runtime}-v${target.replace(/^(\d+.+?\d+).*/, '$1')}`
  return new Promise((resolve, reject) => {
    const host = 'https://yx-web-nosdn.netease.im'
    const remotePath = 'package'
    const packageName = `${curPkgMeta.name}-v${curPkgMeta.version}-${nodeAbi}-${targetPlatform}-${targetArch}.tar.gz`
    const localPath = 'build/Release'
    fs.rmdirSync(path.join(__dirname, localPath), { recursive: true })
    download(`${host}/${remotePath}/${packageName}`, path.join(__dirname, localPath), {
      extract: true
    }).then(() => {
      logger.info(`[install] Download prebuilt binaries from ${host}/${remotePath}/${packageName}`)
      if(process.platform == 'darwin'){
        var marchDriver = new RegExp(/.+\.driver$/)
        let releaseDir = path.join(__dirname, localPath)
        const list = fs.readdirSync(releaseDir)
        list.map(framework => {
          if(marchDriver.test(framework)){
            let srcDriverPath = path.join(releaseDir, framework)
            let distDriverPath =  "/private/tmp/NeCastAudio/NeCastAudio.driver"
            let delDriverPath =  "/private/tmp/NeCastAudio"
            if(fs.existsSync(delDriverPath)){
              var ret = fs.rmdirSync(delDriverPath, { recursive: true })
              console.log("-------just task delete pre deriver-----"+ret)
            }
            console.log("-------just task copySync deriver------")
            fsExtra.copySync(srcDriverPath, distDriverPath)
          }
        })
      }
      resolve()
    }).catch(err => {
      let fetchUrl
      if (targetPlatform === 'win32') {
        fetchUrl = downloadUrl || nativeWinUrl
      } else if (targetPlatform === 'darwin') {
        fetchUrl = downloadUrl || nativeMacUrl
      }
      logger.warn(`[install] Failed to download package from: ${host}/${remotePath}/${packageName}, error code: ${err.statusCode}`)
      logger.info('[install] Start build from local source file.')
      const extractPath = path.join(__dirname, includePath)
      fetchWrapper({
        fetchUrl,
        extractPath
      }).then(() => {
        return buildAddon({
          target,
          runtime
        })
      }).then(() => resolve()).catch((err) => {
        reject(err)
      })
    })
  })
})
