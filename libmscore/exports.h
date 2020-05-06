
#include "score.h"

namespace Ms {

    extern bool saveMxl(Score*, const QString& name);
    extern bool saveMxl(Score*, QIODevice*);
    extern bool saveXml(Score*, QIODevice*);
    extern bool saveXml(Score*, const QString& name);

    bool saveSvg(Score*, QIODevice*, int pageNum = 0, bool drawPageBackground = false);

}

