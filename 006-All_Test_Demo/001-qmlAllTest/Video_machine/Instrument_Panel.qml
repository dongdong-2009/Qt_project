import QtQuick 2.0

Item {
    property string mWeekDayStr: ""
    property string mCurrentFloor: ""
    Image {
        id: panelBlackBg
        source: "qrc:/images/panel_Background/PanelBackground.png"
    }

    Item {
        width: 295
        height: 30
        x: 362
        y: 418 - 370
        Item {
            id: week_ID_Item
            width: 94
            height: parent.height
            Text {
                id: week_ID_text
                text: qsTr("周一")
                anchors.centerIn: parent
                color: "white"
                font.bold: true
                font.pixelSize: 16
            }
        }

        Image {
            id: timeBg_ID
            x: week_ID_Item.width
            width: 99
            height: parent.height
            source: "qrc:/images/panel_Background/Time_Bg.png"
            Item {
                width: 97
                height: parent.height
                Text {
                    id: time_TextID
                    text: DateAndTime.getTimeFormat(false)
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    font.pixelSize: 20
                }
            }
        }
        Item {
            x: week_ID_Item.width + timeBg_ID.width
            width: parent.width - week_ID_Item.width - timeBg_ID.width
            height: parent.height
            Image {
                id: weather_ID
                source: "qrc:/images/TimeAndWeather/Cloundy.png"
                anchors.centerIn: parent
            }
        }
    }

    Image {
        id: blackBg_ID
        x: 24
        y: 36 // 406 - 370 //  x: 24 //   y: 406
        width: 335
        height: 335
        source: "qrc:/images/floor/floor_Bg.png"
    }

    Image {
        id: frame_blue
        x: 24
        y: 36 // 406 - 370 //  x: 24 //   y: 406
        width: 336
        height: 335
        source: "qrc:/images/panel_Background/Arrow_Frame_Blue.png"
        transform: Rotation {
            id: minuteRotation
            origin.x: 336 / 2;
            origin.y: 335 / 2.0;
            NumberAnimation on angle {
                from: 0
                to: 360
                duration: 6000
                loops: Animation.Infinite
            }
        }
        Image {
            id: img_CircleDot
            source: "qrc:/images/arrow/circle_dot.png"
            anchors.centerIn: parent
        }
        Image {
            id: img_Circle
            source: "qrc:/images/arrow/circle.png"
            anchors.centerIn: parent
        }
    }

    Image {
        id: floorblackBg_ID
        x: 666
        y: 36
        width: 335
        height: 335
        source: "qrc:/images/floor/floor_Bg.png"
    }
    Image {
        // x: 666  y: 366
        id: floor_img
        x: 666
        y: 36
        width: 336
        height: 335
        source: "qrc:/images/panel_Background/Arrow_Frame_Blue.png"
        transform: Rotation {
            id: minuteRotation2
            origin.x: 336 / 2;
            origin.y: 335 / 2.0;
            NumberAnimation on angle {
                from: 0
                to: 360
                duration: 6000
                loops: Animation.Infinite
            }
        }

        Image {
            id: img_CircleDot2
            source: "qrc:/images/arrow/circle_dot.png"
            anchors.centerIn: parent
        }

        Image {
            id: img_Circle2
            source: "qrc:/images/arrow/circle.png"
            anchors.centerIn: parent
        }

        Image {
            id: small_circle
            source: "qrc:/images/floor/small_circle.png"
            anchors.centerIn: parent
        }

        Image {
            id: keli
            source: "qrc:/images/floor/keli.png"
            anchors.centerIn: parent
        }

        Image {
            id: half_circle
            source: "qrc:/images/floor/half_cricle.png"
            anchors.centerIn: parent
        }
    }

    Item {
        x: 666
        y: 36 - 14
        width: 336
        height: 335
        Image {
            property string mFloorPath: "qrc:/images/Floor_Panel/"
            id: showFloor
            anchors.centerIn: parent
            source: mFloorPath + "Floor_AllBlack.png"
        }
    }

    Image {
        id: earth
        x: 462 - 30
        y: 468 - 370
        source: "qrc:/images/earth/earth.png"
        transform: Rotation {             // (a)
            origin.x : 139 / 2.0
            origin.y : 139 / 2.0
            axis {
                x: 0
                y: 1
                z: 0                      // 绕y轴转动
            }
            NumberAnimation on angle {    // 定义角度angle上的动画
                from: 0
                to: 360
                duration: 20000
                loops: Animation.Infinite
            }
        }
    }
//    AnimatedImage {
//        id: earth
//        x: 462
//        y: 488 - 370
//        source: "qrc:/images/earth/gif_earth.gif"
//    }

    Image {
        id: current_floor_Text
        x: 796
        y: 662 - 370 // 662 - 370
        source: "qrc:/images/floor/current_floor.png"
    }

    Image {
        id: name
        x: 380 - 35
        y: 618 - 370
        source: "qrc:/images/floorMark/floorMark.png"
    }
    Image {
        id : logo_OTIS
        x: 280
        y: 708 - 370
        source: "qrc:/images/OTIS/OTIS_logoBg.png"
        Image {
            anchors.centerIn: parent
            source: "qrc:/images/OTIS/OTIS_logo.png"
            transform: Rotation {             // (a)
                origin.x : 80 / 2.0
                origin.y : 25 / 2.0
                axis {
                    x: 0
                    y: 1
                    z: 0                      // 绕y轴转动
                }
                NumberAnimation on angle {    // 定义角度angle上的动画
                    from: 0
                    to: 360
                    duration: 5000
                    loops: Animation.Infinite
                }
            }
        }
    }

    Connections {
        target: DateAndTime
        onSignalTimeAndDate: {
            time_TextID.text = pTime;
        }
    }
}
