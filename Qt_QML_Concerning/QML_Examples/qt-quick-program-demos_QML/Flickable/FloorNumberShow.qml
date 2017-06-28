import QtQuick 2.0

// 主画面坐标为： x:18 ;  y: 451
Item {
    property alias floornum: floornum
    id: floor
    width: 261; height: 177
    Text {
        id: floornum
        font.bold: true
        font.pixelSize: 100
        color: "white"
        text: "999"
        anchors.centerIn: floor
    }
}

