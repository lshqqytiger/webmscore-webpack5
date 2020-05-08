
#include "mscore/exportmidi.h"
#include "shape.h"
#include "xml.h"
#include "element.h"
#include "note.h"
#include "rest.h"
#include "sig.h"
#include "clef.h"
#include "key.h"
#include "score.h"
#include "page.h"
#include "dynamic.h"
#include "style.h"
#include "tempo.h"
#include "select.h"
#include "staff.h"
#include "part.h"
#include "utils.h"
#include "barline.h"
#include "slur.h"
#include "hairpin.h"
#include "ottava.h"
#include "textline.h"
#include "pedal.h"
#include "trill.h"
#include "volta.h"
#include "timesig.h"
#include "box.h"
#include "excerpt.h"
#include "system.h"
#include "tuplet.h"
#include "keysig.h"
#include "measure.h"
#include "undo.h"
#include "repeatlist.h"
#include "beam.h"
#include "stafftype.h"
#include "revisions.h"
#include "lyrics.h"
#include "segment.h"
#include "tempotext.h"
#include "sym.h"
#include "image.h"
#include "stafflines.h"
#include "mscore/svggenerator.h"

#include "chordlist.h"
#include "mscore.h"
#include "exports.h"

namespace Ms {

int trimMargin = -1;

//---------------------------------------------------------
//   paintElement(s)
//---------------------------------------------------------

void paintElement(QPainter& p, const Element* e)
      {
      QPointF pos(e->pagePos());
      p.translate(pos);
      e->draw(&p);
      p.translate(-pos);
      }

void paintElements(QPainter& p, const QList<Element*>& el)
      {
      for (Element* e : el) {
            if (!e->visible())
                  continue;
            paintElement(p, e);
            }
      }

//---------------------------------------------------------
//   MuseScore::saveSvg
///  Save a single page
//---------------------------------------------------------
bool saveSvg(Score* score, QIODevice* device, int pageNumber, bool drawPageBackground) 
    {
    QString title(score->title());
    score->setPrinting(true);
    MScore::pdfPrinting = true;
    MScore::svgPrinting = true;
    const QList<Page*>& pl = score->pages();
    int pages = pl.size();
    double pr = MScore::pixelRatio;

    Page* page = pl.at(pageNumber);
    SvgGenerator printer;
    printer.setTitle(pages > 1 ? QString("%1 (%2)").arg(title).arg(pageNumber + 1) : title);
    printer.setOutputDevice(device);

    QRectF r;
    if (trimMargin >= 0) {
        QMarginsF margins(trimMargin, trimMargin, trimMargin, trimMargin);
        r = page->tbbox() + margins;
    } else
        r = page->abbox();
    qreal w = r.width();
    qreal h = r.height();
    printer.setSize(QSize(w, h));
    printer.setViewBox(QRectF(0, 0, w, h));
    QPainter p(&printer);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    if (trimMargin >= 0 && score->npages() == 1)
        p.translate(-r.topLeft());
    MScore::pixelRatio = DPI / printer.logicalDpiX();
    if (trimMargin >= 0)
        p.translate(-r.topLeft());

    if (drawPageBackground)
        p.fillRect(r, Qt::white);

    // 1st pass: StaffLines
    for (System* s : page->systems()) {
        for (int i = 0, n = s->staves()->size(); i < n; i++) {
            if (score->staff(i)->invisible() || !score->staff(i)->show())
                continue;  // ignore invisible staves
            if (s->staves()->isEmpty() || !s->staff(i)->show())
                continue;
            Measure* fm = s->firstMeasure();
            if (!fm)  // only boxes, hence no staff lines
                continue;

            // The goal here is to draw SVG staff lines more efficiently.
            // MuseScore draws staff lines by measure, but for SVG they can
            // generally be drawn once for each system. This makes a big
            // difference for scores that scroll horizontally on a single
            // page. But there are exceptions to this rule:
            //
            //   ~ One (or more) invisible measure(s) in a system/staff ~
            //   ~ One (or more) elements of type HBOX or VBOX          ~
            //
            // In these cases the SVG staff lines for the system/staff
            // are drawn by measure.
            //
            bool byMeasure = false;
            for (MeasureBase* mb = fm; mb; mb = s->nextMeasure(mb)) {
                if (!mb->isMeasure() || !toMeasure(mb)->visible(i)) {
                    byMeasure = true;
                    break;
                }
            }
            if (byMeasure) {  // Draw visible staff lines by measure
                for (MeasureBase* mb = fm; mb; mb = s->nextMeasure(mb)) {
                    if (mb->isMeasure() && toMeasure(mb)->visible(i)) {
                        StaffLines* sl = toMeasure(mb)->staffLines(i);
                        printer.setElement(sl);
                        paintElement(p, sl);
                    }
                }
            } else {  // Draw staff lines once per system
                StaffLines* firstSL = s->firstMeasure()->staffLines(i)->clone();
                StaffLines* lastSL = s->lastMeasure()->staffLines(i);

                qreal lastX = lastSL->bbox().right() + lastSL->pagePos().x() - firstSL->pagePos().x();
                QVector<QLineF>& lines = firstSL->getLines();
                for (int l = 0, c = lines.size(); l < c; l++)
                    lines[l].setP2(QPointF(lastX, lines[l].p2().y()));

                printer.setElement(firstSL);
                paintElement(p, firstSL);
            }
        }
    }
    // 2nd pass: the rest of the elements
    QList<Element*> pel = page->elements();
    qStableSort(pel.begin(), pel.end(), elementLessThan);
    ElementType eType;
    for (const Element* e : pel) {
        // Always exclude invisible elements
        if (!e->visible())
            continue;

        eType = e->type();
        switch (eType) {                    // In future sub-type code, this switch() grows, and eType gets used
            case ElementType::STAFF_LINES:  // Handled in the 1st pass above
                continue;                   // Exclude from 2nd pass
                break;
            default:
                break;
        }  // switch(eType)

        // Set the Element pointer inside SvgGenerator/SvgPaintEngine
        printer.setElement(e);

        // Paint it
        paintElement(p, e);
    }
    p.end();  // Writes MuseScore SVG file to disk, finally

    // Clean up and return
    MScore::pixelRatio = pr;
    score->setPrinting(false);
    MScore::pdfPrinting = false;
    MScore::svgPrinting = false;
    return true;
}

//---------------------------------------------------------
//   saveMidi
//---------------------------------------------------------
bool saveMidi(Score* score, QIODevice* device, bool midiExpandRepeats, bool exportRPNs)
    {
        ExportMidi em(score);
        return em.write(device, midiExpandRepeats, exportRPNs);
    }

}  // namespace Ms
