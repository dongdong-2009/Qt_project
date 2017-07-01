import QtQuick 2.0

Item {
    property string coverImag: 'images/kone_arrow_up.png'
    id: id_arrow
    width: 156; height: 171
    anchors.fill: parent
    clip:true
    Image {
        id:arrorimage
        width: 156
        height: 171
        source: coverImag
    }
    Image {
        width: 156
        height: 171
        source: coverImag
        y:171
    }
    NumberAnimation {
        id:animatUp
        target:id_scrollArrow
        property: "y"
        from:0; to:-id_arrow.height ;
        alwaysRunToEnd: true
        duration: 1000 //l_speedLeve;
        loops: Animation.Infinite
    }

    NumberAnimation {
        id:animatDown
        target:id_scrollArrow
        property: "y"
        alwaysRunToEnd: true
        from: -id_arrow.height; to: 0;
        duration: 1000 //l_speedLeve;
        loops: Animation.Infinite
    }
//    onFloorChanged: {
//        if (fnum == 9)
//        {
//            coverImag = 'kone_arrow_down.png'
//            arrorimage.source = coverImag
//        }
//    }

    function animationCtrl(){
        if(orienteRun){
            animatDown.stop();
            animatUp.start();
        }else{
            animatUp.stop();
            animatDown.start();
        }
    }
}

