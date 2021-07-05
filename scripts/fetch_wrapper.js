const path = require('path')
const download = require('download')
const tar = require('tar')
const fs = require('fs')
const extract = require('extract-zip')
const { logger } = require('just-task')

module.exports = ({
  fetchUrl,
  extractPath,
  temporaryPath,
  platform = process.platform,
  arch = process.arch
}) => {
  return new Promise((resolve, reject) => {
    logger.info(`Downloading file from: ${fetchUrl} to ${temporaryPath}`)
    if (fs.existsSync(temporaryPath)) {
      fs.rmdirSync(temporaryPath, { recursive: true })
    }
    if (fs.existsSync(extractPath)) {
      fs.rmdirSync(extractPath, { recursive: true })
    }
    if (fs.existsSync(extractPath.replace('nertc_sdk', 'sdk'))) {
      fs.rmdirSync(extractPath.replace('nertc_sdk', 'sdk'), { recursive: true })
    }
    download(fetchUrl, temporaryPath).then(() => {
      const zipFile = fs.readdirSync(temporaryPath)
      if (zipFile.length > 0) {
        if (!fs.existsSync(extractPath)) {
          fs.mkdirSync(extractPath)
        }
        extract(path.join(temporaryPath, zipFile[0]), {
          dir: extractPath.replace('nertc_sdk', '')
        }).then(() => {
          if (platform == 'win32') {
            let copyArch
            if (arch === 'ia32') {
              copyArch = 'x86'
            } else if (arch === 'x64') {
              copyArch = 'x64'
            } else {
              reject(new Error('Unsupported arch.'))
            }
            const dllSrcPath = path.join(extractPath, `bin/windows/${copyArch}`)
            const libSrcPath = path.join(extractPath, `libs/windows/${copyArch}`)
            const dlls = fs.readdirSync(dllSrcPath)
            dlls.map((dll) => {
              logger.info(`Copy file from ${dll} to ${extractPath}/bin`)
              fs.copyFileSync(path.join(dllSrcPath, dll), path.join(extractPath, `bin/${dll}`))
            })
            const libs = fs.readdirSync(libSrcPath)
            libs.map((lib) => {
              logger.info(`Copy file from ${lib} to ${extractPath}/libs`)
              fs.copyFileSync(path.join(libSrcPath, lib), path.join(extractPath, `libs/${lib}`))
            })
          } else if (platform === 'darwin') {
            fs.renameSync(path.join(extractPath.replace('nertc_sdk', ''), 'sdk'), extractPath)
          } else {
            return reject(new Error('Unsupported platform.'))
          }
          resolve()
        })
      }
    }).catch(err => {
      console.log(err);
      reject(err)
    })
  })
}
