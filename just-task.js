const { task, option, logger } = require('just-task');
const path = require('path')
const build = require('./scripts/build')
const download = require('./scripts/download')
const cleanup = require('./scripts/cleanup')

option('electron_version', {default: '8.1.1'});
option('runtime', {default: 'electron', choices: ['electron', 'node']});
option('platform', {default: process.platform, choices: ['darwin', 'win32']});
option('debug', {default: false, boolean: true});
option('silent', {default: false, boolean: true});
option('msvs_version', {default: '2015'});

// trigger when run npm install
task('install', () => {
  // work-around
  return new Promise((resolve, reject) => {
    cleanup(path.join(__dirname, "./build")).then(_ => {
      return cleanup(path.join(__dirname, "./nertc_sdk")).then(_ => {
        return cleanup(path.join(__dirname, "./nertc-sdk.zip")).then(_ => {
          return download().then(() => {
            logger.info("start building...");
            return build({}).then(() => {
              resolve()
            }).catch(err => {
              reject(err)
            })
          })
        })
      })
    })
  })
})

