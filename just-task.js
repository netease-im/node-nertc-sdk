const { task, option, logger } = require('just-task');
const path = require('path')
const build = require('./scripts/build')
const download = require('./scripts/download')
const cleanup = require('./scripts/cleanup')
const extract = require('./scripts/extract')

option('electron_version', {default: '8.1.1'});
option('runtime', {default: 'electron', choices: ['electron', 'node']});
option('platform', {default: process.platform, choices: ['darwin', 'win32']});
option('debug', {default: false, boolean: true});
option('silent', {default: false, boolean: true});
option('msvs_version', {default: '2015'});

// trigger when run npm install
task('install', () => {
  return new Promise((resolve, reject) => {
    cleanup(path.join(__dirname, "./build")).then(() => {
      cleanup(path.join(__dirname, "./nertc_sdk")).then(() => {
        download("http://yx-web.nos.netease.com/package/nertc_sdk.zip", "../").then(f => {
          extract(f, "../").then(() => {
            build({})
            resolve()
            // build({}).then(() => {
            //   resolve()
            // })
          })
        })
      })
    })
  })
})

// cleanup(path.join(__dirname, "./nertc-sdk.zip").then(() => {})