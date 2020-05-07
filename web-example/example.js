
import WebMscore from 'webmscore'
import fs from 'fs'

// https://musescore.com/openscore/scores/4074271
// public domain
const name = 'Aequale_No_1.mscz'

const exportedPrefix = 'exported'

const filedata = fs.readFileSync(`./${name}`)

WebMscore.ready.then(async () => {
    console.log('supported file format version:', await WebMscore.version())

    const score = await WebMscore.load(name, filedata)
    console.log(score)
    console.log()

    console.log('score title:', score.title())
    console.log('number of pages:', score.npages())
    console.log()

    fs.writeFileSync(`./${exportedPrefix}.musicxml`, score.saveXml())
    console.log(`generated MusicXML file: ./${exportedPrefix}.musicxml`)

    for (const [index, svg] of score.saveSvgIt(true)) {
        const f = `./${exportedPrefix}-${index}.svg`
        fs.writeFileSync(f, svg)
        console.log(`generated SVG page ${index}: ${f}`)
    }

    fs.writeFileSync(`./${exportedPrefix}.mxl`, score.saveMxl())
    console.log(`generated compressed MusicXML file: ./${exportedPrefix}.mxl`)

    score.destroy()
    console.log('destroyed')
})