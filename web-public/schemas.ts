
type BoolString = 'true' | 'false'

interface ScorePartData {
    name: string;

    /**
     * MIDI Program Number
     */
    program: number;

    /**
     * @see share/instruments/instruments.xml
     */
    instrumentId: string;

    lyricCount: number;
    harmonyCount: number;

    hasPitchedStaff: BoolString;
    hasTabStaff: BoolString;
    hasDrumStaff: BoolString;
    isVisible: BoolString;
}

interface ScoreTextFramesData {
    titles: string[];
    subtitles: string[];
    composers: string[];
    poets: string[];
}

interface ScorePageFormat {
    height: number;
    width: number;
    twosided: BoolString;
}

interface ScoreExcerptData {
    /**
     * excerpt id
     */
    id: number;

    /**
     * title of the excerpt
     */
    title: string;

    /**
     * linked parts in the excerpt
     */
    parts: ScorePartData[];
}

/**
 * The score metadata
 */
export interface ScoreMetadata {
    title: string;
    subtitle: string;

    composer: string;

    /** The poet/lyricist of the score */
    poet: string;

    /**
     * The current (lib)mscore version being used
     */
    mscoreVersion: string;

    /**
     * The MSCZ/MSCX file format version
     */
    fileVersion: number;

    /**
     * Number of pages
     */
    pages: number;

    /**
     * Number of measures
     */
    measures: number;

    hasLyrics: BoolString;
    hasHarmonies: BoolString;

    /**
     * @todo explanations
     */
    keysig: number;

    /**
     * @todo explanations
     */
    timesig: string;

    /**
     * The score duration in seconds
     */
    duration: number;

    lyrics: string;

    /**
     * tempo in quarter notes (crochets) per second
     */
    tempo: number;
    /**
     * tempo marker which determines the midi tempo.
     */
    tempoText: string;

    /**
     * excerpts (linked parts) 
     */
    excerpts: ScoreExcerptData[];

    parts: ScorePartData[];

    /**
     * page format of the sheet
     */
    pageFormat: ScorePageFormat;

    /**
     * text frames metadata
     */
    textFramesData: ScoreTextFramesData;
}

interface PositionElement {
    /**
     * element index
     */
    id: number;

    /**
     * The x coordinate (the top-left corner of the page to the top-left corner of the element)
     */
    x: number;
    /**
     * The x coordinate (the top-left corner of the page to the top-left corner of the element)
     */
    y: number;

    /**
     * The width of the element
     */
    sx: number;
    /**
     * The height of the element
     */
    sy: number;

    /**
     * The page index (zero-based) where the measure or segment presents
     */
    page: number;
}

interface PositionEvent {
    /**
     * The element index corresponding to the envent
     */
    elid: number;

    /**
     * The time position (in ms) of the element (measure or segment) in the exported audio
     */
    position: number;
}

/**
 * The position information of measures or segments
 */
export interface Positions {
    /**
     * The position information of each element (measure or segment) on the sheet,  
     * in pixels of the exported SVG/PNG/PDF file
     * 
     * the `space` property in space.jsonp
     */
    elements: PositionElement[];

    /**
     * The time position/offset (in ms) of each element in the exported audio
     * 
     * the `time` property in space.jsonp
     */
    events: PositionEvent[];
}
