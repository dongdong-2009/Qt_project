import QtQuick 2.7
import QtQuick.Window 2.2

Window {
    visible: true
    width: 1027
    height: 768
    title: qsTr("Hello World")

    BackGroundVideo {
        width: parent.width
        height: parent.height
    }

    Instrument_Panel {
        x: 0
        y: 370
    }

    Text {
        text: "6"
        x: parent.width / 2.0
        y: parent.height / 2.0 - 150
        font.bold: true
        font.pixelSize: 30
        color: "white"
    }
}
