
export default {
    input: "index.js",
    output: {
        file: "dist/webmscore.js",
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
}