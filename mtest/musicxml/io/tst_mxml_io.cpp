//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2012-2014 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include <QtTest/QtTest>
#include "mtest/testutils.h"
#include "libmscore/score.h"
#include "mscore/preferences.h"
// start includes required for fixupScore()
#include "libmscore/measure.h"
#include "libmscore/staff.h"
#include "libmscore/keysig.h"
// end includes required for fixupScore()

namespace Ms {
extern bool saveMxl(Score*, const QString&);
}

#define DIR QString("musicxml/io/")

using namespace Ms;

//---------------------------------------------------------
//   TestMxmlIO
//---------------------------------------------------------

class TestMxmlIO : public QObject, public MTest
      {
      Q_OBJECT

      void mxmlIoTest(const char* file);
      void mxmlIoTestRef(const char* file);
      void mxmlIoTestRefBreaks(const char* file);
      void mxmlMscxExportTestRef(const char* file);
      void mxmlMscxExportTestRefBreaks(const char* file);
      void mxmlReadTestCompr(const char* file);
      void mxmlReadWriteTestCompr(const char* file);
      void mxmlImportTestRef(const char* file);


      // The list of MusicXML regression tests
      // Currently failing tests are commented out and annotated with the failure reason
      // To extract the list in a shell script use:
      // cat tst_mxml_io.cpp | grep "{ <test>" | awk -F\" '{print $2}'
      // where <test> is mxmlIoTest or mxmlIoTestRef

private slots:
      void initTestCase();

      void accidentals1() { mxmlIoTest("testAccidentals1"); }
      void accidentals2() { mxmlIoTest("testAccidentals2"); }
      void accidentals3() { mxmlIoTest("testAccidentals3"); }
      void arpGliss1() { mxmlIoTest("testArpGliss1"); }
      void arpGliss2() { mxmlIoTest("testArpGliss2"); }
      void arpGliss3() { mxmlIoTest("testArpGliss3"); }
      void barlineFermatas() { mxmlMscxExportTestRef("testBarlineFermatas"); }
      void barStyles() { mxmlIoTest("testBarStyles"); }
      void barStyles2() { mxmlIoTest("testBarStyles2"); }
      void barStyles3() { mxmlIoTest("testBarStyles3"); }
      void breaksImplExpl() { mxmlMscxExportTestRefBreaks("testBreaksImplExpl"); }
      void breaksMMRest() { mxmlMscxExportTestRefBreaks("testBreaksMMRest"); }
      void breaksManual() { mxmlIoTestRefBreaks("testBreaksManual"); }
      void breaksPage() { mxmlMscxExportTestRefBreaks("testBreaksPage"); }
      void breaksSystem() { mxmlMscxExportTestRefBreaks("testBreaksSystem"); }
      void changeTranspose() { mxmlIoTest("testChangeTranspose"); }
      void chordDiagrams1() { mxmlIoTest("testChordDiagrams1"); }
      void chordNoVoice() { mxmlIoTestRef("testChordNoVoice"); }
      void clefs1() { mxmlIoTest("testClefs1"); }
      void clefs2() { mxmlIoTest("testClefs2"); }
      void completeMeasureRests() { mxmlIoTest("testCompleteMeasureRests"); }
      void cueNotes() { mxmlIoTest("testCueNotes"); }
      void cueNotes2() { mxmlMscxExportTestRef("testCueNotes2"); }
      void dalSegno() { mxmlIoTest("testDalSegno"); }
      void dcalCoda() { mxmlIoTest("testDCalCoda"); }
      void dcalFine() { mxmlIoTest("testDCalFine"); }
      void directions1() { mxmlIoTest("testDirections1"); }
      void directions2() { mxmlIoTest("testDirections2"); }
      void divisionsDefinedTooLate1() { mxmlIoTestRef("testDivsDefinedTooLate1"); }
      void divisionsDefinedTooLate2() { mxmlIoTestRef("testDivsDefinedTooLate2"); }
      void doubleClefError() { mxmlIoTestRef("testDoubleClefError"); }
      void drumset1() { mxmlIoTest("testDrumset1"); }
      void drumset2() { mxmlIoTest("testDrumset2"); }
      void durationRoundingError() { mxmlIoTestRef("testDurationRoundingError"); }
      void dynamics1() { mxmlIoTest("testDynamics1"); }
      void dynamics2() { mxmlIoTest("testDynamics2"); }
      void dynamics3() { mxmlIoTestRef("testDynamics3"); }
      void emptyMeasure() { mxmlIoTestRef("testEmptyMeasure"); }
      void emptyVoice1() { mxmlIoTestRef("testEmptyVoice1"); }
      void extendedLyrics() { mxmlIoTestRef("testExtendedLyrics"); }
      void figuredBass1() { mxmlIoTest("testFiguredBass1"); }
      void figuredBass2() { mxmlIoTest("testFiguredBass2"); }
      void figuredBass3() { mxmlIoTest("testFiguredBass3"); }
      void formattedThings() { mxmlIoTest("testFormattedThings"); }
      void fractionMinus() { mxmlIoTestRef("testFractionMinus"); }
      void fractionPlus() { mxmlIoTestRef("testFractionPlus"); }
      void fractionTicks() { mxmlIoTestRef("testFractionTicks"); }
      void grace1() { mxmlIoTest("testGrace1"); }
      void grace2() { mxmlIoTest("testGrace2"); }
      void hairpinDynamics() { mxmlMscxExportTestRef("testHairpinDynamics"); }
      void harmony1() { mxmlIoTest("testHarmony1"); }
      void harmony2() { mxmlIoTest("testHarmony2"); }
      void harmony3() { mxmlIoTest("testHarmony3"); }
      void harmony4() { mxmlIoTest("testHarmony4"); }
      void harmony5() { mxmlIoTest("testHarmony5"); } // chordnames without chordrest
      void harmony6() { mxmlMscxExportTestRef("testHarmony6"); }
      void harmony7() { mxmlMscxExportTestRef("testHarmony7"); }
      void hello() { mxmlIoTest("testHello"); }
      void helloReadCompr() { mxmlReadTestCompr("testHello"); }
      void helloReadWriteCompr() { mxmlReadWriteTestCompr("testHello"); }
      void implicitMeasure1() { mxmlIoTest("testImplicitMeasure1"); }
      void incompleteTuplet() { mxmlIoTestRef("testIncompleteTuplet"); }
      void incorrectStaffNumber1() { mxmlIoTestRef("testIncorrectStaffNumber1"); }
      void incorrectStaffNumber2() { mxmlIoTestRef("testIncorrectStaffNumber2"); }
      void instrumentChangeMIDIportExport() { mxmlMscxExportTestRef("testInstrumentChangeMIDIportExport"); }
      void instrumentSound() { mxmlIoTestRef("testInstrumentSound"); }
      void invalidTimesig() { mxmlIoTestRef("testInvalidTimesig"); }
      void invisibleElements() { mxmlIoTest("testInvisibleElements"); }
      void keysig1() { mxmlIoTest("testKeysig1"); }
      void keysig2() { mxmlIoTest("testKeysig2"); }
      void lines1() { mxmlIoTest("testLines1"); }
      void lines2() { mxmlIoTest("testLines2"); }
      void lines3() { mxmlIoTest("testLines3"); }
      void lines4() { mxmlMscxExportTestRef("testLines4"); }
      void lyricColor() { mxmlIoTest("testLyricColor"); }
      void lyrics1() { mxmlIoTestRef("testLyrics1"); }
      void lyricsVoice2a() { mxmlIoTest("testLyricsVoice2a"); }
      void lyricsVoice2b() { mxmlIoTestRef("testLyricsVoice2b"); }
      void measureLength() { mxmlIoTestRef("testMeasureLength"); }
      void measureStyleSlash() { mxmlImportTestRef("testMeasureStyleSlash"); }
      void midiPortExport() { mxmlMscxExportTestRef("testMidiPortExport"); }
      void multiInstrumentPart1() { mxmlIoTest("testMultiInstrumentPart1"); }
      void multiInstrumentPart2() { mxmlIoTest("testMultiInstrumentPart2"); }
      void multiMeasureRest1() { mxmlIoTestRef("testMultiMeasureRest1"); }
      void multiMeasureRest2() { mxmlIoTestRef("testMultiMeasureRest2"); }
      void multiMeasureRest3() { mxmlIoTestRef("testMultiMeasureRest3"); }
      void multiMeasureRest4() { mxmlIoTestRef("testMultiMeasureRest4"); }
      void multipleNotations() { mxmlIoTestRef("testMultipleNotations"); }
      void nonStandardKeySig1() { mxmlIoTest("testNonStandardKeySig1"); }
      void nonStandardKeySig2() { mxmlIoTest("testNonStandardKeySig2"); }
      void nonStandardKeySig3() { mxmlIoTest("testNonStandardKeySig3"); }
      void nonUniqueThings() { mxmlIoTestRef("testNonUniqueThings"); }
      void noteAttributes1() { mxmlIoTest("testNoteAttributes1"); }
      void noteAttributes2() { mxmlIoTestRef("testNoteAttributes2"); }
      void noteAttributes3() { mxmlIoTest("testNoteAttributes3"); }
      void noteColor() { mxmlIoTest("testNoteColor"); }
      void noteheadParentheses() { mxmlIoTest("testNoteheadParentheses"); }
      void noteheads() { mxmlIoTest("testNoteheads"); }
      void noteheadsFilled() { mxmlIoTest("testNoteheadsFilled"); }
      void notesRests1() { mxmlIoTest("testNotesRests1"); }
      void notesRests2() { mxmlIoTest("testNotesRests2"); }
      void numberedLyrics() { mxmlIoTestRef("testNumberedLyrics"); }
      void overlappingSpanners() { mxmlIoTest("testOverlappingSpanners"); }
      void printSpacingNo() { mxmlIoTestRef("testPrintSpacingNo"); }
      void repeatCounts() { mxmlIoTest("testRepeatCounts"); }
      void repeatSingleMeasure() { mxmlIoTest("testRepeatSingleMeasure"); }
      void restNotations() { mxmlIoTestRef("testRestNotations"); }
      void restsNoType() { mxmlIoTestRef("testRestsNoType"); }
      void restsTypeWhole() { mxmlIoTestRef("testRestsTypeWhole"); }
      void slurTieLineStyle() { mxmlIoTest("testSlurTieLineStyle"); }
      void slurs() { mxmlIoTest("testSlurs"); }
      void slurs2() { mxmlIoTest("testSlurs2"); }
      void sound1() { mxmlIoTestRef("testSound1"); }
      void sound2() { mxmlIoTestRef("testSound2"); }
      void specialCharacters() { mxmlIoTest("testSpecialCharacters"); }
      void staffTwoKeySigs() { mxmlIoTest("testStaffTwoKeySigs"); }
      void stringVoiceName() { mxmlIoTestRef("testStringVoiceName"); }
      void systemBrackets1() { mxmlIoTest("testSystemBrackets1"); }
      void systemBrackets2() { mxmlIoTest("testSystemBrackets2"); }
      void systemBrackets3() { mxmlImportTestRef("testSystemBrackets3"); }
      void tablature1() { mxmlIoTest("testTablature1"); }
      void tablature2() { mxmlIoTest("testTablature2"); }
      void tablature3() { mxmlIoTest("testTablature3"); }
      void tablature4() { mxmlIoTest("testTablature4"); }
      void tablature5() { mxmlIoTestRef("testTablature5"); }
      void tboxAboveBelow1() { mxmlMscxExportTestRef("testTboxAboveBelow1"); }
      void tboxAboveBelow2() { mxmlMscxExportTestRef("testTboxAboveBelow2"); }
      void tboxAboveBelow3() { mxmlMscxExportTestRef("testTboxAboveBelow3"); }
      void tboxMultiPage1() { mxmlMscxExportTestRef("testTboxMultiPage1"); }
      void tboxVbox1() { mxmlMscxExportTestRef("testTboxVbox1"); }
      void tboxWords1() { mxmlMscxExportTestRef("testTboxWords1"); }
      void tempo1() { mxmlIoTest("testTempo1"); }
      void tempo2() { mxmlIoTestRef("testTempo2"); }
      void tempo3() { mxmlIoTestRef("testTempo3"); }
      void tempo4() { mxmlIoTestRef("testTempo4"); }
      void tempoOverlap() { mxmlIoTestRef("testTempoOverlap"); }
      void textLines() { mxmlMscxExportTestRef("testTextLines"); }
      void timesig1() { mxmlIoTest("testTimesig1"); }
      void timesig3() { mxmlIoTest("testTimesig3"); }
      void trackHandling() { mxmlIoTest("testTrackHandling"); }
      void tremolo() { mxmlIoTest("testTremolo"); }
      void tuplets1() { mxmlIoTestRef("testTuplets1"); }
      void tuplets2() { mxmlIoTestRef("testTuplets2"); }
      void tuplets3() { mxmlIoTestRef("testTuplets3"); }
      void tuplets4() { mxmlIoTest("testTuplets4"); }
      void tuplets5() { mxmlIoTestRef("testTuplets5"); }
      void tuplets6() { mxmlIoTestRef("testTuplets6"); }
      void tuplets7() { mxmlIoTest("testTuplets7"); }
      void tuplets8() { mxmlMscxExportTestRef("testTuplets8"); }
      void tuplets9() { mxmlIoTest("testTuplets9"); }
      void twoNoteTremoloTuplet() { mxmlIoTest("testTwoNoteTremoloTuplet"); }
      void uninitializedDivisions() { mxmlIoTestRef("testUninitializedDivisions"); }
      void unusualDurations() { mxmlIoTestRef("testUnusualDurations"); }
      void virtualInstruments() { mxmlIoTestRef("testVirtualInstruments"); }
      void voiceMapper1() { mxmlIoTestRef("testVoiceMapper1"); }
      void voiceMapper2() { mxmlIoTestRef("testVoiceMapper2"); }
      void voiceMapper3() { mxmlIoTestRef("testVoiceMapper3"); }
      void voicePiano1() { mxmlIoTest("testVoicePiano1"); }
      void volta1() { mxmlIoTest("testVolta1"); }
      void wedge1() { mxmlIoTest("testWedge1"); }
      void wedge2() { mxmlIoTest("testWedge2"); }
      void wedge3() { mxmlIoTest("testWedge3"); }
      void words1() { mxmlIoTest("testWords1"); }
      void words2() { mxmlIoTest("testWords2"); }
      };

