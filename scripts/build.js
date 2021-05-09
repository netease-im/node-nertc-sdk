const { logger } = require('just-task');
const shell = require("shelljs");
const path = require('path')
const fs = require('fs')

module.exports = ({
  nodeVersion = '8.1.1',
  runtime = 'electron',
  platform = process.platform,
  debug = false,
  silent = false,
  msvsVersion = '2015',
  arch = 'ia32',
  distUrl = 'https://electronjs.org/headers'
}) => {
  logger.info("Platform:", platform)
  logger.info("Runtime:", runtime)
  logger.info("Arch:", arch)
  logger.info("Debug:", debug)

  if (runtime === 'electron') {
    let electron_gyp_exec = `${path.resolve(__dirname, '../../.bin/electron-rebuild')}`
    if (!fs.existsSync(electron_gyp_exec)) {
      logger.info(`electron-gyp_exec not found at ${electron_gyp_exec}, switch`)
      electron_gyp_exec = `${path.resolve(__dirname, '../node_modules/.bin/electron-rebuild')}`
    }
    let sdk_path = `${path.resolve(__dirname, '../../nertc-electron-sdk')}`
    let framework_path = `${path.resolve(__dirname, '../../nertc-electron-sdk/nertc_sdk/bin/darwin')}`
    if (!fs.existsSync(sdk_path)) {
      logger.info(`sdk_path not found at ${sdk_path}, switch`)
      sdk_path = `${path.resolve(__dirname, '../node_modules/nertc-electron-sdk')}`
      framework_path = `${path.resolve(__dirname, '../node_modules/nertc-electron-sdk/nertc_sdk/bin/darwin')}`
    }
    const command = [`${electron_gyp_exec} --module-dir=${sdk_path}`];
    // check platform
    if (platform === 'win32') {
      command.push(`--arch=${arch}`)
    }
    // check debug
    if (debug) {
      command.push('--debug');
    }
    const commandStr = command.join(' ')

    /** start build */
    logger.info(commandStr, '\n');
    logger.info("Building...\n")

    shell.exec(`npm install -D electron-rebuild@latest`, { silent }, (code, stdout, stderr) => {
      if (code !== 0) {
        logger.error('npm install -D electron-rebuild@latest error!!!!!');
        logger.error(stderr);
        process.exit(1)
      }
      shell.exec(`${commandStr}`, { silent }, (code, stdout, stderr) => {
        // handle error
        if (code !== 0) {
          logger.error('build error!!!!!');
          logger.error(stderr);
          process.exit(1)
        }

        if(platform === "darwin") {
          logger.info(`patch loader path for mac build..`)
          shell.exec(`install_name_tool -add_rpath "@loader_path" ${framework_path}`, {silent}, (code, stdout, stderr) => {
            if (code !== 0) {
              logger.error(stderr);
              process.exit(1)
            }
            // handle success
            logger.info('Build complete')
            process.exit(0)
          })
        }
      })
    })
  } else {
    logger.info("Node Version:", nodeVersion)
    logger.info("MSVS Version:", msvsVersion)
    // workaround to find executable when install as dependency
    let gyp_path = `${path.resolve(__dirname, '../../node-gyp/bin/node-gyp.js')}`

    if (!fs.existsSync(gyp_path)) {
      logger.info(`gyp_exec not found at ${gyp_path}, switch`)
      gyp_path = `${path.resolve(__dirname, '../node_modules/node-gyp/bin/node-gyp.js')}`
    }
    const gyp_exec = `node ${gyp_path}`

    /** get command string */
    const command = [`${gyp_exec} configure`];
    // check platform
    if (platform === 'win32') {
      // command.push(`--arch=${arch} --msvs_version=${msvsVersion}`)
      command.push(`--arch=${arch} --msvs_version=${msvsVersion}`)
    }
    command.push(`--target=${electronVersion} --dist-url=${distUrl}`)
    // check debug
    if (debug) {
      command.push('--debug');
      if (platform === 'darwin') {
        // MUST AT THE END OF THE COMMAND ARR
        command.push('-- -f xcode')
      }
    }
    const commandStr = command.join(' ')

    /** start build */
    logger.info(commandStr, '\n');
    logger.info("Building...\n")

    shell.exec(`${gyp_exec} clean`, { silent }, (code, stdout, stderr) => {
      // handle error
      logger.info(`clean done ${stdout}`)
      if (code !== 0) {
        logger.error(stderr);
        process.exit(1)
      }

      shell.exec(commandStr, { silent }, (code, stdout, stderr) => {
        // handle error
        logger.info(`configure done ${stdout}`)
        if (code !== 0) {
          logger.error(stderr);
          process.exit(1)
        }

        if (debug) {
          // handle success
          logger.info('Complete, please go to `/build` and build manually')
          process.exit(0)
        } else {
          shell.exec(`${gyp_exec} build`, { silent }, (code, stdout, stderr) => {
            // handle error
            if (code !== 0) {
              logger.error('build error!!!!!');
              logger.error(stderr);
              process.exit(1)
            }

            // handle success
            logger.info('Build complete')
            process.exit(0)
          })
        }
      })
    })
  }
}