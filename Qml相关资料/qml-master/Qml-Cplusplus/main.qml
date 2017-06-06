import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import lb2616.tools.RGBGame 1.0

Item {
    width: 395; height: 360;

    Text {
        id: timeLabel
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        font.pixelSize: 26
    }

    RGBGame {
        id: rgbGame
        color: Qt.blue
    }

    Rectangle {
        id: colorRect
        anchors.centerIn: parent
        width: 200
        height: 200
        color: "yellow"
    }

    Button {
        id: start
        text: "start"
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        onClicked: rgbGame.start()
    }

    Button {
        id: stop
        text: "stop"
        anchors.left: start.right
        anchors.leftMargin: 5
        anchors.bottom: start.bottom
        onClicked: rgbGame.stop()
    }

    function changeAlgorithm(button, algorithm)
    {
        switch(algorithm)
        {
            case 0:
                button.text = "RandomRGB";
                break;
            case 1:
                button.text = "RandomRed"
                break;
            case 2:
                button.text = "RandomGreen"
                break;
            case 3:
                button.text = "RandomBlue"
                break;
            case 4:
                button.text = "LinearIncrease"
                break;
        }
    }

    Button {
        id: colorAlgorithm
        text: "RandomRGB"
        anchors.left: stop.right
        anchors.leftMargin: 5
        anchors.bottom: start.bottom

        onClicked: {
            var algorithm = (rgbGame.algorithm() + 1)%5
            changeAlgorithm(colorAlgorithm, algorithm)
            rgbGame.setAlgorithm(algorithm)
        }
    }

    Button {
        id: quit
        text: "quit"
        anchors.left: colorAlgorithm.right
        anchors.leftMargin: 5
        anchors.bottom: start.bottom
        onClicked: Qt.quit()
    }

    Component.onCompleted: {
        rgbGame.color = Qt.rgba(0, 180, 120, 255)
        rgbGame.setAlgorithm(rgbGame.LinearIncrease)
        changeAlgorithm(colorAlgorithm, rgbGame.algorithm())
    }

    Connections {
        target: rgbGame
        onCurrentTime: {
            timeLabel.text = strTime
            timeLabel.color = rgbGame.timeColor
        }
    }

    Connections {
        target: rgbGame
        onColorChanged: colorRect.color = color
    }
}
