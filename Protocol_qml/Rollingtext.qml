import QtQuick 2.0

Rectangle {
    id: retScroll
    width: 734
    height: 83
    color: "transparent"  // x:292;y:688
    Text {
        id:idtext
        height:parent.height
        text:"欢迎光临上海贝思特"
        color:"white"
        font.bold: true          // 文字加粗
        font.pixelSize: 24       // 文字大小
        verticalAlignment: Text.AlignVCenter      // 水平居中
//        anchors.centerIn: parent;  // 这样设置将没有动画
        SequentialAnimation on x {
        loops: Animation.Infinite
            PropertyAnimation {
                from:293
                to: 1024
                duration: 15000      // 滚动的文字运行的时间长度
            }
       }
    }
}