//---------------------------------------------------------
//   initTestCase
//---------------------------------------------------------

void TestMxmlIO::initTestCase()
      {
      initMTest();
      }

//---------------------------------------------------------
//   fixupScore -- do required fixups after MusicXML import
//   see mscore/file.cpp MuseScore::readScore(Score* score, QString name)
//---------------------------------------------------------

static void fixupScore(Score* score)
      {
      score->connectTies();
      score->masterScore()->rebuildMidiMapping();
      score->setCreated(false);
      score->setSaved(false);
      }

//---------------------------------------------------------
//   mxmlIoTest
//   read a MusicXML file, write to a new file and verify both files are identical
//---------------------------------------------------------

void TestMxmlIO::mxmlIoTest(const char* file)
      {
      MScore::debugMode = true;
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      preferences.setPreference(PREF_IMPORT_MUSICXML_IMPORTBREAKS, true);
      preferences.setPreference(PREF_EXPORT_MUSICXML_EXPORTLAYOUT, false);
      MasterScore* score = readScore(DIR + file + ".xml");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + ".xml"));
      delete score;
      }

//---------------------------------------------------------
//   mxmlIoTestRef
//   read a MusicXML file, write to a new file and verify against reference
//---------------------------------------------------------

void TestMxmlIO::mxmlIoTestRef(const char* file)
      {
      MScore::debugMode = true;
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      preferences.setPreference(PREF_IMPORT_MUSICXML_IMPORTBREAKS, true);
      MasterScore* score = readScore(DIR + file + ".xml");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + "_ref.xml"));
      delete score;
      }

