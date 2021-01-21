//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2011 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENSE.GPL
//=============================================================================

#include "libmscore/score.h"
#include "libmscore/measure.h"
#include "libmscore/segment.h"
#include "libmscore/repeatlist.h"
#include "libmscore/system.h"
#include "libmscore/page.h"
// #include "libmscore/xml.h"
#include "mscore/globals.h"
// #include "mscore/preferences.h"
// #include "mscore/musescore.h"



namespace Ms {

//---------------------------------------------------------
//   saveMeasureEvents
//---------------------------------------------------------

void saveMeasureEvents(QJsonArray& jsonEventsArray, Measure* m, int offset, QHash<void*, int>* segs)
{
      for (Segment* s = m->first(SegmentType::ChordRest); s; s = s->next(SegmentType::ChordRest)) {
            int tick = s->tick().ticks() + offset;
            int id = (*segs)[(void*)s];
            int time = lrint(m->score()->repeatList().utick2utime(tick) * 1000);

            QJsonObject jsonEvent;
            jsonEvent.insert("elid", id);
            jsonEvent.insert("position", time);
            jsonEventsArray.append(jsonEvent);

      }
}

//---------------------------------------------------------
//   savePositions
//    All in pixels of the exported SVG/PNG/PDF files
//---------------------------------------------------------

QJsonObject savePositions(Score* score, bool segments)
      {
      QHash<void*, int> segs;

      QJsonObject json;
      int id = 0;

      // qreal ndpi = ((qreal) preferences.getDouble(PREF_EXPORT_PNG_RESOLUTION) / DPI) * 12.0;
      // -> qreal ndpi = ((qreal) DPI / DPI) * 12.0;
      // qreal ndpi = 12.0;
      qreal ndpi = 1.0;
      
      QJsonArray jsonElementsArray;
      if (segments) {
            Measure* m = score->firstMeasureMM();
            for (Segment* s = (m ? m->first(SegmentType::ChordRest) : nullptr);
               s; s = s->next1MM(SegmentType::ChordRest)) {
                  qreal sx   = 0;
                  int tracks = score->nstaves() * VOICES;
                  for (int track = 0; track < tracks; track++) {
                        Element* e = s->element(track);
                        if (e)
                              sx = qMax(sx, e->width());
                        }

                  sx      *= ndpi;
                  int sy   = s->measure()->system()->height() * ndpi;
                  int x    = s->pagePos().x() * ndpi;
                  int y    = s->pagePos().y() * ndpi;

                  Page* p  = s->measure()->system()->page();
                  int page = score->pageIdx(p);

                  QJsonObject jsonElement;
                  jsonElement.insert("id", id);
                  jsonElement.insert("x", x);
                  jsonElement.insert("y", y);
                  jsonElement.insert("sx", sx);
                  jsonElement.insert("sy", sy);
                  jsonElement.insert("page", page);

                  jsonElementsArray.append(jsonElement);

                  segs[(void*)s] = id++;
            }
      }
      else {
            for (Measure* m = score->firstMeasureMM(); m; m = m->nextMeasureMM()) {
                  qreal sx   = m->bbox().width() * ndpi;
                  qreal sy   = m->system()->height() * ndpi;
                  qreal x    = m->pagePos().x() * ndpi;
                  qreal y    = m->system()->pagePos().y() * ndpi;

                  Page* p  = m->system()->page();
                  int page = score->pageIdx(p);

                  QJsonObject jsonElement;
                  jsonElement.insert("id", id);
                  jsonElement.insert("x", x);
                  jsonElement.insert("y", y);
                  jsonElement.insert("sx", sx);
                  jsonElement.insert("sy", sy);
                  jsonElement.insert("page", page);

                  jsonElementsArray.append(jsonElement);

                  segs[(void*)m] = id++;
            }
      }
      json.insert("elements", jsonElementsArray);

      QJsonArray jsonEventsArray;
      score->masterScore()->setExpandRepeats(true);
      for (const RepeatSegment* rs : score->repeatList()) {
            int startTick  = rs->tick;
            int endTick    = startTick + rs->len();
            int tickOffset = rs->utick - rs->tick;
            for (Measure* m = score->tick2measureMM(Fraction::fromTicks(startTick)); m; m = m->nextMeasureMM()) {
                        if (segments)
                              saveMeasureEvents(jsonEventsArray, m, tickOffset, &segs);
                        else {
                              int tick = m->tick().ticks() + tickOffset;
                              int i = segs[(void*)m];
                              int time = lrint(m->score()->repeatList().utick2utime(tick) * 1000);
                              
                              QJsonObject jsonEvent;
                              jsonEvent.insert("elid", i);
                              jsonEvent.insert("position", time);
                              jsonEventsArray.append(jsonEvent);
                        }
                  if (m->endTick().ticks() >= endTick)
                        break;
            }
      }
      json.insert("events", jsonEventsArray);

      // pageSize
      // mscore/file.cpp#L2898 saveSvg
      QRectF r;
      auto page = score->pages().at(0);  // all pages sizes should be the same as the first page
      if (trimMargin >= 0) {
            QMarginsF margins(trimMargin, trimMargin, trimMargin, trimMargin);
            r = page->tbbox() + margins;
      } else {
            r = page->abbox();
      }

      QJsonObject jsonPageSize;
      jsonPageSize.insert("height", r.height() * ndpi); // in px
      jsonPageSize.insert("width", r.width() * ndpi);
      json.insert("pageSize", jsonPageSize);

      return json;
}

}  // namespace Ms
