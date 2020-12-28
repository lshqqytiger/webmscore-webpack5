# Changelog

All notable changes to this project will be documented in this file.

## 0.14.3 - 2020-12-28

### Fixed

* SoundFont related bug ([`2b07ee9`](https://github.com/LibreScore/webmscore/commit/2b07ee9095d243e831afc8128914af73a80228d7))

## 0.14.2 - 2020-12-22

### Changed 

* Data files are in `.wasm` file extension for Webpack. This makes web servers compress those files automatically.

## 0.14.1 - 2020-12-22

### Changed 

* WebMscore web worker no longer extends the native `Worker` class

## 0.14.0 - 2020-12-18

### Added

* Support legacy browsers that have no WebAssembly support

## 0.13.4 - 2020-12-18

### Fixed

* ES5 compilation

## 0.13.3 - 2020-12-18

### Added

* ES5 Compatibility (Webpack only)

## 0.13.2 - 2020-12-16

### Fixed

* Handle exceptions sent from the webmscore web worker

## 0.13.1 - 2020-12-16

### Fixed

* Fixed an issue that webmscore catches all (and irrelevant) uncaught exceptions in Node.js

## 0.13.0 - 2020-12-14

### Added

* Allow to set custom `stdout` and `stderr`  
(Node.js exclusive feature)

See the example in [web-public/src/index.js](https://github.com/LibreScore/webmscore/blob/e7a9f3bed0059842e7ca758f66e75ee8b6ccbd1a/web-public/src/index.js#L34-L67)

## 0.12.1 - 2020-12-14

### Fixed

* Throw file loading error instead of failing silently

## 0.12.0 - 2020-12-14

### Added

* Support MP3 export 

```js
await score.saveAudio('mp3')
```

## 0.11.0 - 2020-12-13

### Added

* The `instrumentName` field of parts in Metadata JSON

## 0.10.3 - 2020-08-10

### Added

* Commonjs Compatibility

## 0.10.0 - 2020-08-01

### BREAKING CHANGE

* The return value of `synthAudio`'s iterator function has changed, see [`interface SynthRes` in schemas.ts](https://github.com/LibreScore/webmscore/blob/web/web-public/schemas.ts#L213)

## 0.9.1 - 2020-07-31

no production code change

## 0.9.0 - 2020-07-30

### BREAKING CHANGE

* The `.destory()` method is now having the `soft` parameter (default: `true`) 
    * `true`: destroy the score instance only, or
    * `false`: destroy the whole WebMscore context

    > To retrieve the default `.destory()` behavior of webmscore in WebWorker prior to v0.9.0, set `soft` to `false`

### Fixed

* Don't transfer the ownership of SoundFont data to the webworker context

## 0.8.3 - 2020-07-30

### Added

* Webpack Compatibility

### Changed 

* Use WOFF2 fonts and LZ4 compression to reduce the data file size (significantly)

## 0.8.1 - 2020-07-28

### Added

* TypeScript declaration (`.d.ts`) files

## 0.8.0 - 2020-07-28

### Added

* The `pageSize` (in pixels) field in position JSON (`measurePositions()` or `segmentPositions()`)

## 0.7.0 - 2020-05-31

### Added

* Save part score as MSCZ/MSCX file

* Boost Mode (set the `doLayout` parameter in `WebMscore.load` to `false`) if you only need score metadata or midi file

### Fixed

* Fixed the runtime error in `processSynth`

## 0.6.0 - 2020-05-29

### Added

* Generate audio files in WAV, OGG, or FLAC formats

* Synthesize raw audio frames, can be used in the Web Audio API 

> A soudfont (sf2/sf3) file must be set using `setSoundFont`

### Changed 

* CJK fonts are no longer bundled inside webmscore. If you would like to export sheet PDF/images with those characters, pass an array of font file (ttf/ttc/otf/otc/woff/etc.) data (Uint8Array) to the `fonts` parameter in `WebMscore.load`

### Fixed

* Always load scores in page mode

## 0.5.0 - 2020-05-13

### Added

* Generate excerpts (if no excerpts found) from parts using `await score.generateExcerpts()`

## 0.4.0 - 2020-05-13

### Changed

* The `name` param is replaced by `filetype` (`"mscz"` or `"mscx"`) in `WebMscore.load`

## 0.3.0 - 2020-05-13

### Added 

* Export individual parts (or `excerpt`s, linked parts). Set the excerpt range of exported files using `await score.setExcerptId(n)`.

* Excerpt information is available in score metadata

## 0.2.1 - 2020-05-12

> Changelog of early versions are omitted

## 0.2.0 - 2020-05-11

## 0.1.0 - 2020-05-10

