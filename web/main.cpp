
#include <emscripten/emscripten.h>

#include <QBuffer>
#include <QtGui>

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
void _init(int argc, char **argv) {
    QApplication* app = new QApplication(argc, argv);

    Ms::MScore::noGui = true;
    Ms::MScore::debugMode = true;
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

    Ms::MasterScore* score = new Ms::MasterScore(Ms::MScore::baseStyle());
    score->setMovements(new Ms::Movements());
    score->setName(_name);

    score->loadMsc(_name, &buffer, true);

    score->doLayout();
    // for (Ms::Score* s : score->scoreList()) {
    //     s->doLayout();
    // }

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

    // MusicXML is plain text
    Ms::saveXml(score, &buffer);

    qDebug("saveXml size %d", buffer.size());

    return QString(buffer.data()).toUtf8();
}

/**
 * export score as compressed MusicXML file
 */
const char* _saveMxl(uintptr_t score_ptr) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    // compressed MusicXML
    Ms::saveMxl(score, &buffer);

    uint32_t size = buffer.size();
    const char* data = buffer.data().data();
    qDebug("saveMxl size %d", size);

    return QByteArray::number(size).append(data);
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
    void init(int argc, char **argv) {
        return _init(argc, argv);
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

    EMSCRIPTEN_KEEPALIVE
    const char* saveMxl(uintptr_t score_ptr) {
        return _saveMxl(score_ptr);
    };

}
