"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const api_1 = __importDefault(require("./api"));
const NERtcSDK = { NERtcEngine: api_1.default };
if (process.platform === 'win32') {
    // Add the compiled product path to the environment variable,
    // so that the third party does not need to copy the
    // dynamic library separately to the program directory when importing.
    const path = require('path');
    const asarPath = path.join(__dirname, '../../build/Release/');
    const unpackedPath = asarPath.replace('app.asar', 'app.asar.unpacked');
    process.env.PATH = `${unpackedPath};${process.env.PATH}`;
}
exports.default = NERtcSDK;
