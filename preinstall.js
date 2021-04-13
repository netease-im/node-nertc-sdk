const download = require("download");

download("http://yx-web.nos.netease.com/package/nertc_sdk.zip", "./", { filename: "nertc_sdk.zip" }).then(() => {
})
    .catch(err => {
        throw new Error(err);
    });
