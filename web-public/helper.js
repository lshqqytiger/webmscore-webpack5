
import LibMscore from './webmscore.js'

const Module = LibMscore()
export { Module }

/**
 * stdout
 */
Module['print'] = console.log
/**
 * stderr
 */
Module['printErr'] = console.error
/**
 * log on fatal error
 */
Module['onAbort'] = console.error


/**
 * get the pointer to a js string, as utf-8 encoded char*
 * @param {string} str 
 * @returns {number}
 */
export const getStrPtr = (str) => {
    const maxSize = str.length * 4 + 1
    const buf = Module._malloc(maxSize)
    Module.stringToUTF8(str, buf, maxSize)
    return buf
}

/**
 * get the pointer to a TypedArray, as char*
 * @typedef {Int8Array | Int16Array | Int32Array | Uint8Array | Uint16Array | Uint32Array | Uint8ClampedArray | Float32Array | Float64Array} TypedArray
 * @param {TypedArray} data 
 * @returns {number}
 */
export const getTypedArrayPtr = (data) => {
    const size = data.length * data.BYTES_PER_ELEMENT
    const buf = Module._malloc(size)
    Module.HEAPU8.set(data, buf)
    return buf
}

/**
 * read length-prefixed data (char*) as Uint8Array
 * @param {number} ptr 
 * @returns {Uint8Array}
 */
export const readData = (ptr) => {
    let offset = ptr + 8  // 8 bytes padding

    const sizeData = new DataView(
        new Uint8Array(  // make a copy
            Module.HEAPU8.subarray(offset, offset + 4)
        ).buffer
    )

    const size = sizeData.getUint32(0, true)
    const data = new Uint8Array(Module.HEAPU8.subarray(offset + 4, offset + 4 + size))  // make a copy

    freePtr(ptr)

    return data
}

/**
 * free a pointer
 * @param {number} bufPtr 
 */
export const freePtr = (bufPtr) => {
    Module._free(bufPtr)
}

/**
 * this promise is resolved when the runtime is fully initialized
 */
export const RuntimeInitialized = new Promise((resolve) => {
    Module.onRuntimeInitialized = () => {
        Module.ccall('init')  // init libmscore
        resolve()
    }
})
