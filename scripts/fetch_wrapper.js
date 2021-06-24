const path = require('path')
const download = require('download')
const tar = require('tar')
const fs = require('fs')
const { logger } = require('just-task')

module.exports = ({
  fetchUrl,
  extractPath
}) => {
  return new Promise((resolve, reject) => {
    logger.info(`Downloading file from: ${fetchUrl} to ${extractPath}`)
    download(fetchUrl, extractPath, {
      strip: 1,
      extract: true
    }).then(() => {
      resolve()
    }).catch(err => {
      reject(err)
    })
  })
}
