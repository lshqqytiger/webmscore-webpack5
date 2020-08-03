
#ifndef __SYNTHRES_H__
#define __SYNTHRES_H__

namespace Ms {
    struct SynthRes {
        int done;  // bool
        float startTime; // the chunk's start time in seconds
        float endTime;   // the chunk's end time in seconds (playtime)
        unsigned chunkSize;
        const char* chunkPtr;
    };
}

#endif
