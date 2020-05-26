
#include "score.h"

namespace Ms {

    struct SynthRes {
        int done;  // bool
        float playtime;  // in seconds
        unsigned chunkSize;
        const char* chunkPtr;
    };

    extern bool saveXml(Score*, QIODevice*);
    extern bool saveXml(Score*, const QString& name);
    extern bool saveMxl(Score*, const QString& name);
    extern bool saveMxl(Score*, QIODevice*);

    bool saveSvg(Score*, QIODevice*, int pageNum = 0, bool drawPageBackground = false);
    bool savePng(Score*, QIODevice*, int pageNum = 0, bool drawPageBackground = false, bool transparent = true);

    bool savePdf(Score* score, QIODevice* device);

    bool saveMidi(Score* score, QIODevice* device, bool midiExpandRepeats, bool exportRPNs);

    bool saveAudio(Score* score, QIODevice *device, std::function<bool(float, float)> updateProgress, float starttime = 0, bool audioNormalize = true);
    bool saveAudio(Score* score, const QString& filename);

    std::function<SynthRes(bool)> synthAudioWorklet(Score* score, float starttime = 0);

    QJsonObject savePositions(Score* score, bool segments);

    QJsonObject saveMetadataJSON(Score* score);
}

