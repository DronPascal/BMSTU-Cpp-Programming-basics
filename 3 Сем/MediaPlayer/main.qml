import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0
import QtMultimedia 5.0
import QtQml.Models 2.12
import QtWinExtras 1.0 as Win
import Qt.labs.folderlistmodel 2.1

import io.qt.examples.quick.controls.filesystembrowser 1.0
import my.mo.playlisthandler 1.0

ApplicationWindow {
    id: window
    visible: true
    minimumHeight: 350
    maximumHeight: 350
    minimumWidth: 360
    title: qsTr("ChillAudioPlayer")
    color: "#1f1f1f"

    MediaPlayer {
        id: media_player
        property bool changeSlotBlocked: false
        autoPlay: true
        //source: "qrc:/Daughter - Improve.mp3"
        onPositionChanged: {
            console.log(position, media_player.duration-1000)
            if (!changeSlotBlocked && position !== 0 && position > media_player.duration-500 && !position_slider.pressed)
            {
                if (playlistView.currentIndex !== playlistView.count-1)
                {
                    if (playlistView.currentIndex == playlistView.count-1)
                        playlistView.currentIndex = 0
                    else
                        playlistView.currentIndex++
                    media_player.source = playlistModel.get(playlistView.currentIndex)["path"]
                }
            }
        }
        onPlaybackStateChanged:
        {
            if (media_player.PlayingState) {
                play_pause_button.source = "qrc:/images/pause.png"
            }
            else if (media_player.PausedState) {
                play_pause_button.source = "qrc:/images/play.png"
            }
            else if (media_player.StoppedState) {
                play_pause_button.source = "qrc:/images/play.png"
            }
        }
    }

    PlaylistHandler {
        id: playlistHandler
        onUrlListUploaded: window.uploadSongs(text)
    }

    Window {
        id: dockWindow
        visible: false
        width: 150
        height: 20
        title: qsTr("")
        TextField {
            anchors.fill: parent
            width: parent.width
            height: 20
            placeholderText: qsTr("Enter name")

            onAccepted: {
                var album_name = getText(0, length)
                //select(0, length)
                console.log(album_name + ".alb")    //.txt
                remove(0, length)
                dockWindow.visible = false
                //нужно сложить строки
                var musics = ""
                var curIndex = playlistView.currentIndex
                playlistView.currentIndex = 0
                for (var i = 0; i < playlistModel.count; i++, playlistView.currentIndex++) {
                    musics = musics + playlistModel.get(playlistView.currentIndex)["path"] + "\n"
                    console.log(playlistModel.get(playlistView.currentIndex)["path"])
                }

                console.log(musics)
                playlistHandler.savePlaylist(album_name, musics)
            }
        }
    }

    menuBar: MenuBar
    {
        Menu {
            title: qsTr("&Menu")

            MenuItem {
                text: qsTr("&New")
                shortcut: "Ctrl+N"
                onTriggered: {
                    playlistModel.clear()
                    playlistTitle.text = "Playlist"
                    playlistTitle.playlist_path = "Playlist"
                }
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("&Save") //открывает альбом для прослушивания
                shortcut: "Ctrl+S"
                onTriggered: {
                    var album_name = playlistTitle.text
                    console.log(album_name + ".alb")    //txt
                    var musics = ""
                    var curIndex = playlistView.currentIndex
                    playlistView.currentIndex = 0
                    for (var i = 0; i < playlistModel.count; i++, playlistView.currentIndex++) {
                        musics = musics + playlistModel.get(playlistView.currentIndex)["path"] + "\n"
                        console.log(playlistModel.get(playlistView.currentIndex)["path"])
                    }

                    console.log(musics)
                    playlistHandler.savePlaylist(album_name, musics)
                }
            }

            MenuItem {
                text: qsTr("&Save As") //сохраняет плейлист(при нажатии нужно ввести имя альбома)
                onTriggered: {
                    dockWindow.visible = true
                }
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("&Delete") //удаляет плейлист
                onTriggered: {
                    console.log(playlistTitle.playlist_path)
                    playlistHandler.deletePlaylist(playlistTitle.playlist_path)
                }
            }
        }

        Menu {
            title: qsTr("&View")

            MenuItem {
                text: qsTr("&Album")
                shortcut: "Ctrl+A"
                checkable: true     //даю возможность сделать флаг
                onTriggered: {
                    if (checked) {
                        playlistArea.visible = true
                        window.width = window.width + column.width
                        window.minimumWidth = window.width

                    }
                    else {
                        playlistArea.visible = false
                        window.minimumWidth = window.width - column.width
                        window.width = window.width - playlistArea.width
                    }
                }
                //должен делать видным список песен
            }

            MenuItem {
                text: qsTr("&See Albums")
                shortcut: "Ctrl+D"
                checkable: true
                onTriggered: {
                    if (checked) {
                        fileModel.visible = true
                        window.width = window.width + fileModel.width
                        window.minimumWidth = window.width

                    }
                    else {
                        fileModel.visible = false
                        window.minimumWidth = window.width - fileModel.width
                        window.width = window.width - playlistArea.width
                    }
                }
            }
        }
    }

    RowLayout {
        width: parent.width
        height: parent.height

        ColumnLayout {
            height: parent.height
            //anchors.margins: 9
            width: 200
            Rectangle {
                id: fileModel
                visible: false
                width: 200
                height: column.height
                //title: qsTr("File System")

                ItemSelectionModel {
                    id: sel
                    model: fileSystemModel
                }

                TreeView {
                    id: view
                    anchors.fill: parent
                    anchors.margins: 0
                    model: fileSystemModel

                    rootIndex: rootPathIndex
                    selection: sel

                    itemDelegate: Item {
                          Text {
                              anchors.verticalCenter: parent.verticalCenter
                              color: styleData.textColor
                              elide: styleData.elideMode
                              text: styleData.value
                          }
                      }

                    TableViewColumn {
                        title: "Name"
                        role: "fileName"
                        resizable: true
                    }

                    onActivated : {
                        var url = fileSystemModel.data(index, FileSystemModel.UrlStringRole)
                        console.log("|-------------------|")
                        console.log(url)
                        if (url[url.length - 4] === '.' && url[url.length - 3] === 'a' &&
                                url[url.length - 2] === 'l' && url[url.length - 1] === 'b') {
                            playlistModel.clear()
                            var str = url.toString()
                            str = str.split("/")[str.split("/").length - 1]
                            playlistHandler.openPlaylist(url)
                            playlistTitle.text = str.split(".")[0]
                            playlistTitle.playlist_path = url.toString()
                            console.log(playlistTitle.playlist_path)
                            console.log("ok")
                            console.log(str.split(".")[0])
                        }
                        else if (url[url.length - 4] === '.' && url[url.length - 3] === 'm' &&
                                 url[url.length - 2] === 'p' && url[url.length - 1] === '3') {
                             uploadSongs(url)
                             console.log("ok")
                         }
                    }


                }
            }
        }

        ColumnLayout {
            id: column
            anchors.margins: 9
            width: 350
            height: parent.height
            spacing: 2
            //Layout.maximumWidth: 350

            Item {
                //anchors.centerIn: parent
                Layout.alignment: Qt.AlignCenter
                width: 200
                height: 200

                Image {
                    id: poster
                    smooth: true
                    anchors.fill: parent
                    source: playlistView.currentIndex != -1 ? "qrc:/images/def.jpg" : {}
                    //source: "qrc:/images/def.jpg" //media_player.metaData.coverArtUrlLarge
                    //source: media_player.metaData.posterUrl
                    onSourceChanged: {
                        /*if (!media_player.metaData.posterUrl) {
                            source: "qrc:/images/def.jpg"
                        }
                        else {
                            source: media_player.metaData.posterUrl
                        }*/
                    }
                }
            }
            Rectangle {
                id: textrect
                height: 45
                color: window.color
                anchors.left: parent.left
                anchors.right: parent.right
                Text {
                    id: name
                    anchors.fill: textrect
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    // maximumLineCount: 10
                    //text: "Song's name"
                    text: media_player.metaData.title
                    color: "white"
                    font.pointSize: text.length > 20 ? 350/(text.length+4) : 16
                }
            }



            Slider {
                id: position_slider
                enabled: media_player.hasAudio
                Layout.minimumWidth: 75
                Layout.fillWidth: true
                maximumValue: media_player.duration

                property bool sync: false

                onValueChanged: {
                    if (!sync)
                        media_player.seek(value)
                    if (position_slider.value === 1.0) {
                        play_pause_button.source = "qrc:/images/play.png"
                        if (playlistView.currentIndex == playlistView.count-1)
                            playlistView.currentIndex = 0
                        else
                            playlistView.currentIndex++
                        media_player.source = playlistModel.get(playlistView.currentIndex)["path"]
                    }
                    if (position_slider.value === 0.0) {
                        play_pause_button.source = "qrc:/images/pause.png"
                    }
                }

                Connections {
                    target: media_player
                    onPositionChanged: {
                        position_slider.sync = true
                        position_slider.value = media_player.position
                        position_slider.sync = false

                    }
                }
            }

            Rectangle {
                id: rect_duration_line
                width: parent.width
                height: 15
                color: "#1f1f1f"
                anchors.left: parent.left
                anchors.right: parent.right

                function normalSecs() {
                    if (arguments[0] < 10)
                        return "0"+arguments[0]
                    else return arguments[0]
                }

                RowLayout {
                    id: row_durations
                    anchors.centerIn: parent
                    anchors.fill: parent
                    Layout.fillWidth: true
                    Text {
                        id: current_duration
                        color: "white"

                        readonly property int current_minutes: Math.floor(media_player.position / 60000)
                        readonly property int current_seconds: Math.round((media_player.position % 60000) / 1000)

                        function formatTime(date) {
                            return Qt.formatTime(date, qsTr("mm:ss"))
                        }

                        text: qsTr("    " + current_minutes.toString() + ":" + rect_duration_line.normalSecs(current_seconds))
                        font.pixelSize: 16
                    }

                    Text {
                        id: duration
                        color: "white"
                        Layout.alignment: Qt.AlignRight
                        readonly property int duration_minutes: Math.floor(media_player.duration / 60000)
                        readonly property int duration_seconds: Math.round((media_player.duration % 60000) / 1000)

                        text: qsTr(duration_minutes.toString() + ":" + rect_duration_line.normalSecs(duration_seconds) + "  ")
                        font.pixelSize: 16
                    }
                }
            }

            Rectangle {
                id: rect_button_line
                Layout.fillWidth: true
                height: 35
                color: "#1f1f1f"
                anchors.left: parent.left
                anchors.right: parent.right


                RowLayout {
                    id: row
                    anchors.centerIn: rect_button_line
                    Image {
                        id: volume_icon
                        Layout.alignment: Qt.AlignLeft
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30
                        source: "qrc:/images/loud_sound.png"
                        MouseArea {
                            anchors.fill: parent
                            property var volumeNow: 1
                            onClicked: {
                                if (volume_slider.value !== 0) {
                                    volume_icon.source = "qrc:/images/mute.png"
                                    volumeNow = media_player.volume
                                    media_player.volume = 0
                                    volume_slider.value = 0
                                }
                                else {
                                    volume_icon.source = "qrc:/images/loud_sound.png"
                                    media_player.volume = volumeNow
                                    volume_slider.value = volumeNow
                                }
                            }
                        }
                    }

                    Slider {
                        id: volume_slider
                        Layout.alignment: Qt.AlignRight
                        minimumValue: 0.0
                        maximumValue: 1.0
                        value: 0.7
                        stepSize: 0.1
                        implicitWidth: 100

                        property bool sync: false
                        onValueChanged: {
                            if (!sync) {
                                media_player.volume = value
                                if (value > 0.66) {
                                    volume_icon.source = "qrc:/images/loud_sound.png"
                                }
                                else if (0.33 <= value && value <= 0.66) {
                                    volume_icon.source = "qrc:/images/medium_volume.png"
                                }
                                else if (0 < value && value < 0.33){
                                    volume_icon.source = "qrc:/images/low_volume.png"
                                }
                                else {
                                    volume_icon.source = "qrc:/images/mute.png"
                                }
                            }
                        }
                    }

                    Image {
                        id: prev_button
                        source: "qrc:/images/prev.png"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30
                        Layout.alignment: Qt.AlignLeft

                        state: "none"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (position_slider.value > 0.1) {
                                    position_slider.value = 0
                                }
                                else {
                                    if (playlistView.currentIndex == 0)
                                        playlistView.currentIndex = playlistView.count-1
                                    else
                                        playlistView.currentIndex--
                                    media_player.source = playlistModel.get(playlistView.currentIndex)["path"]
                                }

                                //если == 0, то на предыдущую песню
                            }

                            onPressed: prev_button.state = "pressed"
                            onReleased: prev_button.state = "none"
                        }
                        states: State {
                            name: "pressed"
                            when: MouseArea.pressed
                            PropertyChanges { target: prev_button; scale: 0.8; }
                        }
                        transitions: Transition {
                            NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad }
                        }

                    }

                    Image {
                        id: play_pause_button
                        source: "qrc:/images/pause.png"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (media_player.playbackState === MediaPlayer.PlayingState) {
                                    media_player.pause()
                                    play_pause_button.source = "qrc:/images/play.png"
                                }
                                else {
                                    media_player.play()
                                    play_pause_button.source = "qrc:/images/pause.png"
                                }
                            }
                        }
                    }

                    Image {
                        id: next_button
                        source: "qrc:/images/next.png"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30

                        state: "none"
                        /*MouseArea {
                            anchors.fill: parent
                            onClicked: playLogic.previous()
                            onPressed: next_button.state = "pressed"
                            onReleased: next_button.state = "none"
                        }*/
                        states: State {
                            name: "pressed"
                            when: MouseArea.pressed
                            PropertyChanges { target: next_button; scale: 0.8; }
                        }
                        transitions: Transition {
                            NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onPressed: next_button.state = "pressed"
                            onReleased: next_button.state = "none"
                            onClicked: {
                                if (playlistView.currentIndex == playlistView.count-1)
                                    playlistView.currentIndex = 0
                                else
                                    playlistView.currentIndex++
                                media_player.source = playlistModel.get(playlistView.currentIndex)["path"]
                                /*if (media_player.position < media_player.duration-1000)
                                {
                                    media_player.changeSlotBlocked = true
                                    media_player.pause()
                                    media_player.seek(media_player.duration-1)
                                    sleep(200)
                                    media_player.changeSlotBlocked = false
                                }
                                else
                                {*/
                                //playlistView.currentIndex++
                                //media_player.source = playlistModel.get(playlistView.currentIndex)["path"]
                                //media_player.play()
                                //}
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            id: spacer
            width: 10000
            height: column.height
        }

        ColumnLayout {
            id : playlistArea
            height: parent.height
            width: 350
            visible: false
            Rectangle {
                width: parent.width
                color: window.color
                height: playlistTitle.height
                z: 1
                Text {
                    id: playlistTitle
                    horizontalAlignment: Text.AlignHCenter
                    property var playlist_path: "Playlist"  //file:///C:/Users/MiAir/Music/Playlist.txt
                    width: parent.width
                    text: "Playlist"
                    font.pixelSize: 24
                    color: "white"
                }
            }
            Flickable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                z: 0
                flickableDirection: Flickable.VerticalFlick

                ListView {
                    id: playlistView
                    anchors.fill: parent
                    model: playlistModel
                    delegate: playlistDelegate
                    highlight: playlistHighlight
                }

                DropArea {
                    anchors.fill: parent
                    onDropped: { window.uploadSongs(drop.text)
                    console.log(drop.text) }
                }
            }
            Rectangle {
                width: parent.width
                color: window.color
                height: 40

                z: 1
                RowLayout {
                    height: playlistTitle.height
                    Image {
                        id: add_music
                        source: "qrc:/images/plus1.png"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30

                        state: "none"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                addingMusicDialog.open()
                            }

                            onPressed: add_music.state = "pressed"
                            onReleased: add_music.state = "none"
                        }
                        states: State {
                            name: "pressed"
                            when: MouseArea.pressed
                            PropertyChanges { target: add_music; scale: 0.8; }
                        }
                        transitions: Transition {
                            NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad }
                        }

                        FileDialog {
                            id: addingMusicDialog
                            folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
                            title: qsTr("Open audio files")
                            nameFilters: [qsTr("MP3 files (*.mp3)"), qsTr("All files (*.*)")]
                            selectMultiple: true
                            onAccepted: {
                                var file_protocol = "file:///"
                                for (var i = 0; i < fileUrls.length; i++) {
                                    var path = fileUrls[i].toString()
                                    path = path.substring(file_protocol.length, path.length)
                                    playlistModel.append( { "path" : path, } )
                                }
                            }
                        }
                    }

                    Image {
                        id: remove_music
                        source: "qrc:/images/minus1.png"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30

                        state: "none"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                playlistModel.remove(playlistView.currentIndex)
                            }

                            onPressed: remove_music.state = "pressed"
                            onReleased: remove_music.state = "none"
                        }
                        states: State {
                            name: "pressed"
                            when: MouseArea.pressed
                            PropertyChanges { target: remove_music; scale: 0.8; }
                        }
                        transitions: Transition {
                            NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad }
                        }
                    }

                    Image {
                        id: clear_musics
                        source: "qrc:/images/cls.png"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30


                        state: "none"
                        MouseArea {

                            anchors.fill: parent
                            onClicked: {
                                playlistModel.clear()
                            }

                            onPressed: clear_musics.state = "pressed"
                            onReleased: clear_musics.state = "none"
                        }
                        states: State {
                            name: "pressed"
                            when: MouseArea.pressed
                            PropertyChanges { target: clear_musics; scale: 0.8; }
                        }
                        transitions: Transition {
                            NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad }
                        }
                    }
                }
            }

            Component {
                id: playlistDelegate

                Item {
                    id: currentItem
                    width: parent.width
                    height: 30
                    Rectangle {
                        id : rectItem
                        anchors.fill: parent
                        color: "#33000000"
                        radius: 5
                        visible: mouse.pressed
                    }
                    Text {
                        x: 10
                        y: 10
                        color: "lightblue"

                        text: path.substring(path.lastIndexOf('/')+1,path.length)
                        font.pixelSize: 16
                    }
                    MouseArea {
                        id: mouse
                        property int playing_index
                        anchors.fill: parent
                        onClicked: {
                            playlistView.currentIndex = index
                        }
                        onDoubleClicked: {
                            media_player.source = path
                            media_player.play()

                        }
                    }
                }
            }
            Component {
                id: playlistHighlight
                Rectangle {
                    color: "gray"
                    radius: 5
                    y: playlistView.currentItem.y
                    Behavior on y {
                        SpringAnimation {
                            spring: 3
                            damping: 0.2
                        }
                    }
                }
            }
            ListModel {
                id: playlistModel
            }
        }
    }

    //проверка на дубликаты
    function hasDublicates()
    {
        var curIndex = playlistView.currentIndex
        playlistView.currentIndex = 0
        for (var i = 0; i < playlistModel.count; i++, playlistView.currentIndex++)
            if (playlistModel.get(playlistView.currentIndex)["path"] === arguments[0])
            {
                console.log("dublicate founded!")
                return true
            }
        return false
    }

    function uploadSongs() {
        var text = arguments[0]
        var curIndex = playlistView.currentIndex
        var urlsArr = text.split('\n')
        if (urlsArr.length === 1)
        {
            !hasDublicates(urlsArr[0]) ? playlistModel.append( { "path" : urlsArr[0], } ) : {} //заменить на playlistModel.append( { "path" : urlsArr[0], } )
            console.log(urlsArr[0])
        }
        else
        {
            for (var i=0; i<urlsArr.length-1; i++)
            {
                !hasDublicates(urlsArr[i]) ? playlistModel.append( { "path" : urlsArr[i], } ) : {}  //заменить на playlistModel.append( { "path" : urlsArr[i], } )
                console.log(urlsArr[i])
            }
        }
        playlistView.currentIndex = curIndex
    }
}

