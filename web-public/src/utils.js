
export const IS_NODE = typeof process === 'object' && typeof process.versions === 'object' && typeof process.versions.node === 'string'

export const getSelfURL = () => {
    let url = import.meta.url  // transforms to "" in the generated bundle
    if (!url) {
        if (typeof document !== 'undefined') {
            url = document.currentScript && document.currentScript.src || document.baseURI
        } else if (typeof location !== 'undefined') {
            url = location.href
        }
    }
    return url
}
