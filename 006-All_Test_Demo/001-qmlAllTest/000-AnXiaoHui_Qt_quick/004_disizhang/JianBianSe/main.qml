import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    JianBianSe {
        x: 50
        y: 50
    }
    Z_ProPertyTest {
        x: 150
        y: 150
    }
    Item_Test {
        x: 200
        y: 200
    }
}
