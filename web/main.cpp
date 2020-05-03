
#include <emscripten/emscripten.h>

#include <QBuffer>

#include "libmscore/mscore.h"
#include "libmscore/score.h"
#include "libmscore/exportxml.h"

/**
 * helper functions
 */
//  Ms::MScore::noGui = true;

/**
 * the MSCZ/MSCX file format version
 */
int _version() {
    return Ms::MSCVERSION;
}

/**
 * init libmscore
 */
void _init() {
    Ms::MScore::init();
}

/**
 * load the score data (a MSCZ/MSCX file buffer)
 */
uintptr_t _load(const char* name, const char* data, const uint32_t size) {
    QString _name = QString::fromUtf8(name);

    QBuffer buffer;
    buffer.setData(data, size);
    buffer.open(QIODevice::ReadOnly);

    Ms::MasterScore* score = new Ms::MasterScore();
    score->setMovements(new Ms::Movements());
    score->setStyle(Ms::MScore::baseStyle());
    score->setName(_name);

    score->loadMsc(_name, &buffer, true);

    return reinterpret_cast<uintptr_t>(score);
}

/**
 * get the score title
 */
QByteArray _title(uintptr_t score_ptr) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);
    return score->title().toUtf8();
}

/**
 * export score as MusicXML file
 */
const char* _saveXml(uintptr_t score_ptr) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    // MusicXML is a plain text file
    Ms::saveXml(score, &buffer);

    qDebug("saveXml size %d", buffer.size());

    return QString(buffer.data()).toUtf8();
}

/**
 * export functions (can only be C functions)
 */
extern "C" {

    EMSCRIPTEN_KEEPALIVE
    int version() {
        return _version();
    };

    EMSCRIPTEN_KEEPALIVE
    void init() {
        return _init();
    };

    EMSCRIPTEN_KEEPALIVE
    uintptr_t load(const char* name, const char* data, const uint32_t size) {
        return _load(name, data, size);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* title(uintptr_t score_ptr) {
        return _title(score_ptr);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* saveXml(uintptr_t score_ptr) {
        return _saveXml(score_ptr);
    };

}
