import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Item {
    width: 640
    height: 480
    Image {
        anchors.fill: parent
        source:"qrc:/image/background.png"
    }

    Item {
        x: 40
        y: 40
        width: parent.width
        height: 30
        ExclusiveGroup {id: radioGroup}
        RadioButton {
            id: radio1
            anchors.left: parent.left
            anchors.leftMargin: 15
            exclusiveGroup: radioGroup
            text: qsTr("radioButton1")
            onClicked: {
                FrameWorkContext.changeUi(0);
            }
        }
        RadioButton {
            anchors.left: radio1.right
            anchors.leftMargin: 15
            exclusiveGroup: radioGroup
            checked: true
            text: qsTr("radioButton2")
            onClicked: {
                FrameWorkContext.changeUi(1);
            }
        }
    }

}
