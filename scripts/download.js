const { logger } = require("just-task");
const download = require("download");
const extract = require('extract-zip')
const path = require("path");
const promisify = require("bluebird").promisify

const extractPromise = promisify(extract)

module.exports = () => {
  return new Promise((resolve, reject) => {
    logger.info(`Downloading...\n`);
    download("http://yx-web.nos.netease.com/package/nertc_sdk.zip", "../", { filename: "nertc-sdk.zip" })
    logger.info("Download finished", "- Start extract to", path.join(__dirname, '../'));
    extractPromise('../nertc-sdk.zip', { dir: path.join(__dirname, '../') })
    logger.info("Extract finished");
    resolve()
  });
};