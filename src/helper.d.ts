export function getStrPtr(str: string): number;
export function getTypedArrayPtr(data: Int8Array | Uint8Array | Uint8ClampedArray | Int16Array | Uint16Array | Int32Array | Uint32Array | Float32Array | Float64Array): number;
export function readData(ptr: number): Uint8Array;
export function freePtr(bufPtr: number): void;
/**
 * this promise is resolved when the runtime is fully initialized
 */
export const RuntimeInitialized: Promise<any>;
export type FileErrorEnum = number;
/**
 * @enum {number}
 * @see libmscore/score.h#L396-L410
 */
export const FileErrorEnum: string[] & {
    FILE_NO_ERROR: number;
    FILE_ERROR: number;
    FILE_NOT_FOUND: number;
    FILE_OPEN_ERROR: number;
    FILE_BAD_FORMAT: number;
    FILE_UNKNOWN_TYPE: number;
    FILE_NO_ROOTFILE: number;
    FILE_TOO_OLD: number;
    FILE_TOO_NEW: number;
    FILE_OLD_300_FORMAT: number;
    FILE_CORRUPTED: number;
    FILE_USER_ABORT: number;
    FILE_IGNORE_ERROR: number;
};
export class FileError extends Error {
    /**
     * @param {FileErrorEnum} errorCode
     */
    constructor(errorCode: number);
    errorCode: number;
    errorName: string;
}
/**
 * get the pointer to a TypedArray, as char*
 */
export type TypedArray = Int8Array | Uint8Array | Uint8ClampedArray | Int16Array | Uint16Array | Int32Array | Uint32Array | Float32Array | Float64Array;
/** @type {Record<string, any>} */
export let Module: Record<string, any>;
