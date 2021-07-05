const path = require('path')
const download = require('download')
const tar = require('tar')
const fs = require('fs')
const { logger } = require('just-task')

module.exports = ({
  fetchUrl,
  extractPath,
  platform = process.platform,
  arch = process.arch
}) => {
  return new Promise((resolve, reject) => {
    logger.info(`Remove file ${extractPath}`)
    fs.rmdirSync(extractPath, { recursive: true });
    logger.info(`Downloading file from: ${fetchUrl} to ${extractPath}`)
    download(fetchUrl, extractPath, {
      strip: 1,
      extract: true
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
        const dllSrcPath = path.join(extractPath, `bin/${copyArch}`)
        const libSrcPath = path.join(extractPath, `lib/${copyArch}`)
        const dlls = fs.readdirSync(dllSrcPath)
        dlls.map((dll) => {
          logger.info(`Copy file from ${dll} to ${extractPath}/dll`)
          fs.copyFileSync(path.join(dllSrcPath, dll), path.join(extractPath, `dll/${dll}`))
        })
        const libs = fs.readdirSync(libSrcPath)
        libs.map((lib) => {
          logger.info(`Copy file from ${lib} to ${extractPath}/lib`)
          fs.copyFileSync(path.join(libSrcPath, lib), path.join(extractPath, `lib/${lib}`))
        })
      }
      resolve()
    }).catch(err => {
      reject(err)
    })
  })
}
