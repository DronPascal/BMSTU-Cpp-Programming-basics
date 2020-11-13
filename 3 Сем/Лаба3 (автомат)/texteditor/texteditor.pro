TEMPLATE = app
TARGET = texteditor
QT += quick quickcontrols2
qtHaveModule(widgets): QT += widgets

cross_compile: DEFINES += QT_EXTRA_FILE_SELECTOR=\\\"touch\\\"

static {
    QT += svg
    QTPLUGIN += qtvirtualkeyboardplugin
}
target.path = $$[QT_INSTALL_EXAMPLES]/virtualkeyboard/basic
INSTALLS += target

HEADERS += \
    documenthandler.h \
    universalHighlighter.h

SOURCES += \
    texteditor.cpp \
    documenthandler.cpp

OTHER_FILES += \
    qml/*.qml

RESOURCES += \
    texteditor.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols2/texteditor
INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
