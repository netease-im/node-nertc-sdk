/// <reference types="node" />
import SoftwareRenderer from './SoftwareRenderer';
import { EventEmitter } from 'events';
interface IRenderer {
    event: EventEmitter;
    bind(element: Element): void;
    unbind(): void;
    equalsElement(element: Element): boolean;
    drawFrame(imageData: {
        header: any;
        yUint8Array: any;
        uUint8Array: any;
        vUint8Array: any;
    }): void;
    setContentMode(mode: number): void;
    refreshCanvas(): void;
    captureImage(): string;
}
declare class GlRenderer implements IRenderer {
    self: any;
    event: EventEmitter;
    constructor();
    bind(element: Element): void;
    unbind(): void;
    equalsElement(element: Element): boolean;
    drawFrame(imageData: {
        header: any;
        yUint8Array: any;
        uUint8Array: any;
        vUint8Array: any;
    }): void;
    setContentMode(mode: number): void;
    refreshCanvas(): any;
    captureImage(): string;
}
declare class CustomRenderer implements IRenderer {
    constructor();
    event: EventEmitter;
    bind(element: Element): void;
    unbind(): void;
    equalsElement(element: Element): boolean;
    drawFrame(imageData: {
        header: any;
        yUint8Array: any;
        uUint8Array: any;
        vUint8Array: any;
    }): void;
    setContentMode(mode: number): void;
    refreshCanvas(): void;
    captureImage(): string;
}
export { SoftwareRenderer, GlRenderer, IRenderer, CustomRenderer };
