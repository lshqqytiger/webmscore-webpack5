
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