//---------------------------------------------------------
//   mxmlIoTestRefBreaks
//   read a MusicXML file, write to a new file and verify against reference
//   using all possible settings for PREF_EXPORT_MUSICXML_EXPORTBREAKS
//---------------------------------------------------------

void TestMxmlIO::mxmlIoTestRefBreaks(const char* file)
      {
      QSKIP("Tests show different results every time");
      MScore::debugMode = true;
      preferences.setPreference(PREF_IMPORT_MUSICXML_IMPORTBREAKS, true);
      preferences.setPreference(PREF_EXPORT_MUSICXML_EXPORTLAYOUT, false);
      MasterScore* score = readScore(DIR + file + ".xml");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::NO);
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + "_no_ref.xml"));
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + "_manual_ref.xml"));
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::ALL);
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + "_all_ref.xml"));
      delete score;
      }

//---------------------------------------------------------
//   mxmlMscxExportTestRef
//   read a MuseScore mscx file, write to a MusicXML file and verify against reference
//---------------------------------------------------------

void TestMxmlIO::mxmlMscxExportTestRef(const char* file)
      {
      MScore::debugMode = true;
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      preferences.setPreference(PREF_EXPORT_MUSICXML_EXPORTLAYOUT, false);
      MasterScore* score = readScore(DIR + file + ".mscx");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + "_ref.xml"));
      delete score;
      }

