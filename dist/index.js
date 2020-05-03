// @ts-check

const {
    Module,
    RuntimeInitialized,
    getStrPtr,
    getTypedArrayPtr,
    freePtr,
} = require('./helper')


class WebMscore {

    /**
     * This promise is resolved when the runtime is fully initialized
     * @returns {Promise<void>}
     */
    static get ready() {
        return RuntimeInitialized
    }

    /**
     * The MSCZ/MSCX file format version  (e.g. `301`)
     * @returns {Promise<number>}
     */
    static async version() {
        await WebMscore.ready
        return Module.ccall('version', null, ['number'])
    }

    /**
     * Load the score data (from a MSCZ/MSCX file)
     * @param {string} filename 
     * @param {Uint8Array} data 
     */
    static async load(filename, data) {
        await WebMscore.ready

        const nameptr = getStrPtr(filename)
        const dataptr = getTypedArrayPtr(data)

        // get the pointer to the MasterScore class instance in C
        const scoreptr = Module.ccall('load',  // name of C function
            'number',  // return type
            ['number', 'number', 'number'],  // argument types
            [nameptr, dataptr, data.byteLength]  // arguments
        )

        freePtr(nameptr)
        freePtr(dataptr)

        return scoreptr
    }

    /**
     * @hideconstructor use `WebMscore.load`
     */
    constructor() {

    }


}

module.exports = WebMscore
