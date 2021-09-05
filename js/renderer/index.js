"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.CustomRenderer = exports.GlRenderer = exports.SoftwareRenderer = void 0;
const SoftwareRenderer_1 = __importDefault(require("./SoftwareRenderer"));
exports.SoftwareRenderer = SoftwareRenderer_1.default;
const GlRenderer_1 = __importDefault(require("./GlRenderer"));
class GlRenderer {
    constructor() {
        this.self = GlRenderer_1.default.apply(this);
        this.event = this.self.event;
    }
    bind(element) {
        return this.self.bind(element);
    }
    unbind() {
        return this.self.unbind();
    }
    equalsElement(element) {
        return this.self.view === element;
    }
    drawFrame(imageData) {
        return this.self.drawFrame(imageData);
    }
    setContentMode(mode) {
        return this.self.setContentMode(mode);
    }
    refreshCanvas() {
        return this.self.refreshCanvas();
    }
    captureImage() {
        return this.self.captureImage();
    }
}
exports.GlRenderer = GlRenderer;
class CustomRenderer {
    constructor() {
        throw new Error('You have to declare your own custom render');
    }
    bind(element) {
        throw new Error('You have to declare your own custom render');
    }
    unbind() {
        throw new Error('You have to declare your own custom render');
    }
    equalsElement(element) {
        throw new Error('You have to declare your own custom render');
        return false;
    }
    drawFrame(imageData) {
        throw new Error('You have to declare your own custom render');
    }
    setContentMode(mode) {
        throw new Error('You have to declare your own custom render');
    }
    refreshCanvas() {
        throw new Error('You have to declare your own custom render');
    }
    captureImage() {
        throw new Error('You have to declare your own custom render');
    }
}
exports.CustomRenderer = CustomRenderer;
