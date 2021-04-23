const { task, option, logger } = require('just-task');
const path = require('path')
const build = require('./scripts/build')
const download = require('./scripts/download')
const cleanup = require('./scripts/cleanup')
const extract = require('./scripts/extract')

let electron_version = '8.5.5'
let sdk_url = `http://yx-web.nos.netease.com/package/${process.platform}-sdk.zip`
const projectDir = path.join(process.env.INIT_CWD, 'package.json')
const pkgMeta = require(projectDir);
if (pkgMeta.nertc_config) {
  electron_version = pkg.pkgMeta.nertc_config.electron_version
}

// trigger when run npm install
task('install', () => {
  return new Promise((resolve, reject) => {
    cleanup(path.join(__dirname, "./build")).then(() => {
      cleanup(path.join(__dirname, "./nertc_sdk")).then(() => {
        download(sdk_url, "../").then(f => {
          extract(f, "../").then(() => {
            build({electronVersion: electron_version})
            resolve()
          })
        })
      })
    })
  })
})

// cleanup(path.join(__dirname, "./nertc-sdk.zip").then(() => {})