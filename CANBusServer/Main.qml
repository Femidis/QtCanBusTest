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


            TextInput {
                id: myTextInput
                anchors.fill: parent
                focus: true  // автоматически фокусируется при загрузке
                text: "Hello World!"
                font.pixelSize: 16
                color: "black"
                selectionColor: "lightblue"

                // Обработка нажатия Enter
                onAccepted: {
                    console.log("Введённый текст:", text)
                }
            }
        }

        Button {

            text: "Отправить через сигнал"
            onClicked: {
                // Генерируем JS-сигнал и подключаем к C++ слоту
                CanInterface.handleText(myTextInput.text)
            }
        }
    }
}