//---------------------------------------------------------
//   mxmlMscxExportTestRefBreaks
//   read a MuseScore mscx file, write to a MusicXML file and verify against reference
//   using all possible settings for PREF_EXPORT_MUSICXML_EXPORTBREAKS
//---------------------------------------------------------

void TestMxmlIO::mxmlMscxExportTestRefBreaks(const char* file)
      {
      QSKIP("Tests show different results every time");
      MScore::debugMode = true;
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      preferences.setPreference(PREF_EXPORT_MUSICXML_EXPORTLAYOUT, false);
      MasterScore* score = readScore(DIR + file + ".mscx");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::NO);
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + "_no_ref.xml"));
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + "_manual_ref.xml"));
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::ALL);
      QVERIFY(saveMusicXml(score, QString(file) + ".xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + ".xml", DIR + file + "_all_ref.xml"));
      delete score;
      }

//---------------------------------------------------------
//   mxmlReadTestCompr
//   read a compressed MusicXML file, write to a new file and verify against reference
//---------------------------------------------------------

void TestMxmlIO::mxmlReadTestCompr(const char* file)
      {
      MScore::debugMode = true;
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      preferences.setPreference(PREF_IMPORT_MUSICXML_IMPORTBREAKS, true);
      MasterScore* score = readScore(DIR + file + ".mxl");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      QVERIFY(saveMusicXml(score, QString(file) + "_mxl_read.xml"));
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + "_mxl_read.xml", DIR + file + ".xml"));
      delete score;
      }

