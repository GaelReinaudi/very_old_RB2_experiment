TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

GIT_VERSION = $$system(git --git-dir $$PWD/core/.git --work-tree $$PWD describe --always --tags)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
DEFINES += USE_SPDLOG

DEBUGRELEASE = what
CONFIG(debug, debug|release) {
DEBUGRELEASE = debug
}
else {
DEBUGRELEASE = release
}

COMMON_DIR = $$PWD

BINDIR = $${COMMON_DIR}/../bin/$${DEBUGRELEASE}
LIBDIR = $${COMMON_DIR}/../lib/$${DEBUGRELEASE}
DESTDIR = $${BINDIR}

LIBS += -L$${BINDIR}
LIBS += -L$${LIBDIR}

MOC_DIR = $$PWD/../build/$${TARGET}/$${DEBUGRELEASE}
OBJECTS_DIR = $$PWD/../build/$${TARGET}/$${DEBUGRELEASE}
UI_DIR = $$PWD/../build/$${TARGET}/$${DEBUGRELEASE}

#unix|win32: LIBS += -ldbghelp

win32 {
QMAKE_CXXFLAGS += -D_WIN32_WINNT=0x600
QMAKE_CXXFLAGS += /bigobj
}
#QMAKE_CXXFLAGS += -fpermissive

DEFINES += CROSS_COMPILER_IF
LIBS += \
    -L$$PWD/lib \
    -L$${BINDIR} \
    -L"."

SRC_FOLDER = $$PWD

SOURCES += main.cpp \
$$SRC_FOLDER/_DefinitionSequenceMainDialog.cpp \
$$SRC_FOLDER/ControlesDuMainDialog.h \
DAQmxTasksAndCodes.cpp \
$$SRC_FOLDER/InputDialog.cpp \
$$SRC_FOLDER/MainDialog.cpp \
$$SRC_FOLDER/MainFrm.cpp \
$$SRC_FOLDER/PortDialog.cpp \
$$SRC_FOLDER/Rb2Manip.cpp \
$$SRC_FOLDER/Rb2View.cpp \
$$SRC_FOLDER/SeqViewDialog.cpp \
$$SRC_FOLDER/stdafx.cpp \
$$SRC_FOLDER/VariationDialog.cpp \
$$SRC_FOLDER/FolderUtils.cpp \
$$SRC_FOLDER/nrutil.cpp \
$$SRC_FOLDER/OriginClient.cpp \
$$SRC_FOLDER/PrntScreen.cpp \
$$SRC_FOLDER/Rb2Doc.cpp \
$$SRC_FOLDER/serialport.cpp \
$$SRC_FOLDER/SpeakWithOriginDlg.cpp \
$$SRC_FOLDER/Colonne.cpp \
$$SRC_FOLDER/DialogCurve.cpp \
$$SRC_FOLDER/DialogFilmColonnes.cpp \
$$SRC_FOLDER/DialogParam.cpp \
$$SRC_FOLDER/DialogSetValue.cpp \
$$SRC_FOLDER/DialogVariation.cpp \
$$SRC_FOLDER/DialogWaveForm.cpp \
$$SRC_FOLDER/Parametre.cpp \
$$SRC_FOLDER/ProDialog.cpp \
$$SRC_FOLDER/RelationAnalog.cpp \
$$SRC_FOLDER/Sequence.cpp \
$$SRC_FOLDER/SettingSave.cpp \
$$SRC_FOLDER/Variation.cpp \
$$SRC_FOLDER/WaveForme.cpp \


HEADERS += \
$$SRC_FOLDER/CamDialog.h \
$$SRC_FOLDER/DAQmxTasksAndCodes.h \
$$SRC_FOLDER/FitDialog.h \
$$SRC_FOLDER/GeneticDialog.h \
$$SRC_FOLDER/InputDialog.h \
$$SRC_FOLDER/LaserDialog.h \
$$SRC_FOLDER/MainDialog.h \
$$SRC_FOLDER/MainFrm.h \
$$SRC_FOLDER/NiIncludes.h \
$$SRC_FOLDER/PortDialog.h \
$$SRC_FOLDER/Rb2Manip.h \
$$SRC_FOLDER/Rb2View.h \
$$SRC_FOLDER/Resource.h \
$$SRC_FOLDER/SeqViewDialog.h \
$$SRC_FOLDER/stdafx.h \
$$SRC_FOLDER/TraitImageDialog.h \
$$SRC_FOLDER/VariationDialog.h \
$$SRC_FOLDER/PrntScreen.h \
$$SRC_FOLDER/Rb2Doc.h \
$$SRC_FOLDER/serialport.h \
$$SRC_FOLDER/SMTPConnection2.h \
$$SRC_FOLDER/SpeakWithOriginDlg.h \
$$SRC_FOLDER/AlgorithmeGenetique.h \
$$SRC_FOLDER/Colonne.h \
$$SRC_FOLDER/DialogCurve.h \
$$SRC_FOLDER/DialogFilmColonnes.h \
$$SRC_FOLDER/DialogParam.h \
$$SRC_FOLDER/DialogSetValue.h \
$$SRC_FOLDER/DialogTranslationNewPort.h \
$$SRC_FOLDER/DialogVariation.h \
$$SRC_FOLDER/DialogWaveForm.h \
$$SRC_FOLDER/Fitting.h \
$$SRC_FOLDER/NewPortXPS.h \
$$SRC_FOLDER/Parametre.h \
$$SRC_FOLDER/ProDialog.h \
$$SRC_FOLDER/RelationAnalog.h \
$$SRC_FOLDER/Sequence.h \
$$SRC_FOLDER/SerieImage.h \
$$SRC_FOLDER/SettingSave.h \
$$SRC_FOLDER/Variation.h \
$$SRC_FOLDER/WaveForme.h \

