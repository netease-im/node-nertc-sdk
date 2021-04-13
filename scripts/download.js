const { logger } = require("just-task");
const download = require("download");
const extract = require('extract-zip')
const path = require("path");
const promisify = require("bluebird").promisify
const fs = require("fs-extra")

const extractPromise = promisify(extract)

module.exports = () => {
  return new Promise((resolve, reject) => {
    /** start download */
    const outputDir = "./";
    logger.info(`Downloading...\n`);

    fs.remove(path.join(__dirname, './nertc-sdk')).then(() => {
      return download("http://yx-web.nos.netease.com/package/nertc_sdk.zip", outputDir, {filename: "nertc-sdk.zip"})
    }).then(() => {
      logger.info("Success", "Download finished");
      return extractPromise('./nertc-sdk.zip', {dir: path.join(__dirname, './nertc-sdk/')})
    }).then(() => {
      logger.info("Success", "Prepare finished");
      resolve()
    }).catch(err => {
      logger.error("Failed: ", err);
      reject(new Error(err));
    });
  })
};