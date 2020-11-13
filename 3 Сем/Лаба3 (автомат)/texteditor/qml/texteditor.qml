import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.0
import Qt.labs.platform 1.1

import io.qt.texteditor 1.0


ApplicationWindow {
    id: window
    width: 1024
    height: 600
    visible: true
    title: document.fileName + " - Text Editor"
    property bool closeApplicationNow: false

    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }

    Shortcut {
        sequence: StandardKey.New
        onActivated: document.load("untitled")
    }
    Shortcut {
        sequence: StandardKey.Open
        onActivated: openDialog.open()
    }
    Shortcut {
        sequence: StandardKey.Save
        onActivated: window.savePressed()
    }
    Shortcut {
        onActivated: saveAllDialog.open()
        sequence: "Ctrl+Shift+S"
    }
    Shortcut {
        sequence: StandardKey.Copy
        onActivated: textArea.copy()
    }
    Shortcut {
        sequence: StandardKey.Cut
        onActivated: textArea.cut()
    }
    Shortcut {
        sequence: StandardKey.Paste
        onActivated: textArea.paste()
    }
    Shortcut {
        sequence: StandardKey.Bold
        onActivated: document.bold = !document.bold
    }
    Shortcut {
        sequence: StandardKey.Italic
        onActivated: document.italic = !document.italic
    }
    Shortcut {
        sequence: StandardKey.Underline
        onActivated: document.underline = !document.underline
    }
    Shortcut {
        sequence: StandardKey.Close
        onActivated: document.isSaved ? document.closeDocument() : closeDialog.open()
    }
    Shortcut {
        sequence: "Ctrl+Shift+F4"
        onActivated: closeAllDialog.open()
    }
    Shortcut {
        onActivated: textArea.selectAll()
        sequence: StandardKey.SelectAll
    }

    MenuBar
    {
        Menu {
            title: qsTr("&File")

            MenuItem {
                text: qsTr("&New")
                onTriggered: document.load("untitled")
                shortcut: StandardKey.New
            }
            MenuItem {
                text: qsTr("&Open")
                onTriggered: openDialog.open()
                shortcut: StandardKey.Open
            }
            MenuItem {
                text: qsTr("&Save")
                enabled: saveButton.saveEnabled
                onTriggered: window.savePressed()
                shortcut: StandardKey.Save
            }
            MenuItem { separator: true }
            MenuItem {
                text: qsTr("&Save As...")
                onTriggered: saveDialog.open()
            }
            MenuItem {
                text: qsTr("&Save All")
                enabled: saveAllButton.saveAllEnabled
                onTriggered: saveAllDialog.open()
                shortcut: "Ctrl+Shift+S"
            }
            MenuItem {
                id: closeMenuItem
                text: qsTr("&Close")
                onTriggered: document.isSaved ? document.closeDocument() : closeDialog.open()
                shortcut: StandardKey.Close
            }
            MenuItem {
                text: qsTr("&Close All")
                enabled: tabBar.tabCount > 1
                onTriggered: {
                    closeAllDialog.msg = document.toSaveList()
                    closeAllDialog.open()
                }
                shortcut: "Ctrl+Shift+F4"
            }
            MenuItem { separator: true }
            MenuItem {
                text: qsTr("&Quit")
                onTriggered: {
                    if (saveAllButton.saveAllEnabled || saveButton.saveEnabled) {
                        console.log("Save everything before closing!!! …")
                        close.accepted = false
                        window.closeApplicationNow = true
                        if (document.docCount() > 1)
                            saveAllDialog.open()
                        else
                            closeDialog.open()
                    }
                    else
                    {
                        close.accepted = true
                        window.close()
                    }
                }
            }
        }

        Menu {
            title: qsTr("&Edit")
            MenuItem {
                text: qsTr("&Undo")
                enabled: textArea.canUndo
                onTriggered: textArea.undo()
                shortcut: StandardKey.Undo
            }
            MenuItem {
                text: qsTr("&Redo")
                enabled: textArea.canRedo
                onTriggered: textArea.redo()
                shortcut: StandardKey.Redo
            }
            MenuItem { separator: true }
            MenuItem {
                text: qsTr("&Copy")
                enabled: textArea.selectedText
                onTriggered: textArea.copy()
                shortcut: StandardKey.Copy
            }
            MenuItem {
                text: qsTr("Cu&t")
                enabled: textArea.selectedText
                onTriggered: textArea.cut()
                shortcut: StandardKey.Cut
            }
            MenuItem {
                text: qsTr("&Paste")
                enabled: textArea.canPaste
                onTriggered: textArea.paste()
                shortcut: StandardKey.Paste
            }
            MenuItem { separator: true }
            MenuItem {
                text: qsTr("&Delete")
                enabled: textArea.selectedText
                onTriggered: textArea.remove(document.selectionStart, document.selectionEnd)
            }
            MenuItem {
                text: qsTr("&Select All")
                enabled: true
                onTriggered: textArea.selectAll()
                shortcut: StandardKey.SelectAll
            }
        }
        Menu {
            title: qsTr("&View")
            MenuItem {
                text: qsTr("&Documents list")
                enabled: tabBar.tabCount > 1
                onTriggered: documentsListWindow.show()
            }
        }
    }

    Window {
        id: documentsListWindow
        title: qsTr("Documents list")
        width: window.width/3
        height: window.height/2
        ListView {
            id: documentsList
            anchors.fill: parent
            orientation: Qt.Vertical
            spacing: 0
            model: tabBarModel
            delegate: SwipeDelegate {
                id: delegate
                text: model.display.substring(model.display.lastIndexOf('/')+1,model.display.length)
                width: parent.width
                onClicked: {
                    document.tabNumber = index
                    tabBar.current = index
                    document.uploadDocument(document.urlByTab(index))
                }
            }
        }
    }

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        selectedNameFilter.index: 1
        nameFilters: ["Text files (*.txt)", "HTML files (*.html *.htm)", "All files (*.*)"]
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            let curIsUntitled = document.isUntitled
            if (document.docCount() === 1 && curIsUntitled)
            {
                document.tabOccurrence = false
                document.load(file)
                tabBar.current = saveAllDialog.currentTab
                document.tabNumber = saveAllDialog.currentTab
                document.uploadDocument(document.urlByTab(0))
                document.closeDocument()
                document.tabOccurrence = true
            }
            else document.load(file)
        }
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: document.fileType
        nameFilters: openDialog.nameFilters
        selectedNameFilter.index: document.fileType === "txt" ? 0 : 1
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            document.saveAs(file)
            saveButton.saveEnabled = false
            if (document.isUntitled)
                document.isUntitled = false
            if (!document.isSaved)
                document.isSaved = true
            if (window.closeApplicationNow && document.docCount() === document.tabNumber+1)
                window.close()
            if (closeDialog.deleteAfterSave)
            {
                document.closeDocument()
                closeDialog.deleteAfterSave = false
            }
            if (closeAllDialog.deleteAll)
                window.continueClosing()
            if (saveAllDialog.saveAll)
                window.continueSaving()
        }
        onRejected: closeApplicationNow = false
    }

    MessageDialog {
        id: closeDialog
        property bool deleteAfterSave: false
        property bool isActive: false
        text: qsTr("Do you want to save changes in the file " + document.fileName + "?")
        buttons:  StandardButton.Save | StandardButton.Discard | StandardButton.Cancel
        onDiscardClicked: {
            document.closeDocument()
            if (closeAllDialog.deleteAll)
                window.continueClosing()
            if (window.closeApplicationNow)
                window.close()
        }
        onSaveClicked: {
            if (document.isUntitled) {
                closeDialog.deleteAfterSave = true
                saveDialog.open()
            }
            else {
                document.save()
                document.closeDocument()
            }
        }
    }

    MessageDialog {
        id: closeDialogCopy
        text: qsTr("Do you want to save changes in the file " + document.fileName + "?")
        buttons:  StandardButton.Save | StandardButton.Discard | StandardButton.Cancel
        onDiscardClicked: {
            document.closeDocument()
            if (closeAllDialog.deleteAll)
                window.continueClosing()
        }
        onSaveClicked: {
            if (document.isUntitled) {
                closeDialog.deleteAfterSave = true
                saveDialog.open()
            }
            else {
                document.save()
                document.closeDocument()
            }
        }
    }

    MessageDialog {
        id: closeAllDialog
        objectName: "closeAllDialogObj"
        property string msg: ""
        property bool deleteAll: false
        property int docCount: 0
        property int i: 0
        text: qsTr("Do you want to save files before close?")
        detailedText: msg
        buttons:  StandardButton.SaveAll | StandardButton.Discard | StandardButton.Cancel
        onDiscardClicked: document.closeAllDocuments()
        onSaveAllClicked: {
            closeAllDialog.deleteAll = true
            closeAllDialog.docCount = document.docCount()
            closeAllDialog.i = 0
            window.continueClosing()
        }
    }

    function continueClosing() {
        if (closeAllDialog.i < closeAllDialog.docCount)
        {
            closeAllDialog.i++
            if (document.isSaved)
            {
                //document.save()
                document.closeDocument()
                continueClosing()
            }
            else
            {
                if (closeDialog.isActive)
                {
                    closeDialog.isActive = false
                    closeDialogCopy.open()
                }
                else
                {
                    closeDialog.isActive = true
                    closeDialog.open()
                }
            }
        }
        else
        {
            closeAllDialog.deleteAll = false
        }
    }

    MessageDialog {
        id: saveAllDialog
        property bool saveAll: false
        property int docCount: 0
        property int i: 0
        property int currentTab: 0
        text: qsTr("Do you want to save all files?")
        buttons:  StandardButton.SaveAll | StandardButton.Discard | StandardButton.Cancel
        onSaveAllClicked: {
            saveAllDialog.saveAll = true
            saveAllDialog.docCount = document.docCount()
            saveAllDialog.i = 0
            saveAllDialog.currentTab = tabBar.current
            window.continueSaving()
        }
        onDiscardClicked: window.closeApplicationNow ? window.close() : {}
        onCancelClicked: closeApplicationNow = false
    }
    //продолжение сохранения всеx документов
    function continueSaving()
    {
        if (saveAllDialog.i < saveAllDialog.docCount)
        {
            tabBar.current = saveAllDialog.i
            document.tabNumber = saveAllDialog.i
            document.uploadDocument(document.urlByTab(saveAllDialog.i))
            saveAllDialog.i++
            window.savePressed()
        }
        else
        {
            saveAllDialog.saveAll = false
            tabBar.current = saveAllDialog.currentTab
            document.tabNumber = saveAllDialog.currentTab
            document.uploadDocument(document.urlByTab(saveAllDialog.currentTab))
            saveAllButton.saveAllEnabled = false
        }
    }
    //сохрнение текущего документа
    function savePressed()
    {
        if (document.isSaved)
        {
            window.continueSaving()
        }
        else
        {
            if (document.isUntitled)
            {
                saveDialog.open()
            }
            else
            {
                document.save()
                if (errorDialog.text !== "Cannot save: No file name specified")
                {
                    document.isSaved = true
                    saveButton.saveEnabled = false
                }
            }
        }
    }

    FontDialog {
        id: fontDialog
        onAccepted: {
            document.fontFamily = font.family;
            document.fontSize = font.pointSize;
        }
    }

    ColorDialog {
        id: colorDialog
        currentColor: "black"
    }

    MessageDialog {
        id: errorDialog
        informativeText: qsTr("File unavailable at current path")
        onCloseClicked: text = ""
        onOkClicked: {
            text = ""
            saveDialog.open()
        }
    }

    header: ToolBar {
        leftPadding: 8
        background: Rectangle { color: "white" }
        Flow {
            id: flow
            width: parent.width

            Row {
                id: fileRow
                ToolButton {
                    id: newButton
                    text: "\uE800" // icon-new
                    font.family: "fontello"
                    onClicked: document.load("untitled")
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("New")
                }
                ToolSeparator {
                    contentItem.visible: true
                }
                ToolButton {
                    id: openButton
                    text: "\uF115" // icon-folder-open
                    font.family: "fontello"
                    onClicked: openDialog.open()
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Open")
                }
                ToolButton {
                    id: saveButton
                    property bool saveEnabled: false
                    text: "\uF150" // icon-save
                    font.family: "fontello"
                    enabled: saveEnabled
                    onClicked: window.savePressed()
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Save")
                }
                ToolButton {
                    id: saveAllButton
                    property bool saveAllEnabled: false
                    text: "\uF0A0" // icon-save-all
                    font.family: "fontello"
                    enabled: saveAllEnabled
                    onClicked: document.docCount() > 1 ? saveAllDialog.open() : saveDialog.open()
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Save all")
                }
                ToolSeparator {
                    contentItem.visible: true
                }
                ToolButton {
                    id: undoButton
                    text: "\uF0EB" // icon-undo
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    enabled: textArea.canUndo
                    onClicked: textArea.undo()
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Undo")
                }
                ToolButton {
                    id: redoButton
                    text: "\uF0EC" // icon-redo
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    enabled: textArea.canRedo
                    onClicked: textArea.redo()
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Redo")
                }
                ToolButton {
                    id: copyButton
                    text: "\uF0C5" // icon-copy
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    enabled: textArea.selectedText
                    onClicked: textArea.copy()
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Copy")
                }
                ToolButton {
                    id: cutButton
                    text: "\u2702" // icon-cut
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    enabled: textArea.selectedText
                    onClicked: textArea.cut()
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Cut")
                }
                ToolButton {
                    id: pasteButton
                    text: "\uF0EA" // icon-paste
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    enabled: textArea.canPaste
                    onClicked: textArea.paste()
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Paste")
                }
                ToolSeparator {
                    contentItem.visible: fileRow.y === editRow.y
                }
            }
            Row {
                id: editRow
                ToolButton {
                    id: boldButton
                    text: "\uE77A" // icon-bold
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.bold
                    onClicked: document.bold = !document.bold
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Bold")
                }
                ToolButton {
                    id: italicButton
                    text: "\uE77B" // icon-italic
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.italic
                    onClicked: document.italic = !document.italic
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Italic")
                }
                ToolButton {
                    id: underlineButton
                    text: "\uF0CD" // icon-underline
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.underline
                    onClicked: document.underline = !document.underline
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Underline")
                }
                ToolButton {
                    id: fontFamilyToolButton
                    text: qsTr("\uE779") // icon-font
                    font.family: "fontello"
                    font.bold: document.bold
                    font.italic: document.italic
                    font.underline: document.underline
                    onClicked: {
                        fontDialog.currentFont.family = document.fontFamily;
                        fontDialog.currentFont.pointSize = document.fontSize;
                        fontDialog.open();
                    }
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Font")
                }
                ToolButton {
                    id: textColorButton
                    text: "\uF1FC" // icon-brush
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    onClicked: colorDialog.open()

                    Rectangle {
                        width: aFontMetrics.width + 3
                        height: 2
                        color: document.textColor
                        parent: textColorButton.contentItem
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.baseline: parent.baseline
                        anchors.baselineOffset: 6
                        TextMetrics {
                            id: aFontMetrics
                            font: textColorButton.font
                            text: textColorButton.text
                        }
                    }
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Text color")
                }
                ToolSeparator {
                    contentItem.visible: true
                }
                ToolButton {
                    id: alignLeftButton
                    text: "\uE77E" // icon-align-left
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignLeft
                    onClicked: document.alignment = Qt.AlignLeft
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Align left")
                }
                ToolButton {
                    id: alignCenterButton
                    text: "\uE77F" // icon-align-center
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignHCenter
                    onClicked: document.alignment = Qt.AlignHCenter
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Align center")
                }
                ToolButton {
                    id: alignRightButton
                    text: "\uE780" // icon-align-right
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignRight
                    onClicked: document.alignment = Qt.AlignRight
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Align right")
                }
                ToolButton {
                    id: alignJustifyButton
                    text: "\uE781" // icon-align-justify
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignJustify
                    onClicked: document.alignment = Qt.AlignJustify
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                    ToolTip.text: qsTr("Align justify")
                }
            }
        }
    }

    DocumentHandler {
        id: document
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        textColor: colorDialog.color
        Component.onCompleted: document.load("untitled")
        property bool tabOccurrence: true
        onLoaded: {
            if (document.isSaved)
            {
                textArea.text = text
                textArea.focus = true
                textArea.cursorPosition = document.cursorPosition
                document.isSaved = true
                saveButton.saveEnabled = false
            }
            else
            {
                textArea.text = text
                textArea.focus = true
                textArea.cursorPosition = document.cursorPosition
                document.isSaved = false
                saveButton.saveEnabled = true
            }
            if (document.docCount() > 1 && document.hasUnsaved())
                saveAllButton.saveAllEnabled = true
            else
                saveAllButton.saveAllEnabled = false
        }
        onError: {
            errorDialog.text = qsTr(message)
            errorDialog.visible = true
        }
        onTabNumberChanged: {
            tabBar.current = document.tabNumber
            tabBar.positionViewAtIndex(tabBar.current, ListView.Center)
            if (document.tabOccurrence)
            {
                if (document.docCount() === 2)
                {
                    tabPopup.open()
                    textArea.topPadding = tabPopup.height
                }
                if (document.docCount() === 1)
                {
                    tabPopup.close()
                    textArea.topPadding = 0
                }
            }
            tabBar.tabCount = document.docCount()
        }
        onIsSavedChanged: saveButton.saveEnabled = false
        onDocumentClosed: {
            if (document.docCount() > 1 && document.hasUnsaved())
                saveAllButton.saveAllEnabled = true
            else
                saveAllButton.saveAllEnabled = false
        }
    }

    Popup {
        id: tabPopup
        height: {
            if (Screen.height > 1300)
                Screen.height / 40
            else if (Screen.height < 700)
                Screen.height / 25
            else Screen.height / 12
        }
        width: window.width
        z: 0
        margins: 0
        padding: 0
        background: null
        closePolicy: Popup.NoAutoClose
        ListView {
            id: tabBar
            property int current: 0
            property int tabCount: 0
            flickableDirection: Flickable.HorizontalFlick
            width: parent.width
            height: parent.height
            orientation: Qt.Horizontal
            model: tabBarModel
            Rectangle {
                width: tabBar.current !== 0 ? 2 : 0
                height: parent.height
                anchors.left: tabBar.left
                color: "orange"
            }
            Component {
                id: tabDelegate
                Rectangle {
                    id: curTab
                    width: tabBar.current === index ? tabInfo.width + tabBar.height : tabInfo.width + parent.height *2 / 7
                    height: tabBar.height
                    color:  tabBar.current === index ? "white" : "lightgray"
                    Text {
                        id: tabInfo
                        font.pixelSize: parent.height / 2.5
                        horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: parent.height / 7
                        text: model.display.substring(model.display.lastIndexOf('/')+1,model.display.length)
                        elide: Text.ElideRight
                        font.bold: tabBar.current === index
                    }
                    Rectangle {//bot
                        width: parent.width
                        height: parent.height / 6
                        anchors.bottom: parent.bottom
                        color: tabBar.current === index ? "orange" : "gray"
                    }
                    Rectangle {//right
                        width: {
                            if (tabBar.current === index && tabBar.current > 0) 2
                            else if (tabBar.current-1 === index) 0
                            else 1
                        }
                        height: parent.height
                        anchors.right: parent.right
                        color: tabBar.current === index ? "orange" : "gray"
                    }
                    Rectangle {//left
                        width: (tabBar.current === index && tabBar.current > 0) ? 2 : 0
                        height: parent.height
                        anchors.left: parent.left
                        color: "orange"
                    }
                    Rectangle {//top
                        width: parent.width
                        height: tabBar.current !== index ? 2 : 0
                        anchors.top: parent.top
                        color: "orange"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (index != document.tabNumber)
                            {
                                tabBar.current = index
                                document.tabNumber = index
                                document.uploadDocument(model.display)
                            }
                        }
                        Rectangle {
                            id: closeXButton
                            visible: tabBar.current === index
                            anchors.rightMargin: parent.height / 7
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            height: tabBar.height / 2
                            width: height
                            radius: height / 4
                            color: closeXArea.containsMouse ? "red" : "purple"
                            Text {
                                horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
                                font.bold: true
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "\u0F31" // icon-close
                                font.family: "fontello"
                                font.pixelSize: parent.height*1
                                color: "white"
                            }
                            MouseArea {
                                id: closeXArea
                                propagateComposedEvents: true
                                anchors.fill: parent
                                onClicked: {
                                    document.isSaved ? document.closeDocument() : closeDialog.open()
                                }
                                hoverEnabled: true
                            }
                        }
                    }
                }
            }
            delegate: tabDelegate
        }
    }

    Flickable {
        id: flickable
        flickableDirection: Flickable.VerticalFlick
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        TextArea.flickable: TextArea {
            id: textArea
            textFormat: Qt.AutoText
            wrapMode: TextArea.Wrap
            font.pixelSize: 20
            focus: true
            selectByMouse: true
            persistentSelection: true
            // Different styles have different padding and background
            // decorations, but since this editor is almost taking up the
            // entire window, we don't need them.
            leftPadding: 6
            rightPadding: 6
            topPadding: 0
            bottomPadding: 0
            background: null

            onTextChanged: {
                document.isSaved = false
                saveButton.saveEnabled = true
                if (document.docCount() > 1)
                    saveAllButton.saveAllEnabled = true
            }
            MouseArea {
                acceptedButtons: Qt.RightButton
                anchors.fill: parent
                onClicked: {contextMenu.open()
                    scale++}
            }
            onLinkActivated: Qt.openUrlExternally(link)
            Rectangle {//top
                width: parent.width
                height: 2
                anchors.top: parent.top
                color: "orange"
            }
            Rectangle {//bot
                width: parent.width
                height: 2
                anchors.bottom: parent.bottom
                color: "orange"
            }
            Rectangle {//left
                width: 2
                height: parent.height
                anchors.left: parent.left
                color: "orange"
            }
            Rectangle {//right
                width: 2
                height: parent.height
                anchors.right: parent.right
                color: "orange"
            }
        }
        ScrollBar.vertical: ScrollBar {}
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: qsTr("Copy")
            enabled: textArea.selectedText
            onTriggered: textArea.copy()
        }
        MenuItem {
            text: qsTr("Cut")
            enabled: textArea.selectedText
            onTriggered: textArea.cut()
        }
        MenuItem {
            text: qsTr("Paste")
            enabled: textArea.canPaste
            onTriggered: textArea.paste()
        }
        MenuItem {
            text: qsTr("Select All")
            onTriggered: textArea.selectAll()
        }
        MenuSeparator {}
        MenuItem {
            text: qsTr("Font...")
            onTriggered: fontDialog.open()
        }
        MenuItem {
            text: qsTr("Color...")
            onTriggered: colorDialog.open()
        }
    }

    onClosing: {
        if (saveAllButton.saveAllEnabled || saveButton.saveEnabled) {
            console.log("Save everything before closing!!! …")
            close.accepted = false
            window.closeApplicationNow = true
            if (document.docCount() > 1)
                saveAllDialog.open()
            else
                closeDialog.open()
        }
        else
            close.accepted = true
    }

    function close() {
        var closeEvent = { accepted: true };
        window.closing(closeEvent)
        if (closeEvent.accepted)
        {
            Qt.quit()
        }
        else
        {
            console.log("Closing denied")
        }
    }
}
