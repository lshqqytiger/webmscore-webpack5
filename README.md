
# webmscore

> MuseScore's libmscore (the core library) in WebAssembly!  

## Features

* Parse `mscz` file data
* Get score metadata
* Generate music sheets in SVG/PNG/PDF formats
* Generate MIDI
* Export as MusicXML compressed/uncompressed
* Generate position information of measures or segments on the generated sheets
* Run inside a Web Worker thread

## Installation

The package is available on npm: https://www.npmjs.com/package/webmscore

```sh
npm i webmscore
```

## Use webmscore

### Load in browsers

```html
<!-- using a CDN -->
<script src="https://cdn.jsdelivr.net/npm/webmscore/webmscore.js"></script>
<script>
    WebMscore.ready.then(async () => {
        const score = await WebMscore.load('mscz', msczdata)
    })
</script>
```

For latest browsers which support ES Modules

```js
import WebMscore from 'https://cdn.jsdelivr.net/npm/webmscore/webmscore.mjs'
```

### Run in Node.js directly

Minimum version: v8.9.0 with ES Modules support

The `--experimental-modules` flag is required for Node.js versions under 14,  
Also require `"type": "module"` in `package.json`

```js
import WebMscore from 'webmscore'
WebMscore.ready.then(async () => {
    const score = await WebMscore.load('mscz', msczdata)
})
```

### Use a JavaScript bundler

*(TBD)*

### Note: 

**Important!**

Copy `webmscore.lib.data` and `webmscore.lib.wasm` to your artifact dir (the same directory as your final js bundle).

## Compiling

1. Install essential tools like `make`, `cmake`, `llvm`, etc.

2. Install `emscripten` using `emsdk`
https://emscripten.org/docs/getting_started/downloads.html

3. Get and compile Qt5 for WebAssembly

```sh
CPUS=$(getconf _NPROCESSORS_ONLN 2>/dev/null || getconf NPROCESSORS_ONLN 2>/dev/null || 8)

QT_PATH=/usr/qt515/
# If you want to use other directory, make sure you changed `PREFIX_PATH` to your Qt5WASM installation dir in the Makefile

git clone git://code.qt.io/qt/qt5.git --depth=1 -b 5.15.0 $QT_PATH
# or
# download and extract qt-everywhere 5.15.0 (https://download.qt.io/development_releases/qt/5.15/5.15.0-rc/single/ later https://download.qt.io/official_releases/qt/5.15/5.15.0/single/)

cd $QT_PATH
./configure -xplatform wasm-emscripten -nomake examples -prefix $PWD/qtbase
make -j$CPUS
```

4. Checkout submodules

```sh
git submodule init
git submodule update
```

5. Compile `webmscore`

```sh
make release
```

Build artifacts are in the [web-public](./web-public) directory

## Browser Support 

All modern browsers which support [WebAssembly](https://caniuse.com/#feat=wasm) and [Async Functions](https://caniuse.com/#feat=async-functions)

| Name | Minimum Version |
|---|---|
| Chrome | 57 |
| Firefox | 53, 52 (non-ESR) |
| Edge | 16 (Fall Creators Update) |
| Safari | 11 |
| IE | NO! |
| Other browsers | I don't know! |

Only tested on the latest version of Chrome and Firefox.

## Examples

see files in the [web-example](./web-example) directory

```sh
cd ./web-example
npm i
npm start  # Node.js example
npm run start:browser  # browser example
```

---

webmscore is part of the [LibreScore](https://github.com/LibreScore/) project.
