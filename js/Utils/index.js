"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const deprecate = (replaceApi) => {
    console.warn('This method will be deprecated soon. ', replaceApi ? `Please use ${replaceApi} instead` : '');
};
exports.deprecate = deprecate;
class Config {
    constructor() {
        this.glDebug = false;
    }
    setGlDebug(enable) {
        this.glDebug = enable;
    }
    getGlDebug() {
        return this.glDebug;
    }
}
exports.Config = Config;
const config = new Config();
exports.config = config;
