import QtQuick
import QtQuick.Controls
import App.Core 1.0

Window {
    width: 640
    height: 580
    visible: true
    title: qsTr("Target Generator")

    property var lastTarget: null

    Column {
        id: mainColumn
        spacing: 10
        padding: 10
        width: parent.width

        // Блок для отображения последней цели
        Rectangle {
            id: targetDisplay
            width: parent.width - 20
            height: 300
            color: "#f0f0f0"
            border.color: "gray"
            border.width: 1

            Rectangle {
                id: targetVisual
                visible: lastTarget !== null
                x: lastTarget ? lastTarget.x : 0
                y: lastTarget ? lastTarget.y : 0
                width: lastTarget ? lastTarget.width : 0
                height: lastTarget ? lastTarget.height : 0
                color: Qt.rgba(0.2, 0.6, 1.0, 0.5)
                border.color: "blue"
                border.width: 2

                Column {
                    anchors.centerIn: parent
                    spacing: 5
                    Text {
                        text: lastTarget ? "Class: " + lastTarget.class : ""
                        font.bold: true
                        color: "white"
                    }
                    Text {
                        text: lastTarget ? "Probability: " + lastTarget.probability + "%" : ""
                        color: "white"
                    }
                }
            }

            Text {
                anchors.centerIn: parent
                text: "No targets generated yet"
                visible: lastTarget === null
                color: "gray"
                font.italic: true
            }
        }

        // Блок с информацией о последней цели (теперь с прокруткой)
        ScrollView {
            id: infoScroll
            width: parent.width - 20
            height: Math.min(infoContent.implicitHeight + 20, 150) // Ограничиваем максимальную высоту
            clip: true

            TextArea {
                id: infoContent
                width: infoScroll.width - 20
                text: lastTarget ? JSON.stringify(lastTarget, null, 2) : "No target data"
                font.family: "Courier New"
                wrapMode: Text.Wrap
                readOnly: true
                background: Rectangle {
                    color: "#e8e8e8"
                    border.color: "lightgray"
                    border.width: 1
                }
                padding: 5
                selectByMouse: true
            }
        }

        // Кнопка генерации цели


        // Блок режимов
        Row {


            spacing: 10

            Button {
                id: aimGenButton
                text: "Сгенерировать цель"
                width: 200
                height: 40
                onClicked: {
                    lastTarget = {
                        "type": "target",
                        "x": Math.floor(Math.random() * (targetDisplay.width - 50)),
                        "y": Math.floor(Math.random() * (targetDisplay.height - 50)),
                        "width": Math.floor(30 + Math.random() * 100),
                        "height": Math.floor(30 + Math.random() * 100),
                        "class": Math.floor(Math.random() * 80),
                        "probability": Math.floor(Math.random() * 101),
                        "options": [1, 2, 3]
                    };
                    CanInterface.handleText(lastTarget);
                }
            }

            Rectangle {
                id: mode1rootRect
                objectName: "mode1rootRect"
                property alias mode1RectColor: mode1rootRect.color
                color: IntegrationModule.mode === IntegrationModule.Mode.DEZ ? "crimson" : "white"
                width: mode1label.width + 20  // Ширина Label + отступы 10 с каждой стороны
                height: mode1label.height + 20

                Label {
                    id: mode1label
                    text: "Режим 1"
                    anchors.centerIn: parent
                }
            }
            Rectangle {
                id: mode2rootRect
                objectName: "mode2rootRect"
                property alias mode1RectColor: mode2rootRect.color
                color: IntegrationModule.mode === IntegrationModule.Mode.BOEVOI ? "crimson" : "white"
                width: mode2label.width + 20  // Ширина Label + отступы 10 с каждой стороны
                height: mode2label.height + 20

                Label {
                    id: mode2label
                    text: "Режим 2"
                    anchors.centerIn: parent
                }
            }


            Rectangle {
                id: mode3rootRect
                objectName: "mode3rootRect"

                property alias mode1RectColor: mode3rootRect.color
                color: "white"
                width: mode3label.width + 20  // Ширина Label + отступы 10 с каждой стороны
                height: mode3label.height + 20

                Label {
                    id: mode3label
                    text: "Режим 3"
                    anchors.centerIn: parent
                }
            }
        }


    }
}
