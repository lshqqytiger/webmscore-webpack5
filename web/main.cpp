
#include <emscripten/emscripten.h>

#include <QBuffer>
#include <QtGui>

#include "libmscore/exports.h"
#include "libmscore/mscore.h"
#include "libmscore/score.h"

/**
 * helper functions
 */

/**
 * pack length-prefixed data
 */
QByteArray packData(QByteArray data, qint64 size) {
    QByteArray sizeData = QByteArray((const char*)&size, 4);

    QBuffer result;
    result.open(QIODevice::ReadWrite);
    result.write(QByteArray(8, '\0'));  // padding
    result.write(sizeData);
    result.write(data);
    result.close();

    return result.data();
}

/**
 * It's so weird that the first 8 bytes of data would be overwritten by some random things
 * @todo PLEASE HELP - I'm not familiar with emscripten
 */
QByteArray padData(QByteArray data) {
    return QByteArray(8, '\0').append(data);
}

/**
 * the MSCZ/MSCX file format version
 */
int _version() {
    return Ms::MSCVERSION;
}

/**
 * init libmscore
 */
void _init(int argc, char** argv) {
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
    return padData(
        score->title().toUtf8()
    );
}

/**
 * get the number of pages
 */
int _npages(uintptr_t score_ptr) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);
    return score->npages();
}

/**
 * export score as MusicXML file
 */
const char* _saveXml(uintptr_t score_ptr) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    Ms::saveXml(score, &buffer);
    qDebug("saveXml size %lld bytes", buffer.size());

    // MusicXML is plain text
    return padData(
        QString(buffer.data()).toUtf8()
    );
}

/**
 * export score as compressed MusicXML file
 */
const char* _saveMxl(uintptr_t score_ptr) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);

    // compressed MusicXML
    Ms::saveMxl(score, &buffer);

    auto size = buffer.size();
    qDebug("saveMxl size %lld", size);

    return packData(buffer.data(), size);
}

/**
 * export score as SVG
 */
const char* _saveSvg(uintptr_t score_ptr, int pageNumber, bool drawPageBackground) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    score->switchToPageMode(); // not really required, as the default _layoutMode is LayoutMode::PAGE
    Ms::saveSvg(score, &buffer, pageNumber, drawPageBackground);
    qDebug("saveSvg: page index %d, size %lld bytes", pageNumber, buffer.size());

    // SVG is plain text
    return padData(
        QString(buffer.data()).toUtf8()
    );
}

/**
 * export score as PNG
 */
const char* _savePng(uintptr_t score_ptr, int pageNumber, bool drawPageBackground, bool transparent) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    score->switchToPageMode();
    Ms::savePng(score, &buffer, pageNumber, drawPageBackground, transparent);

    auto size = buffer.size();
    qDebug("savePng: page index %d, drawPageBackground %d, transparent %d, size %lld bytes", pageNumber, drawPageBackground, transparent, size);

    return packData(buffer.data(), size);
}

/**
 * export score as PDF
 */
const char* _savePdf(uintptr_t score_ptr) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);

    Ms::savePdf(score, &buffer);

    auto size = buffer.size();
    qDebug("savePdf size %lld", size);

    return packData(buffer.data(), size);
}

/**
 * export score as MIDI
 */
const char* _saveMidi(uintptr_t score_ptr, bool midiExpandRepeats, bool exportRPNs) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);

    Ms::saveMidi(score, &buffer, midiExpandRepeats, exportRPNs);

    auto size = buffer.size();
    qDebug("saveMidi: midiExpandRepeats %d, exportRPNs %d, size %lld", midiExpandRepeats, exportRPNs, size);

    return packData(buffer.data(), size);
}

/**
 * save positions of measures or segments (if the `ofSegments` param == true) as XML
 */
const char* _savePositions(uintptr_t score_ptr, bool ofSegments) {
    Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    score->switchToPageMode();
    Ms::savePositions(score, &buffer, ofSegments);

    qDebug("savePositions: ofSegments %d, size %lld", ofSegments, buffer.size());

    // XML is plain text
    return padData(
        QString(buffer.data()).toUtf8()
    );
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
    void init(int argc, char** argv) {
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
    int npages(uintptr_t score_ptr) {
        return _npages(score_ptr);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* saveXml(uintptr_t score_ptr) {
        return _saveXml(score_ptr);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* saveMxl(uintptr_t score_ptr) {
        return _saveMxl(score_ptr);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* saveSvg(uintptr_t score_ptr, int pageNumber, bool drawPageBackground) {
        return _saveSvg(score_ptr, pageNumber, drawPageBackground);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* savePng(uintptr_t score_ptr, int pageNumber, bool drawPageBackground, bool transparent) {
        return _savePng(score_ptr, pageNumber, drawPageBackground, transparent);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* savePdf(uintptr_t score_ptr) {
        return _savePdf(score_ptr);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* saveMidi(uintptr_t score_ptr, bool midiExpandRepeats, bool exportRPNs) {
        return _saveMidi(score_ptr, midiExpandRepeats, exportRPNs);
    };

    EMSCRIPTEN_KEEPALIVE
    const char* savePositions(uintptr_t score_ptr, bool ofSegments) {
        return _savePositions(score_ptr, ofSegments);
    };

}
