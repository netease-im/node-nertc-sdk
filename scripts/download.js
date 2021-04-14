const { logger } = require("just-task");
const download = require("download");

module.exports = (url, f) => {
  return new Promise((resolve, reject) => {
    logger.info("start downloading: ", url)
    download(url, f, { filename: "nertc-sdk.zip" }).then(() => {
      logger.info("download complete.")
      resolve(f + "nertc-sdk.zip");
    }).catch(err => {
      reject(err);
    })
  })
}