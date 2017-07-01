import QtQuick 2.0
import lb2616.tools.ShowFloor 1.0
// 主画面坐标为： x:18 ;  y: 451
Item {
    property alias floornum: floornum
    signal floorChanged(int fnum)
    id: floor
    width: 351; height: 173

    ShowFloor { // 导入C++ 类对象
        id: showfloor
    }

    Text {
        id: floornum
        font.family: "KONE Information_v12"
        font.pixelSize: 260
        color: "white"
        text: "1"
        anchors.centerIn: floor
    }

    Connections {
        target: showfloor
        onSendSignalFloor: {
            floornum.text = index
            emit: floorChanged(index)
        }
    }
//    function hidepicture(i)
//    {
//        if (i === 9)

//    }
}

