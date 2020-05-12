
// The main entry point inside a Web Worker

import WebMscore from './index.js'

/** @type {WebMscore} */
let score

/**
 * @typedef {{ id: number; method: Exclude<keyof import('./index').default, 'scoreptr'> | 'load' | 'ready'; params: any[]; }} RPCReq
 * @typedef {{ id: number; result?: any; error?: any; }} RPCRes
 * @param {number} id 
 * @param {any} result 
 * @param {Transferable[]} transfer
 */
const rpcRes = (id, result, transfer = undefined) => {
    /** @type {RPCRes} */
    const res = {
        id,
        result,
    }
    self.postMessage(res, transfer)
}

/**
 * @param {number} id 
 * @param {Error} err 
 */
const rpcErr = (id, err) => {
    /** @type {RPCRes} */
    const res = {
        id,
        error: err.message,
    }
    self.postMessage(res)
}

self.onmessage = async (e) => {
    /** @type {RPCReq} */
    const req = e.data  // JSON-RPC
    const { id, method, params } = req

    try {
        switch (method) {
            case 'ready':
                await WebMscore.ready
                rpcRes(id, 'done')
                break

            case 'load':
                const [name, filedata] = params
                await WebMscore.ready
                score = await WebMscore.load(name, filedata)
                rpcRes(id, 'done')
                break;

            default:
                if (!score) { rpcErr(id, new Error('Score not loaded')) }
                const result = await score[method](...params)
                let transfer
                if (result instanceof Uint8Array) {
                    transfer = [result.buffer]
                }
                rpcRes(id, result, transfer)
        }
    } catch (err) {
        rpcErr(id, err)
    }
}

