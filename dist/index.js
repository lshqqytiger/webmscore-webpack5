
import {
    Module,
    RuntimeInitialized,
    getStrPtr,
    getTypedArrayPtr,
    freePtr,
} from './helper.js'


export class WebMscore {

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

        return new WebMscore(scoreptr)
    }

    /**
     * @hideconstructor use `WebMscore.load`
     * @param {number} scoreptr the pointer to the MasterScore class instance in C++
     */
    constructor(scoreptr) {
        this.scoreptr = scoreptr
    }

    /**
     * Get the score title
     * @returns {string}
     */
    title() {
        const strptr = Module.ccall('title', 'number', ['number'], [this.scoreptr])
        const str = Module.UTF8ToString(strptr)
        freePtr(strptr)
        return str
    }

    /**
     * Export score as MusicXML file
     * @returns {string}
     */
    saveXml() {
        const dataptr = Module.ccall('saveXml', 'number', ['number'], [this.scoreptr])

        // MusicXML is plain text
        const data = Module.UTF8ToString(dataptr)
        freePtr(dataptr)

        return data
    }

    /**
     * Export score as SVG file
     * @param {number} pageNumber integer
     * @param {boolean} drawPageBackground 
     */
    saveSvg(pageNumber = 0, drawPageBackground = false) {
        const dataptr = Module.ccall('saveSvg',
            'number',
            ['number', 'number', 'boolean'],
            [this.scoreptr, pageNumber, drawPageBackground]
        )

        // SVG is plain text
        const data = Module.UTF8ToString(dataptr)
        freePtr(dataptr)

        return data
    }

    /**
     * @returns {void}
     */
    destroy() {
        freePtr(this.scoreptr)
    }

}

export default WebMscore
