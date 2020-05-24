
import WebMscore from 'webmscore'
import fs from 'fs'

// https://musescore.com/openscore/scores/4074271
// public domain
const name = 'Aequale_No_1.mscz'

const exportedPrefix = 'exported'

const filedata = fs.readFileSync(`./${name}`)

WebMscore.ready.then(async () => {
    console.log('supported file format version:', await WebMscore.version())

    const score = await WebMscore.load('mscz', filedata, [
        fs.readFileSync('../fonts/SourceHanSansCN-Regular.otf'),  // only contains the CN variation (style) of Chinese characters (the range of GB18030), including traditional and simplified
        fs.readFileSync('../fonts/SourceHanSansKR-Regular.otf'),  // to support hangul syllables
    ])
    console.log(score)
    console.log()

    console.log('score title:', await score.title())
    console.log('number of pages:', await score.npages())
    console.log()

    // await score.generateExcerpts()
    // await score.setExcerptId(2)

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

    await score.setSoundFont(fs.readFileSync('../share/sound/FluidR3Mono_GM.sf3'))
    fs.writeFileSync(`./${exportedPrefix}.ogg`, await score.saveAudio('ogg'))
    console.log(`generated OGG audio: ./${exportedPrefix}.ogg`)

    fs.writeFileSync(`./${exportedPrefix}-mpos.json`, await score.savePositions(false))
    console.log(`exported positions of measures: ./${exportedPrefix}-mpos.json`)

    fs.writeFileSync(`./${exportedPrefix}-spos.json`, await score.savePositions(true))
    console.log(`exported positions of segments: ./${exportedPrefix}.spos.json`)

    const metadata = await score.metadata()
    fs.writeFileSync(`./${exportedPrefix}-metadata.json`, JSON.stringify(metadata, null, 4))
    console.log('score metadata', metadata)

    score.destroy()
    console.log('destroyed')
})