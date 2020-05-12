
const plugins = [
    {
        resolveImportMeta(property) {
            if (property === 'url') {
                return '""';
            }
            return null;
        },
    },
]

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
        plugins,
    },
    {
        input: "src/worker-helper.js",
        output: {
            file: "webmscore.js",
            format: "iife",
            name: 'WebMscore',
            sourcemap: false,
        },
        plugins,
    },
    {
        input: "src/worker-helper.js",
        output: {
            file: "webmscore.mjs",
            format: "esm",
            sourcemap: false,
        }
    }
]
