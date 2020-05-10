
export default [
    {
        input: "src/index.js",
        output: {
            file: "webmscore.js",
            format: "iife",
            name: 'WebMscore',
            exports: 'default',
            sourcemap: false,
        },
        plugins: [
            {
                resolveImportMeta(property) {
                    if (property === 'url') {
                        return '""';
                    }
                    return null;
                },
            },
        ],
    },
    {
        input: "src/index.js",
        output: {
            file: "webmscore.mjs",
            format: "esm",
            sourcemap: false,
        }
    }
]
