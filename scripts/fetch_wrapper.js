const path = require('path')
const download = require('download')
const fs = require('fs')
const fsExtra = require('fs-extra')
const { logger } = require('just-task')

module.exports = ({
  fetchUrl,
  extractPath,
  platform = process.platform,
  arch = process.arch
}) => {
  return new Promise((resolve, reject) => {
    logger.info(`Downloading src: ${fetchUrl}`)
    logger.info(`Downloading dest: ${extractPath}`)
    if (fs.existsSync(extractPath)) {
      fs.rmdirSync(extractPath, { recursive: true })
    }
    const temporaryPath = path.join(extractPath, 'temporary')
    download(fetchUrl, temporaryPath, { extract: true, strip: 0 }).then(() => {
      if (platform === 'win32') {
        let binaryDirectory = ''
        let libraryDirectory = ''
        let headersDirectory = ''
        function readDirectories(rootDir, arch) {
          const dirs = fs.readdirSync(rootDir)
          dirs.map(dir => {
            const joinedPath = path.join(rootDir, dir)
            const stats = fs.lstatSync(joinedPath)
            if (stats.isDirectory()) {
              readDirectories(joinedPath, arch)
            } else {
              let matchBin
              let matchLib
              const matchInclude = new RegExp(/nertc.+\.h/g)
              if (arch === 'ia32') {
                matchBin = new RegExp(/x86.+nertc_sdk\.dll/g)
                matchLib = new RegExp(/x86.+nertc_sdk\.lib/g)
              } else if (arch === 'x64') {
                matchBin = new RegExp(/x64.+nertc_sdk\.dll/g)
                matchLib = new RegExp(/x64.+nertc_sdk\.lib/g)
              }
              if (matchBin.test(joinedPath) && binaryDirectory === '') {
                binaryDirectory = rootDir
              }
              if (matchLib.test(joinedPath) && libraryDirectory === '') {
                libraryDirectory = rootDir
              }
              if (matchInclude.test(joinedPath) && headersDirectory === '') {
                headersDirectory = rootDir
              }
            }
          })
        }
        // find bin/lib/api directory automatically
        readDirectories(temporaryPath, arch)
        function copyFiles(src, dst, suffix) {
          if (!fs.existsSync(dst)) {
            fs.mkdirSync(dst)
          }
          const list = fs.readdirSync(src)
          list.map(file => {
            if (file.indexOf(suffix) !== -1) {
              logger.info(`[fetch] Copy file from ${path.join(src, file)} to ${dst}`)
              fs.copyFileSync(path.join(src, file), path.join(dst, file))
            }
          })
        }
        // copy files from temporary to nertc_sdk
        copyFiles(binaryDirectory, path.join(extractPath, 'dll'), '.dll')
        copyFiles(libraryDirectory, path.join(extractPath, 'lib'), '.lib')
        copyFiles(headersDirectory, path.join(extractPath, 'api'), '.h')
      } else if (platform === 'darwin') {
        let frameworkDirectory = ''
        const marchFramework = new RegExp(/.+\.framework|\.driver|\.a|\.sh$/)
        const exceptRegex = new RegExp('/.+sdk\/demo/')
        function readDirectory(rootDir, arch) {
          const dirs = fs.readdirSync(rootDir)
          dirs.map(dir => {
            const newPath = path.join(rootDir, dir)
            const stats = fs.lstatSync(newPath)
            if (marchFramework.test(dir) && !exceptRegex.test(newPath) && frameworkDirectory === '') {
              frameworkDirectory = rootDir
            } else if (stats.isDirectory() && !exceptRegex.test(newPath)) {
              readDirectory(path.join(rootDir, dir), arch)
            }
          })
        }
        readDirectory(temporaryPath, arch)
        logger.info('[fetch] framework directory: ', frameworkDirectory)
        const list = fs.readdirSync(frameworkDirectory)
        list.map(framework => {
          if (marchFramework.test(framework)) {
            const copied = path.join(frameworkDirectory, framework)
            const dst = path.join(extractPath, framework)
            logger.info(`[fetch] copy file: ${copied} to ${dst}`)
            fsExtra.copySync(copied, dst)
          }
        })
      } else {
        return reject(new Error('Unsupported platform.'))
      }
      // remove temporary directory
      fs.rmdirSync(temporaryPath, { recursive: true })
      resolve()
    }).catch(err => {
      console.log(err);
      reject(err)
    })
  })
}
