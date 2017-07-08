import QtQuick 2.0
import QtQuick.Window 2.2

Window {
    id:win
    visible: true
    color: "black"
    width: 768; height: 1024
    contentOrientation: Qt.LandscapeOrientation

    ShowTitle {
        x:36; y:24
        width: 330; height: 50
    }

    ArrowAnimation {
        x:40; y:114
        width: 156; height: 174
    }
    FloorNumberShow {
        x:204; y:104
        width: 218; height: 182
    }
    ShowWifi {
        x:40; y:288
        width: 156; height: 78
    }

    DateandTime {
        x:614; y:30
        width: 140; height: 86
    }

    MultiMedia {
        x:0; y:384
        width: 768; height: 579
    }
    Rollingtext {
        x:0; y:962
        width: 768; height: 63
    }
}

