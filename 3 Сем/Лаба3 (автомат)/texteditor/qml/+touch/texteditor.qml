import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0
import Qt.labs.platform 1.1
import QtQuick 2.0

import io.qt.texteditor 1.0

ApplicationWindow {
    id: window
    visible: true
    title: document.fileName + "Text Editor"
    property bool closeApplicationNow: false

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3
        height: window.height
        dragMargin: window.width/50
        edge: Qt.LeftEdge
        opacity: 0.90
        background: menuBackground
        z: 2
        Rectangle {
            id: menuBackground
            color: "lightgray"
            anchors.fill: drawer
        }

        Flickable {
            anchors.fill: parent
            flickableDirection: Qt.Vertical
            boundsBehavior: Flickable.StopAtBounds
            contentHeight: menuColumn.implicitHeight

            Column {
                id: menuColumn
                anchors.fill: parent
                spacing: 2
                Rectangle {
                    width: parent.width
                    height: header.height
                    color: "orange"
                    Text {
                        anchors.centerIn: parent
                        text: "MAIN MENU"
                        font.pixelSize: 30
                        font.bold: true
                        color: "black"
                    }
                }
                Button {
                    text: qsTr("New")
                    width: parent.width
                    font.pixelSize: 30
                    onClicked: { document.load("untitled"); drawer.close() }
                }
                Button {
                    text: qsTr("Open")
                    width: parent.width
                    font.pixelSize: 30
                    onClicked: {drawer.close(); openDialog.open() }
                }
                Button {
                    text: qsTr("Save")
                    width: parent.width
                    font.pixelSize: 30
                    enabled: saveButton.saveEnabled
                    onClicked: {drawer.close(); window.savePressed() }
                }
                Button {
                    text: qsTr("Save As...")
                    width: parent.width
                    font.pixelSize: 30
                    onClicked: {drawer.close(); saveDialog.open() }
                }
                Button {
                    text: qsTr("Save All")
                    width: parent.width
                    font.pixelSize: 30
                    enabled: saveAllButton.saveAllEnabled
                    onClicked: {drawer.close(); saveAllDialog.open() }
                }
                Button {
                    id: closeMenuItem
                    text: qsTr("Close")
                    width: parent.width
                    font.pixelSize: 30
                    enabled: !(tabBar.tabCount == 1 && document.isSaved && document.isUntitled)
                    onClicked: {drawer.close(); document.isSaved ? document.closeDocument() : closeDialog.open() }
                }
                Button {
                    text: qsTr("Close All")
                    width: parent.width
                    font.pixelSize: 30
                    enabled: tabBar.tabCount > 1
                    onClicked: {
                        drawer.close();
                        closeAllDialog.msg = document.toSaveList()
                        closeAllDialog.open()
                    }
                }
                Button {
                    text: qsTr("Documents list")
                    width: parent.width
                    font.pixelSize: 30
                    enabled: tabBar.tabCount > 1
                    onClicked: {drawer.close(); documentsListWindow.open() }
                }
                Button {
                    text:  qsTr("Done editing")
                    width: parent.width
                    font.pixelSize: 30
                    enabled: textArea.readOnly ? 0 : 1
                    onClicked: {drawer.close(); textArea.readOnly = true }
                }
                Button {
                    text: tabPopup.opened ? qsTr("Hide TabBar") : qsTr("Show TabBar")
                    width: parent.width
                    font.pixelSize: 30
                    onClicked: {drawer.close(); tabPopup.opened ? tabPopup.close() : tabPopup.open() }
                }
                Button {
                    text: qsTr("Quit")
                    width: parent.width
                    font.pixelSize: 30
                    font.italic: true
                    onClicked: {
                        drawer.close();
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
                            Qt.quit()
                        }
                    }
                }
            }
        }
    }

    header: ToolBar {
        leftPadding: 5

        RowLayout {
            anchors.fill: parent
            spacing: 0
            visible: textArea.readOnly
            ToolButton {
                id: menuTool
                font.family: "fontello"
                text: "\uF142" // icon-ellipsis-vert
                font.pixelSize: 20
                onClicked: drawer.open()
            }
            Label {
                text: qsTr("Text Editor")
                font.bold: true
                font.pixelSize: 20
                //width: parent.width-menuTool.width-editTool.width
                elide: Label.ElideRight
                Layout.fillWidth: true

                MouseArea {
                    anchors.fill: parent
                    property int startX
                    property int startY
                    onPressed: {
                        startX = mouseX
                        startY = mouseY
                    }
                    onReleased: {
                        var deltax = mouseX - startX
                        var deltay = mouseY - startY
                        if (Math.abs(deltax) > 50 || Math.abs(deltay) > 50) {
                            if (deltax > 30 && Math.abs(deltay) < 30) {
                                // swipe left
                                if (tabBar.current > 0)
                                {
                                    tabBar.current--
                                    document.tabNumber = tabBar.current
                                    document.uploadDocument(document.urlByTab(tabBar.current))
                                }
                            } else if (deltax < -30 && Math.abs(deltay) < 30) {
                                // swipe right
                                if (tabBar.current < tabBar.tabCount - 1)
                                {
                                    tabBar.current++
                                    document.tabNumber = tabBar.current
                                    document.uploadDocument(document.urlByTab(tabBar.current))
                                }
                            }
                        }
                    }
                }
            }
            ToolButton {
                id: editTool
                font.family: "fontello"
                text: "\u270E" // icon-pencil
                font.pixelSize: 20
                visible: textArea.readOnly
                highlighted: true
                onClicked: textArea.readOnly = false
            }
        }

        Flickable {
            visible: !textArea.readOnly
            anchors.fill: parent
            Material.primary: "#E0E0E0"
            Material.elevation: 0
            contentWidth: toolRow.implicitWidth
            flickableDirection: Qt.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            Row {
                id: toolRow
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
                    contentItem.visible: true
                }
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
                Qt.quit()
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
                Qt.quit()
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

    DocumentHandler {
        id: document
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        textColor: colorDialog.color
        Component.onCompleted: document.load("qrc:/texteditor.html")
        property bool tabOccurrence: true
        onLoaded: {
            if (document.isSaved)
            {
                textArea.text = text
                textArea.cursorPosition = document.cursorPosition
                document.isSaved = true
                saveButton.saveEnabled = false
            }
            else
            {
                textArea.text = text
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
                }
                if (document.docCount() === 1)
                {
                    tabPopup.close()
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
        height: Math.min(window.width, window.height) / 15
        width: window.width
        margins: 0
        padding: 0
        z: 1
        background: null
        closePolicy: Popup.NoAutoClose
        ListView {
            id: tabBar
            property int current: 0
            property int tabCount: 0
            flickableDirection: Flickable.HorizontalFlick
            boundsBehavior: Flickable.StopAtBounds
            width: parent.width
            height: parent.height
            orientation: Qt.Horizontal
            model: tabBarModel
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
                        height: parent.height
                        width:  1
                        anchors.right: parent.right
                        color: "gray"
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
                                text: "\u00D7" // icon-close
                                font.pixelSize: parent.height*1
                                color: "white"
                            }
                            MouseArea {
                                id: closeXArea
                                propagateComposedEvents: true
                                anchors.fill: parent
                                onClicked:  {document.isSaved ? document.closeDocument() : closeDialog.open()
                                    textArea.activeFocus = true}
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
        anchors.fill: parent

        TextArea.flickable: TextArea {
            id: textArea
            textFormat: Qt.AutoText
            wrapMode: TextArea.Wrap
            font.pixelSize: 22
            readOnly: true
            leftPadding: 6
            rightPadding: 6
            topPadding: tabPopup.opened ? tabPopup.height : 0
            bottomPadding: 0
            background: null
            activeFocusOnPress: false

            onLinkActivated: Qt.openUrlExternally(link)
            onTextChanged: {
                document.isSaved = false
                saveButton.saveEnabled = true
                if (document.docCount() > 1)
                    saveAllButton.saveAllEnabled = true
            }
            onPressed: textArea.focus = true
        }
        ScrollBar.vertical: ScrollBar {}
    }

    Popup {
        id: documentsListWindow
        width: window.width/3
        height: window.height/3
        x: window.width/2
        y: window.height/2
        margins: 10
        padding: 0
        background: Rectangle {
            color: "gray"
            anchors.fill: documentsListWindow
        }

        ListView {
            id: documentsList
            anchors.fill: parent
            orientation: Qt.Vertical
            spacing: 0
            model: tabBarModel
            boundsBehavior: Flickable.StopAtBounds
            delegate: SwipeDelegate {
                id: delegate
                width: parent.width
                text: model.display.substring(model.display.lastIndexOf('/')+1,model.display.length)
                background: documentsListWindow
                font.pixelSize: 22
                onClicked: {
                    document.tabNumber = index
                    tabBar.current = index
                    document.uploadDocument(document.urlByTab(index))
                }
            }
        }
    }



    RoundButton {
        id: keyboardButton
        visible: !textArea.readOnly
        property bool keyIsActive: false
        text: "\uF07B" // icon-keyboard
        font.family: "fontello"
        width: 70
        height: width
        font.pixelSize: height/2
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 12
        highlighted: true

        onClicked: {
            if (keyIsActive)
            {
                textArea.focus = true
                keyIsActive = false
            }
            else {
                textArea.focus ? textArea.focus = false : {}
                textArea.activeFocusOnPress = true
                textArea.focus = true
                textArea.activeFocusOnPress = false
                keyIsActive = true
            }
        }
    }
}
