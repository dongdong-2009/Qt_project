import QtQuick 2.4
import QtMultimedia 5.5
// 主画面坐标为： x: 350; y: 128

/*Item*/
Item {
//    property alias screen:screen
    id:screen
    width: 657; height: 511

    MediaPlayer{
        id:player
        //source: "file:///home/devin/Desktop/Repository/Qt_project/Qt_project/AutoPlayer-new/videos/Test2.mp4"
//        source: "file:///home/root/ThreeKingdoms.mp4"
        source: "file:///home/root/kone.mp4"
        autoLoad: false
        autoPlay: true    // 当AutoPlay 设置为true的时候，如果视频文件存在，就会直接播放视频
        volume: 0.4
        loops: MediaPlayer.Infinite
        onStopped: {
            console.log("the media is end!")
        }
    }
    VideoOutput {
        width: 657; height: 511
        anchors.fill: parent
        source: player
    }
}




