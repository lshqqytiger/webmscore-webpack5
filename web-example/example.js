
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

    console.log('score title:', await score.title())
    console.log('number of pages:', await score.npages())
    console.log()

    fs.writeFileSync(`./${exportedPrefix}.musicxml`, await score.saveXml())
    console.log(`generated MusicXML file: ./${exportedPrefix}.musicxml`)

    const n = await score.npages()
    for (let index = 0; index < n; index++) {
        const f = `./${exportedPrefix}-${index}.svg`
        const svg = await score.saveSvg(index, true)
        fs.writeFileSync(f, svg)
        console.log(`generated SVG page ${index}: ${f}`)
    }

    fs.writeFileSync(`./${exportedPrefix}-2.png`, await score.savePng(2))
    console.log(`generated PNG page 2: ./${exportedPrefix}-2.png`)

    fs.writeFileSync(`./${exportedPrefix}.pdf`, await score.savePdf())
    console.log(`generated PDF file: ./${exportedPrefix}.pdf`)

    fs.writeFileSync(`./${exportedPrefix}.mxl`, await score.saveMxl())
    console.log(`generated compressed MusicXML file: ./${exportedPrefix}.mxl`)

    fs.writeFileSync(`./${exportedPrefix}.mid`, await score.saveMidi())
    console.log(`generated MIDI file: ./${exportedPrefix}.mid`)

    fs.writeFileSync(`./${exportedPrefix}-mpos.json`, await score.savePositions(false))
    console.log(`exported positions of measures: ./${exportedPrefix}-mpos.json`)

    fs.writeFileSync(`./${exportedPrefix}-spos.json`, await score.savePositions(true))
    console.log(`exported positions of segments: ./${exportedPrefix}.spos.json`)

    console.log('score metadata', await score.metadata())

    score.destroy()
    console.log('destroyed')
})