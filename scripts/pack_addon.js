const path = require('path')
const tar = require('tar')
const fs = require('fs')
const { logger } = require('just-task')

module.exports = ({
  packageName,
  version,
  target,
  platform,
  arch,
  runtime,
  artifactsDirectory = path.join(__dirname, '../build/Release'),
  distDirectory = path.join(__dirname, '../packages')
}) => {
  return new Promise((resolve, reject) => {
    const strippedVersion = target.replace(/^.*?(\d+.+?\d).*/, '$1')
    const artifacts = fs.readdirSync(artifactsDirectory)
    const targetName = `${packageName}-v${version}-${runtime}-v${strippedVersion}-${platform}-${arch}.tar.gz`
    const targetFile = path.join(distDirectory, targetName)
    if (!fs.existsSync(distDirectory)) {
      fs.mkdirSync(distDirectory)
    }
    logger.info(`[package] Tar artifacts as: ${targetName}`)
    tar.create({
      gzip: true,
      file: targetFile,
      cwd: artifactsDirectory,
      filter: (path, stat) => {
        if (path.indexOf('.pdb') !== -1 || path.indexOf('.node') !== -1 ||
            path.indexOf('.dll') !== -1 || path.indexOf('.framework') !== -1) {
          logger.info(`[package] ${path}`)
          return true
        }
      }
    }, artifacts).then(() => {
      resolve()
    }).catch(err => {
      reject(err)
    })
  })
}
