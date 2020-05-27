
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
     * @param {'mscz' | 'mscx'} filetype 
     * @param {Uint8Array} data 
     * @param {Uint8Array[]} fonts load extra font files (CJK characters support)
     */
    static async load(filetype, data, fonts = []) {
        await WebMscore.ready

        for (const f of fonts) {
            await this.addFont(f)
        }

        const filetypeptr = getStrPtr(filetype)
        const dataptr = getTypedArrayPtr(data)

        // get the pointer to the MasterScore class instance in C
        const scoreptr = Module.ccall('load',  // name of C function
            'number',  // return type
            ['number', 'number', 'number'],  // argument types
            [filetypeptr, dataptr, data.byteLength]  // arguments
        )

        freePtr(filetypeptr)
        freePtr(dataptr)

        return new WebMscore(scoreptr)
    }

    /**
     * Load (CJK) fonts on demand
     * @private
     * @param {string | Uint8Array} font
     *        * path to the font file in the virtual file system, or
     *        * the font file data
     * @returns {Promise<boolean>} success
     */
    static async addFont(font) {
        if (typeof font !== 'string') {
            const name = '' + Math.random()  // a random name
            // save the font data to the virtual file system
            Module['FS_createDataFile']('/fonts/', name, font, true, true)
            font = '/fonts/' + name
        }

        const fontpathptr = getStrPtr(font)
        const success = Module.ccall('addFont', 'number', ['number'], [fontpathptr])
        freePtr(fontpathptr)
        return !!success
    }

    /**
     * @hideconstructor use `WebMscore.load`
     * @param {number} scoreptr the pointer to the MasterScore class instance in C++
     */
    constructor(scoreptr) {
        /** @private */
        this.scoreptr = scoreptr

        /** @private */
        this.excerptId = -1
    }

    /**
     * Only save this excerpt (linked parts) of the score  
     * 
     * if no excerpts, generate excerpts from existing instrument parts
     * 
     * @param {number} id  `-1` means the full score 
     */
    async setExcerptId(id) {
        this.excerptId = id
    }

    async getExcerptId() {
        return this.excerptId
    }

    /**
     * Generate excerpts from Parts (only parts that are visible) if no existing excerpts
     * @returns {Promise<void>}
     */
    async generateExcerpts() {
        return Module.ccall('generateExcerpts', null, ['number'], [this.scoreptr])
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
     * Get the number of pages in the score (or the excerpt if `excerptId` is set)
     * @returns {Promise<number>}
     */
    async npages() {
        return Module.ccall('npages', 'number', ['number', 'number'], [this.scoreptr, this.excerptId])
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
        const dataptr = Module.ccall('saveXml', 'number', ['number', 'number'], [this.scoreptr, this.excerptId])

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
        const dataptr = Module.ccall('saveMxl', 'number', ['number', 'number'], [this.scoreptr, this.excerptId])
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
            ['number', 'number', 'boolean', 'number'],
            [this.scoreptr, pageNumber, drawPageBackground, this.excerptId]
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
            ['number', 'number', 'boolean', 'boolean', 'number'],
            [this.scoreptr, pageNumber, drawPageBackground, transparent, this.excerptId]
        )
        return readData(dataptr)
    }

    /**
     * Export score as PDF file
     * @returns {Promise<Uint8Array>}
     */
    async savePdf() {
        const dataptr = Module.ccall('savePdf', 'number', ['number', 'number'], [this.scoreptr, this.excerptId])
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
            ['number', 'boolean', 'boolean', 'number'],
            [this.scoreptr, midiExpandRepeats, exportRPNs, this.excerptId]
        )
        return readData(dataptr)
    }

    /**
     * Set the soundfont (sf2/sf3) data
     * @param {Uint8Array} data 
     */
    async setSoundFont(data) {
        if (this.hasSoundfont) {
            // remove the old soundfont file
            Module['FS_createDataFile']('/MuseScore_General.sf3')
        }

        // put the soundfont file into the virtual file system
        // side effects: the soundfont is shared across all instances
        Module['FS_createDataFile']('/', 'MuseScore_General.sf3', data, true, true)

        /** @private */
        this.hasSoundfont = true
    }

    /**
     * Export score as audio file (wav/ogg)
     * @param {'wav' | 'ogg'} type 
     */
    async saveAudio(type) {
        if (!this.hasSoundfont) {
            throw new Error('The soundfont is not set.')
        }

        const filetypeptr = getStrPtr(type)
        const dataptr = Module.ccall('saveAudio',
            'number',
            ['number', 'number', 'number'],
            [this.scoreptr, filetypeptr, this.excerptId]
        )
        freePtr(filetypeptr)
        return readData(dataptr)
    }

    /**
     * Synthesize audio frames
     * @param {number} starttime The start time offset in seconds
     * @returns {Promise<(cancel: boolean) => Promise<{ done: boolean; playtime: number; chunk: Uint8Array; }>>} The iterator function, see `processSynth`
     */
    async synthAudio(starttime) {
        const fn = await this._synthAudio(starttime)
        return (cancel) => {
            return this.processSynth(fn, cancel)
        }
    }

    /**
     * Synthesize audio frames
     * @private
     * @param {number} starttime The start time offset in seconds
     * @returns {Promise<number>} Pointer to the iterator function
     */
    async _synthAudio(starttime = 0) {
        if (!this.hasSoundfont) {
            throw new Error('The soundfont is not set.')
        }

        const iteratorFnPtr = Module.ccall('synthAudio',
            'number',
            ['number', 'number', 'number'],
            [this.scoreptr, starttime, this.excerptId]
        )

        const success = iteratorFnPtr !== 0
        if (!success) {
            throw new Error('synthAudio: Internal Error.')
        }

        return iteratorFnPtr
    }

    /**
     * @private
     * @param {number} fnptr - pointer to the iterator function
     * @param {boolean} cancel - cancel the audio synthesis worklet 
     */
    async processSynth(fnptr, cancel = false) {
        const resptr = Module.ccall('processSynth',
            'number',
            ['number', 'boolean'],
            [fnptr, cancel]
        )

        // struct SynthRes
        const done = Module.getValue(resptr + 0, 'i8')
        const playtime = +Module.getValue(resptr + 4, 'float')  // in seconds
        const chunksize = Module.getValue(resptr + 8, 'i32')
        const chunkptr = Module.getValue(resptr + 12, '*')

        const chunk = new Uint8Array(  // make a copy
            Module.HEAPU8.subarray(chunkptr, chunkptr + chunksize)
        )

        freePtr(resptr)

        return {
            done: !!done,
            playtime,  // The current play time in seconds
            chunk,     // The data chunk of audio frames, interleaved, 512 frames, 44100 Hz (44.1 kHz), 0.0116 s (512/44100)
        }
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
            ['number', 'boolean', 'number'],
            [this.scoreptr, ofSegments, this.excerptId]
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
        Module.ccall('destroy', 'void', ['number'], [this.scoreptr])
        freePtr(this.scoreptr)
    }

}

export default WebMscore
