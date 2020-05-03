
#include <emscripten/emscripten.h>

#include <QBuffer>

#include "libmscore/mscore.h"
#include "libmscore/score.h"

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
 * load the score data (a MSCZ/MSCX file buffer)
 */
uintptr_t _load(const char* name, const char* data, const uint32_t size) {
    QString _name = QString::fromUtf8(name);

    QBuffer buffer;
    buffer.setData(data, size);
    buffer.open(QIODevice::ReadOnly);

    Ms::MasterScore* score = new Ms::MasterScore();
    score->setName(_name);
    score->loadMsc(_name, &buffer, true);

    return reinterpret_cast<uintptr_t>(score);
}

// std::string getTitle(std::uintptr_t score_ptr) {
//     // Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);
//     // return score->title().toStdString();
// }

// void destroy(std::uintptr_t score_ptr) {
//     // Ms::MasterScore* score = reinterpret_cast<Ms::MasterScore*>(score_ptr);
//     // score->~MasterScore();
//     // delete &score;
// }

// emscripten::val getBytes() {
//     return emscripten::val(
//         emscripten::typed_memory_view(bufferLength, byteBuffer));
// }

/**
 * export functions (can only be C functions)
 */
extern "C" {

    EMSCRIPTEN_KEEPALIVE
    int version() {
        return _version();
    };

    EMSCRIPTEN_KEEPALIVE
    uintptr_t load(const char* name, const char* data, const uint32_t size) {
        return _load(name, data, size);
    };

}
