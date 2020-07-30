
const WEBPACK_IMPORT = `
import libWasm from '!!file-loader?name=[name].wasm!./webmscore.lib.wasm-'  // workaround for Webpack 4
import libData from '!!file-loader?name=[name].[ext]!./webmscore.lib.data'
`

const WEBPACK_LOCATE_FILE = `
if (path.endsWith('.wasm')) return libWasm
if (path.endsWith('.data')) return libData
`

const INJECTION_HINT = "// %INJECTION_HINT%"

const REPLACE_IMPORT_META = {
    resolveImportMeta(property) {
        if (property === 'url') {
            return '""';
        }
        return null;
    },
}

const INJECT_WEBPACK_LOCATE_FILE = {
    transform(code) {
        code = code.replace(INJECTION_HINT, WEBPACK_LOCATE_FILE)
        return { code }
    }
}

export default [
    {
        input: "src/worker.js",
        output: {
            file: ".cache/worker.js",
            format: "iife",
            sourcemap: false,
            banner: "export const WebMscoreWorker = function () { ",
            footer: "}\n",
        },
        plugins: [REPLACE_IMPORT_META],
    },
    {
        input: "src/worker-helper.js",
        output: {
            file: "webmscore.js",
            format: "iife",
            name: 'WebMscore',
            sourcemap: false,
        },
        plugins: [REPLACE_IMPORT_META],
    },
    {
        input: "src/worker-helper.js",
        output: {
            file: "webmscore.mjs",
            format: "esm",
            sourcemap: false,
        }
    },
    {
        input: "src/worker-helper.js",
        output: {
            file: "webmscore.webpack.mjs",
            format: "esm",
            banner: WEBPACK_IMPORT,
            sourcemap: false,
        },
        plugins: [REPLACE_IMPORT_META, INJECT_WEBPACK_LOCATE_FILE],
    }
]
