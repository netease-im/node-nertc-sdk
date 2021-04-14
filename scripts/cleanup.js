const rimraf = require('rimraf');
const {logger} = require('just-task');

module.exports = (dir) => {
  return new Promise((resolve, reject) => {
    logger.info("start clean ", dir);
    rimraf(dir, err => {
      if (err) {
        logger.error(err);
        reject(err)
      }
      logger.info("complete clean ", dir);
      resolve()
    })
  })
}