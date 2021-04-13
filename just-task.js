const { task, option, logger, argv, series, condition } = require('just-task');
const path = require('path')
const build = require('./scripts/build')
const download = require('./scripts/download')
const cleanup = require('./scripts/cleanup')
const {getArgvFromNpmEnv, getArgvFromPkgJson} = require('./scripts/npm_argv')

option('electron_version', {default: '8.1.1'});
option('runtime', {default: 'electron', choices: ['electron', 'node']});
option('platform', {default: process.platform, choices: ['darwin', 'win32']});
option('debug', {default: false, boolean: true});
option('silent', {default: false, boolean: true});
option('msvs_version', {default: '2015'});

const packageVersion = require('./package.json').version;

// npm run build:electron -- 
task('build:electron', () => {
  cleanup(path.join(__dirname, "./build")).then(_ => {
    build({
      electronVersion: argv().electron_version, 
      runtime: argv().runtime, 
      platform: argv().platform, 
      packageVersion, 
      debug: argv().debug, 
      silent: argv().silent,
      arch: argv().arch,
      msvsVersion: argv().msvs_version,
      distUrl: argv().dist_url
    })
  })
})

// npm run download --
task('download', () => {
  // work-around
  download()
})

// trigger when run npm install
task('install', () => {
  // work-around
  download().then(() => {
    return build()
  })
})