//---------------------------------------------------------
//   mxmlReadWriteTestCompr
//   read a MusicXML file, write to a compressed MusicXML file,
//   read the compressed MusicXML file, write to a new file and verify files are identical
//---------------------------------------------------------

void TestMxmlIO::mxmlReadWriteTestCompr(const char* file)
      {
      // read xml
      MScore::debugMode = true;
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      preferences.setPreference(PREF_IMPORT_MUSICXML_IMPORTBREAKS, true);
      MasterScore* score = readScore(DIR + file + ".xml");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      // write mxl
      QVERIFY(saveMxl(score, QString(file) + "_mxl_read_write.mxl"));
      delete score;
      // read mxl
      score = readCreatedScore(QString(file) + "_mxl_read_write.mxl");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      // write and verify
      QVERIFY(saveCompareMusicXmlScore(score, QString(file) + "_mxl_read_write.xml", DIR + file + ".xml"));
      delete score;
      }

//---------------------------------------------------------
//   mxmlImportTestRef
//   read a MusicXML file, write to a new MuseScore mscx file
//   and verify against a MuseScore mscx reference file
//---------------------------------------------------------

void TestMxmlIO::mxmlImportTestRef(const char* file)
      {
      MScore::debugMode = false;
      preferences.setCustomPreference<MusicxmlExportBreaks>(PREF_EXPORT_MUSICXML_EXPORTBREAKS, MusicxmlExportBreaks::MANUAL);
      preferences.setPreference(PREF_EXPORT_MUSICXML_EXPORTLAYOUT, false);
      MasterScore* score = readScore(DIR + file + ".xml");
      QVERIFY(score);
      fixupScore(score);
      score->doLayout();
      QVERIFY(saveCompareScore(score, QString(file) + ".mscx", DIR + file + "_ref.mscx"));
      delete score;
      }

QTEST_MAIN(TestMxmlIO)
#include "tst_mxml_io.moc"
