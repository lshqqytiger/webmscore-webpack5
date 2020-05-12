
// @ts-check

import {
    Module,
    RuntimeInitialized,
    getStrPtr,
    getTypedArrayPtr,
    readData,
    freePtr,
} from './helper.js'


class WebMscore {

    /**
     * This promise is resolved when the runtime is fully initialized
     * @returns {Promise<void>}
     */
    static get ready() {
        return RuntimeInitialized
    }

    /**
     * The maximum MSCZ/MSCX file format version supported by webmscore 
     * @returns {Promise<number>} e.g. `301`
     */
    static async version() {
        await WebMscore.ready
        return Module.ccall('version', 'number')
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
        /** @private */
        this.scoreptr = scoreptr
    }

    /**
     * Get the score title
     * @returns {Promise<string>}
     */
    async title() {
        const strptr = Module.ccall('title', 'number', ['number'], [this.scoreptr])
        const str = Module.UTF8ToString(strptr + 8)  // 8 bytes of padding
        freePtr(strptr)
        return str
    }

    /**
     * Get the score title (filename safe, replaced some characters)
     */
    async titleFilenameSafe() {
        const title = await this.title()
        return title.replace(/[\s<>:{}"/\\|?*~.\0\cA-\cZ]+/g, '_')
    }

    /**
     * Get the number of pages in the score
     * @returns {Promise<number>}
     */
    async npages() {
        return Module.ccall('npages', 'number', ['number'], [this.scoreptr])
    }

    /**
     * Get score metadata
     * @returns {Promise<import('../schemas').ScoreMetadata>}
     */
    async metadata() {
        return JSON.parse(await this.saveMetadata())
    }

    /**
     * Get the positions of measures
     * @returns {Promise<import('../schemas').Positions>}
     */
    async measurePositions() {
        return JSON.parse(await this.savePositions(false))
    }

    /**
     * Get the positions of segments
     * @returns {Promise<import('../schemas').Positions>}
     */
    async segmentPositions() {
        return JSON.parse(await this.savePositions(true))
    }

    /**
     * Export score as MusicXML file
     * @returns {Promise<string>} contents of the MusicXML file (plain text)
     */
    async saveXml() {
        const dataptr = Module.ccall('saveXml', 'number', ['number'], [this.scoreptr])

        // MusicXML is plain text
        const data = Module.UTF8ToString(dataptr + 8)  // 8 bytes of padding
        freePtr(dataptr)

        return data
    }

    /**
     * Export score as compressed MusicXML file
     * @returns {Promise<Uint8Array>}
     */
    async saveMxl() {
        const dataptr = Module.ccall('saveMxl', 'number', ['number'], [this.scoreptr])
        return readData(dataptr)
    }

    /**
     * Export score as the SVG file of one page
     * @param {number} pageNumber integer
     * @param {boolean} drawPageBackground 
     * @returns {Promise<string>} contents of the SVG file (plain text)
     */
    async saveSvg(pageNumber = 0, drawPageBackground = false) {
        const dataptr = Module.ccall('saveSvg',
            'number',
            ['number', 'number', 'boolean'],
            [this.scoreptr, pageNumber, drawPageBackground]
        )

        // SVG is plain text
        const data = Module.UTF8ToString(dataptr + 8)  // 8 bytes of padding
        freePtr(dataptr)

        return data
    }

    /**
     * Export score as the PNG file of one page
     * @param {number} pageNumber integer
     * @param {boolean} drawPageBackground 
     * @param {boolean} transparent
     * @returns {Promise<Uint8Array>}
     */
    async savePng(pageNumber = 0, drawPageBackground = false, transparent = true) {
        const dataptr = Module.ccall('savePng',
            'number',
            ['number', 'number', 'boolean', 'boolean'],
            [this.scoreptr, pageNumber, drawPageBackground, transparent]
        )
        return readData(dataptr)
    }

    /**
     * Export score as PDF file
     * @returns {Promise<Uint8Array>}
     */
    async savePdf() {
        const dataptr = Module.ccall('savePdf', 'number', ['number'], [this.scoreptr])
        return readData(dataptr)
    }

    /**
     * Export score as MIDI file
     * @param {boolean} midiExpandRepeats 
     * @param {boolean} exportRPNs 
     * @returns {Promise<Uint8Array>}
     */
    async saveMidi(midiExpandRepeats = true, exportRPNs = true) {
        const dataptr = Module.ccall('saveMidi',
            'number',
            ['number', 'boolean', 'boolean'],
            [this.scoreptr, midiExpandRepeats, exportRPNs]
        )
        return readData(dataptr)
    }

    /**
     * Export positions of measures or segments (if `ofSegments` == true) as JSON
     * @param {boolean} ofSegments
     * @also `score.measurePositions()` and `score.segmentPositions()`
     * @returns {Promise<string>}
     */
    async savePositions(ofSegments) {
        const dataptr = Module.ccall('savePositions',
            'number',
            ['number', 'boolean'],
            [this.scoreptr, ofSegments]
        )

        // JSON is plain text
        const data = Module.UTF8ToString(dataptr + 8)  // 8 bytes of padding
        freePtr(dataptr)

        return data
    }

    /**
     * Export score metadata as JSON
     * @also `score.metadata()`
     * @returns {Promise<string>} contents of the JSON file
     */
    async saveMetadata() {
        const dataptr = Module.ccall('saveMetadata', 'number', ['number'], [this.scoreptr])

        // JSON is plain text
        const data = Module.UTF8ToString(dataptr + 8)  // 8 bytes of padding
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
