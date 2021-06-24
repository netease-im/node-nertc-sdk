const { task, option, logger, argv } = require('just-scripts')
const fs = require('fs')
const download = require('download')
const path = require('path')
const fetchWrapper = require('./scripts/fetch_wrapper')
const buildAddon = require('./scripts/build_addon')
const packAddon = require('./scripts/pack_addon')


option('target')
option('target_platform', { default: process.platform, choices: ['darwin', 'win32', 'linux'] })
option('target_arch', { default: process.arch, choices: ['ia32', 'x64'] })
option('runtime', { default: 'electron', choices: ['electron', 'node'] })
option('debug', { default: false, boolean: true })
option('silent', { default: false, boolean: true })

const includePath = 'nertc_sdk'
const replaceKey = '{platform}'
const packageMeta = require(path.join(__dirname, 'package.json'))
const nativeUrl = `http://yx-web.nos.netease.com/package/1623409441/NERtc_${replaceKey}_SDK_v4.3.1.zip`

task('fetch-wrapper', () => {
  const platform = argv().target_platform
  const cachePath = path.join(__dirname, includePath)
  return fetchWrapper({
    fetchUrl: nativeUrl.replace(replaceKey, platform == 'win32' ? 'Windows' : 'Mac'),
    extractPath: cachePath
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
  let target = '5.0.8'
  let runtime = 'electron'
  const targetPlatform = process.platform
  const targetArch = process.arch
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
      strip: 1,
      extract: true
    }).then(() => {
      logger.info(`[install] Download prebuilt binaries from ${host}/${remotePath}/${packageName}`)
      resolve()
    }).catch(err => {
      logger.warn(`[install] Failed to download package from: ${host}/${remotePath}/${packageName}, error code: ${err.statusCode}`)
      logger.info('[install] Start build from local source file.')
      const cachePath = path.join(__dirname, includePath)
      fetchWrapper({
        fetchUrl: nativeUrl.replace(replaceKey, targetPlatform == 'win32' ? 'Windows' : 'Mac'),
        extractPath: cachePath
      }).then(() => {
        return buildAddon({
          target,
          runtime
        })
      }).then(() => resolve())
    })
  })
})
