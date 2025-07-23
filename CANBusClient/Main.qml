import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    x: 950
    width: 900  // Увеличил ширину для размещения двух областей
    height: 600 // Увеличил высоту
    visible: true
    title: qsTr("Target Detection System")
    color: "#f0f0f0"

    // Заголовок окна
    Rectangle {
        id: titleBar
        anchors.top: parent.top
        width: parent.width
        height: 30
        color: "#333333"

        Text {
            text: root.title
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
            font.pixelSize: 12
        }

        // Кнопки управления окном
        Row {
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            Button {
                width: 20
                height: 20
                text: "—"
                font.pixelSize: 12
                background: Rectangle { color: "transparent" }
                onClicked: root.visibility = Window.Minimized
            }

            Button {
                width: 20
                height: 20
                text: "✕"
                font.pixelSize: 10
                background: Rectangle { color: "transparent" }
                onClicked: Qt.quit()
            }
        }
    }

    // Основной контент - разделён на две области
    Rectangle {
        anchors {
            top: titleBar.bottom
            bottom: statusBar.top
            left: parent.left
            right: parent.right
        }
        color: "#ffffff"

        // Разделитель областей
        Rectangle {
            id: splitter
            width: 1
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#e0e0e0"
        }

        // Левая область - визуализация целей
        Rectangle {
            id: visualizationArea
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: splitter.left
                margins: 10
            }

            // Заголовок области визуализации
            Text {
                id: visTitle
                text: "Визуализация целей"
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 14
                font.bold: true
                color: "#333333"
            }

            // Область для отрисовки прямоугольников целей
            Rectangle {
                id: canvasArea
                anchors {
                    top: visTitle.bottom
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "#f8f8f8"
                border.color: "#d0d0d0"
                border.width: 1

                // Холст для отрисовки целей
                Repeater {
                    model: TargetModel

                    Rectangle {
                        id: targetRect
                        x: model.x * canvasArea.width
                        y: model.y * canvasArea.height
                        width: model.w * canvasArea.width
                        height: model.h * canvasArea.height
                        color: Qt.rgba(0.2, 0.5, 0.8, 0.3)
                        border.color: "#1e88e5"
                        border.width: 2

                        // Информация о цели
                        Text {
                            anchors.bottom: parent.top
                            text: `ID: ${model.id} (${(model.probability * 100).toFixed(1)}%)`
                            font.pixelSize: 10
                            color: "#1565c0"
                            // background: Rectangle { color: "#e3f2fd"; opacity: 0.8 }
                        }

                        // Класс цели
                        Text {
                            anchors.centerIn: parent
                            text: model.class
                            font.pixelSize: 12
                            font.bold: true
                            color: "#0d47a1"
                        }
                    }
                }

                // Сетка для масштаба
                Canvas {
                    id: gridCanvas
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)
                        ctx.strokeStyle = "#e0e0e0"
                        ctx.lineWidth = 1

                        // Вертикальные линии
                        for (var x = 0; x <= width; x += width / 10) {
                            ctx.beginPath()
                            ctx.moveTo(x, 0)
                            ctx.lineTo(x, height)
                            ctx.stroke()
                        }

                        // Горизонтальные линии
                        for (var y = 0; y <= height; y += height / 10) {
                            ctx.beginPath()
                            ctx.moveTo(0, y)
                            ctx.lineTo(width, y)
                            ctx.stroke()
                        }

                        // Подписи осей
                        ctx.fillStyle = "#9e9e9e"
                        ctx.font = "8px sans-serif"

                        // X-axis labels
                        for (var i = 0; i <= 10; i++) {
                            var label = (i / 10).toFixed(1)
                            ctx.fillText(label, i * width / 10, height - 2)
                        }

                        // Y-axis labels
                        for (var j = 0; j <= 10; j++) {
                            label = (1 - j / 10).toFixed(1)
                            ctx.fillText(label, 2, j * height / 10 + 10)
                        }
                    }
                }
            }
        }

        // Правая область - таблица данных
        Rectangle {
            id: tableArea
            anchors {
                top: parent.top
                left: splitter.right
                right: parent.right
                margins: 10
            }

            height: parent.height * 0.7

            // Заголовок таблицы
            Rectangle {
                id: tableHeader
                anchors.top: parent.top
                width: parent.width
                height: 40
                color: "#f5f5f5"

                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: "#e0e0e0"
                }

                Row {
                    anchors.fill: parent
                    anchors.leftMargin: 15
                    anchors.rightMargin: 15
                    spacing: 10

                    Text {
                        text: "ID"
                        width: 50
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        font.bold: true
                        color: "#333333"
                    }

                    Text {
                        text: "Класс"
                        width: 120
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        font.bold: true
                        color: "#333333"
                    }

                    Text {
                        text: "Вероятность"
                        width: 100
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        font.bold: true
                        color: "#333333"
                    }

                    Text {
                        text: "Координаты"
                        width: 120
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        font.bold: true
                        color: "#333333"
                    }

                    Text {
                        text: "Размер"
                        width: 100
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        font.bold: true
                        color: "#333333"
                    }
                }
            }

            // Список целей
            ListView {
                id: targetListView
                anchors {
                    top: tableHeader.bottom
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                }
                clip: true
                model: TargetModel

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                    width: 6
                    background: Rectangle { color: "#f0f0f0" }
                }

                delegate: Rectangle {
                    id: itemDelegate
                    width: targetListView.width
                    height: 40
                    color: index % 2 === 0 ? "#ffffff" : "#fafafa"

                    // Содержимое элемента
                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 15
                        anchors.rightMargin: 15
                        spacing: 10

                        // ID цели
                        Text {
                            text: model.id
                            width: 50
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                            color: "#333333"
                        }

                        // Класс цели
                        Text {
                            text: model.class
                            width: 120
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                            color: "#333333"
                        }

                        // Вероятность
                        Text {
                            text: model.probability.toFixed(1) + "%"
                            width: 100
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                            color: model.probability > 0.7 ? "#2e7d32" :
                                   model.probability > 0.4 ? "#ed6c02" : "#d32f2f"
                        }

                        // Координаты
                        Text {
                            text: model.x.toFixed(2) + ", " + model.y.toFixed(2)
                            width: 120
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                            color: "#333333"
                        }

                        // Размеры
                        Text {
                            text: model.w.toFixed(2) + " × " + model.h.toFixed(2)
                            width: 100
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                            color: "#333333"
                        }
                    }

                    // Разделитель
                    Rectangle {
                        width: parent.width
                        height: 1
                        anchors.bottom: parent.bottom
                        color: "#e0e0e0"
                    }

                    // Выделение при наведении
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: itemDelegate.color = "#e3f2fd"
                        onExited: itemDelegate.color = index % 2 === 0 ? "#ffffff" : "#fafafa"
                    }
                }

                // Заглушка при отсутствии данных
                Text {
                    visible: targetListView.count === 0
                    anchors.centerIn: parent
                    text: "Обнаруженные цели отсутствуют"
                    font.pixelSize: 14
                    color: "#9e9e9e"
                }
            }
        }

        Rectangle{
            id: workModeArea
            anchors {
                top: tableArea.bottom
                bottom: parent.bottom
                left: splitter.right
                right: parent.right
                margins: 10
            }

            color: "#f8f8f8"
            border.color: "#d0d0d0"
            border.width: 1

            Column{
                spacing: 10

                Button {
                    text: "Режим 1"
                    onClicked: CanInterfaceClient.sendCommand(0)
                }
                Button {
                    text: "Режим 2"
                    onClicked: CanInterfaceClient.sendCommand(1)
                }
                Button {
                    text: "Режим 3"
                    onClicked: CanInterfaceClient.sendCommand(2)
                }

            }

        }
    }

    // Статус бар
    Rectangle {
        id: statusBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: 24
        color: "#f0f0f0"

        Rectangle {
            anchors.top: parent.top
            width: parent.width
            height: 1
            color: "#e0e0e0"
        }

        Text {
            text: "Целей: " + targetListView.count
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 11
            color: "#616161"
        }

        Text {
            text: Qt.formatDateTime(new Date(), "dd.MM.yyyy hh:mm:ss")
            anchors.right: parent.right
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 11
            color: "#616161"
        }
    }

    // Обработка перемещения окна
    MouseArea {
        anchors.fill: titleBar
        property point clickPos: Qt.point(0, 0)

        onPressed: function(mouse) {
            clickPos = Qt.point(mouse.x, mouse.y)
        }
        onPositionChanged: function(mouse) {
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
            root.x += delta.x
            root.y += delta.y
        }
    }
}
