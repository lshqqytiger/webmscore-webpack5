
#ifndef __IMPORTEXPORTS_H__
#define __IMPORTEXPORTS_H__

#include "score.h"
#include "synthres.h"

namespace Ms {

    using NotesColors = QHash<int /* noteIndex */, QColor>;

    // exports
    extern bool saveXml(Score*, QIODevice*);
    extern bool saveXml(Score*, const QString& name);
    extern bool saveMxl(Score*, const QString& name);
    extern bool saveMxl(Score*, QIODevice*);

    bool saveSvg(Score*, QIODevice*, int pageNum = 0, bool drawPageBackground = false, const NotesColors& notesColors = NotesColors());
    bool savePng(Score*, QIODevice*, int pageNum = 0, bool drawPageBackground = false, bool transparent = true);

    bool savePdf(Score* score, QIODevice* device);

    bool saveMidi(Score* score, QIODevice* device, bool midiExpandRepeats, bool exportRPNs);

    bool saveAudio(Score* score, QIODevice *device, std::function<bool(float, float)> updateProgress, float starttime = 0, bool audioNormalize = true);
    bool saveAudio(Score* score, const QString& filename);

    std::function<SynthRes*(bool)> synthAudioWorklet(Score* score, float starttime = 0);

    QJsonObject savePositions(Score* score, bool segments);

    QJsonObject saveMetadataJSON(Score* score);

    // imports
    // mscore/musescore.h#L973-L982, mscore/file.cpp#L2320 readScore
    extern Score::FileError importMidi(MasterScore*, const QString& name);
    extern Score::FileError importGTP(MasterScore*, const QString& name);
    extern Score::FileError importBww(MasterScore*, const QString& path);
    extern Score::FileError importMusicXml(MasterScore*, const QString&);
    extern Score::FileError importCompressedMusicXml(MasterScore*, const QString&);
    extern Score::FileError importMuseData(MasterScore*, const QString& name);
    extern Score::FileError importLilypond(MasterScore*, const QString& name);
    extern Score::FileError importBB(MasterScore*, const QString& name);
    extern Score::FileError importCapella(MasterScore*, const QString& name);
    extern Score::FileError importCapXml(MasterScore*, const QString& name);
}

#endif
