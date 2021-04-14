const { logger } = require("just-task");
const extract = require('extract-zip')
const path = require("path");
const promisify = require("bluebird").promisify

const extractPromise = promisify(extract)

module.exports = (src, to) => {
  return new Promise((resolve, reject) => {
    logger.info("start extract.")
    extract(src, { dir: path.join(__dirname, to) })
    logger.info("extract complete")
    resolve();
    // extractPromise(src, { dir: path.join(__dirname, to) }).then(() => {
    //   logger.info("extract complete")
    //   resolve();
    // }).catch(err => {
    //   reject(err);
    // })
  })
};