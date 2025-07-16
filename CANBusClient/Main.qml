import QtQuick
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    Column{
        Rectangle {
            width: 200
            height: 50

            border.color: "black"
            border.width: 1


            Text {
                   id: displayText
                   anchors.centerIn: parent
                   text: CanInterfaceClient.dynamicText  // Привязка к свойству C++
                   font.pixelSize: 18
               }
        }


    }
}
