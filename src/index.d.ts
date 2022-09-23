export default WebMscore;
declare class WebMscore {
    /**
     * This promise is resolved when the runtime is fully initialized
     * @returns {Promise<void>}
     */
    static get ready(): Promise<void>;
    /**
     * The maximum MSCZ/MSCX file format version supported by webmscore
     * @returns {Promise<number>} e.g. `301`
     */
    static version(): Promise<number>;
    /**
     * Set custom stdout instead of `console.log`
     * Available before `WebMscore.ready`
     * @private Node.js exclusive
     * @param {(byte: number) => any} write
     */
    private static set stdout(arg);
    /** @private */
    private static get stdout();
    /**
     * Set custom stderr instead of `console.warn`
     * Available before `WebMscore.ready`
     * @private Node.js exclusive
     * @param {(byte: number) => any} write
     * @example
     * ```js
     * WebMscore['stderr'] = function (byte) {
     *     process.stderr.write(new Uint8Array([byte]))
     * }
     * await WebMscore.ready
     * ```
     */
    private static set stderr(arg);
    /** @private */
    private static get stderr();
    /**
     * Load score data
     * @param {import('../schemas').InputFileFormat} format
     * @param {Uint8Array} data
     * @param {Uint8Array[] | Promise<Uint8Array[]>} fonts load extra font files (CJK characters support)
     * @param {boolean} doLayout set to false if you only need the score metadata or the midi file (Super Fast, 3x faster than the musescore software)
     * @returns {Promise<WebMscore>}
     */
    static load(format: import("../schemas.js").InputFileFormat, data: Uint8Array, fonts?: Uint8Array[] | Promise<Uint8Array[]>, doLayout?: boolean): Promise<WebMscore>;
    /**
     * Load (CJK) fonts on demand
     * @private
     * @param {string | Uint8Array} font
     *        * path to the font file in the virtual file system, or
     *        * the font file data
     * @returns {Promise<boolean>} success
     */
    private static addFont;
    /** @private */
    private static set hasSoundfont(arg);
    /**
     * A soundfont file is loaded
     * @private
     * @type {boolean}
     * @see setSoundFont and saveAudio
     */
    private static get hasSoundfont();
    /**
     * Set the soundfont (sf2/sf3) data
     * (Audio needs soundfonts)
     * @private
     * @param {Uint8Array} data
     * @returns {Promise<void>}
     */
    private static setSoundFont;
    /**
     * @hideconstructor use `WebMscore.load`
     * @param {number} scoreptr the pointer to the MasterScore class instance in C++
     */
    constructor(scoreptr: number);
    /** @private */
    private scoreptr;
    /** @private */
    private excerptId;
    /**
     * Only save this excerpt (linked parts) of the score
     *
     * if no excerpts, generate excerpts from existing instrument parts
     *
     * @param {number} id  `-1` means the full score
     */
    setExcerptId(id: number): Promise<void>;
    getExcerptId(): Promise<number>;
    /**
     * Generate excerpts from Parts (only parts that are visible) if no existing excerpts
     * @returns {Promise<void>}
     */
    generateExcerpts(): Promise<void>;
    /**
     * Get the score title
     * @returns {Promise<string>}
     */
    title(): Promise<string>;
    /**
     * Get the score title (filename safe, replaced some characters)
     */
    titleFilenameSafe(): Promise<string>;
    /**
     * Get the number of pages in the score (or the excerpt if `excerptId` is set)
     * @returns {Promise<number>}
     */
    npages(): Promise<number>;
    /**
     * Get score metadata
     * @returns {Promise<import('../schemas').ScoreMetadata>}
     */
    metadata(): Promise<import("../schemas.js").ScoreMetadata>;
    /**
     * Get the positions of measures
     * @returns {Promise<import('../schemas').Positions>}
     */
    measurePositions(): Promise<import("../schemas.js").Positions>;
    /**
     * Get the positions of segments
     * @returns {Promise<import('../schemas').Positions>}
     */
    segmentPositions(): Promise<import("../schemas.js").Positions>;
    /**
     * Export score as MusicXML file
     * @returns {Promise<string>} contents of the MusicXML file (plain text)
     */
    saveXml(): Promise<string>;
    /**
     * Export score as compressed MusicXML file
     * @returns {Promise<Uint8Array>}
     */
    saveMxl(): Promise<Uint8Array>;
    /**
     * Save part score as MSCZ/MSCX file
     * @param {'mscz' | 'mscx'} format
     * @returns {Promise<Uint8Array>}
     */
    saveMsc(format?: "mscz" | "mscx"): Promise<Uint8Array>;
    /**
     * Export score as the SVG file of one page
     * @param {number} pageNumber integer
     * @param {boolean} drawPageBackground
     * @returns {Promise<string>} contents of the SVG file (plain text)
     */
    saveSvg(pageNumber?: number, drawPageBackground?: boolean): Promise<string>;
    /**
     * Export score as the PNG file of one page
     * @param {number} pageNumber integer
     * @param {boolean} drawPageBackground
     * @param {boolean} transparent
     * @returns {Promise<Uint8Array>}
     */
    savePng(pageNumber?: number, drawPageBackground?: boolean, transparent?: boolean): Promise<Uint8Array>;
    /**
     * Export score as PDF file
     * @returns {Promise<Uint8Array>}
     */
    savePdf(): Promise<Uint8Array>;
    /**
     * Export score as MIDI file
     * @param {boolean} midiExpandRepeats
     * @param {boolean} exportRPNs
     * @returns {Promise<Uint8Array>}
     */
    saveMidi(midiExpandRepeats?: boolean, exportRPNs?: boolean): Promise<Uint8Array>;
    /**
     * Set the soundfont (sf2/sf3) data
     * @param {Uint8Array} data
     */
    setSoundFont(data: Uint8Array): Promise<void>;
    /**
     * Export score as audio file (wav/ogg/flac/mp3)
     * @param {'wav' | 'ogg' | 'flac' | 'mp3'} format
     */
    saveAudio(format: "wav" | "ogg" | "flac" | "mp3"): Promise<Uint8Array>;
    /**
     * Synthesize audio frames
     *
     * `synthAudio` is single instance, i.e. you can't have multiple iterators. If you call `synthAudio` multiple times, it will reset the time offset of all iterators the function returned.
     *
     * @param {number} starttime The start time offset in seconds
     * @returns {Promise<(cancel?: boolean) => Promise<import('../schemas').SynthRes>>} The iterator function, see `processSynth`
     */
    synthAudio(starttime: number): Promise<(cancel?: boolean) => Promise<import("../schemas.js").SynthRes>>;
    /**
     * Synthesize audio frames in bulk
     * @param {number} starttime - The start time offset in seconds
     * @param {number} batchSize - max number of result SynthRes' (n * 512 frames)
     * @returns {Promise<(cancel?: boolean) => Promise<import('../schemas').SynthRes[]>>}
     */
    synthAudioBatch(starttime: number, batchSize: number): Promise<(cancel?: boolean) => Promise<import("../schemas.js").SynthRes[]>>;
    /**
     * Synthesize audio frames
     * @private
     * @todo GC this iterator function
     * @param {number} starttime The start time offset in seconds
     * @returns {Promise<number>} Pointer to the iterator function
     */
    private _synthAudio;
    /**
     * Parse struct SynthRes, then free its memory
     * @private
     * @param {number} resptr - pointer to the SynthRes data
     * @returns {import('../schemas').SynthRes}
     */
    private _parseSynthRes;
    /**
     * @private
     * @param {number} fnptr - pointer to the iterator function
     * @param {boolean} cancel - cancel the audio synthesis worklet
     * @returns {Promise<import('../schemas').SynthRes>}
     */
    private processSynth;
    /**
     * @private
     * @param {number} fnptr - pointer to the iterator function
     * @param {number} batchSize - see `synthAudioBatch`
     * @param {boolean} cancel - cancel the audio synthesis worklet
     */
    private processSynthBatch;
    /**
     * Export positions of measures or segments (if `ofSegments` == true) as JSON
     * @param {boolean} ofSegments
     * @also `score.measurePositions()` and `score.segmentPositions()`
     * @returns {Promise<string>}
     */
    savePositions(ofSegments: boolean): Promise<string>;
    /**
     * Export score metadata as JSON
     * @also `score.metadata()`
     * @returns {Promise<string>} contents of the JSON file
     */
    saveMetadata(): Promise<string>;
    /**
     * @param {boolean=} soft (default `true`)
     *                 * `true`  destroy the score instance only, or
     *                 * `false` destroy the whole WebMscore context
     * @returns {void}
     */
    destroy(soft?: boolean): void;
}
