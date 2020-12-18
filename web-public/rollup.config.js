
import babel from '@rollup/plugin-babel'

const WEBPACK_IMPORT = `
import libWasm from '!!file-loader?name=[name].wasm!./webmscore.lib.wasm-'  // workaround for Webpack 4
import libData from '!!file-loader?name=[name].[ext]!./webmscore.lib.data'
`

const WEBPACK_LOCATE_FILE = `
if (path.endsWith('.wasm')) return new URL(MSCORE_LIB_WASM, MSCORE_BASEURL).href
if (path.endsWith('.data')) return new URL(MSCORE_LIB_DATA, MSCORE_BASEURL).href
`

const WEBPACK_WORKER_IMPORT = '+ `var MSCORE_LIB_WASM = "${libWasm}", MSCORE_LIB_DATA = "${libData}", MSCORE_BASEURL = "${document.baseURI}";`'

const INJECTION_HINT = (n) => `// %INJECTION_HINT_${n}%`

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
        code = code.replace(INJECTION_HINT(0), WEBPACK_LOCATE_FILE)
        code = code.replace(INJECTION_HINT(1), WEBPACK_WORKER_IMPORT)
        return { code }
    }
}

const BABEL_PLUGIN = babel({
    babelHelpers: 'bundled',
    babelrc: false,
    compact: true,
    comments: false,
    presets: [[
        '@babel/preset-env',
        { targets: { browsers: "> 0.5%, ie >= 11" } },
    ]],
})

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
        input: "src/nodejs.js",
        output: {
            file: "webmscore.nodejs.cjs",
            format: "cjs",
            exports: "default",
            sourcemap: false,
        },
        plugins: [REPLACE_IMPORT_META],
    },
    {
        input: "src/worker-helper.js",
        output: {
            file: "webmscore.webpack.mjs",
            format: "esm",
            banner: WEBPACK_IMPORT,
            sourcemap: false,
        },
        plugins: [
            REPLACE_IMPORT_META,
            INJECT_WEBPACK_LOCATE_FILE,
            BABEL_PLUGIN,
        ],
    }
]
