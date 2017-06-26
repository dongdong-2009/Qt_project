import QtQuick 2.0

Item {
    Image {
        id: arrowdownImage
        source: "images/ArrowDn.png"
        SequentialAnimation on y {
            id: arrowDownAnim
            running: false
            from
        }
    }
}

