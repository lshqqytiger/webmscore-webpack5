
#include "score.h"

namespace Ms {

    extern bool saveMxl(Score*, const QString& name);
    extern bool saveMxl(Score*, QIODevice*);
    extern bool saveXml(Score*, QIODevice*);
    extern bool saveXml(Score*, const QString& name);

    bool saveSvg(Score*, QIODevice*, int pageNum = 0, bool drawPageBackground = false);
    bool savePng(Score*, QIODevice*, int pageNum = 0, bool drawPageBackground = false, bool transparent = true);

    bool saveMidi(Score* score, QIODevice* device, bool midiExpandRepeats, bool exportRPNs);
}

