import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

ApplicationWindow {
    id: screen_1
    title: qsTr("Chess_screen_1")
    visible: true
    width: Screen.desktopAvailableWidth / 2.5
    height: Screen.desktopAvailableHeight / 2.5
    color: "#5C74CD"

    // Signal;
    signal exitWindow
    signal showscreen2;
    signal showscreen3;

    onClosing:
    {
        screen_1.exitWindow()
    }

    // Functions;
    function makeChessBoard()
    {
        var color_1 = "white"
        var color_2 = "black"
        var tmp = ""
        var coordsArr = [ 8, 16, 24, 32, 40, 48, 56, 64 ]

        for(var i = 0; i < cellsRepeater.model; i++)
        {
            for(var j = 0; j < coordsArr.length; j++)
            {
                if(coordsArr[j] === i)
                {
                    tmp = color_1
                    color_1 = color_2
                    color_2 = tmp
                }
            }
            if(i % 2 === 0)
                cellsRepeater.itemAt(i).color = color_1
            else
                cellsRepeater.itemAt(i).color = color_2
        }
    }

    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter

        RowLayout {
            Layout.alignment: Qt.AlignHCenter

            Button {
                text: "New game"
                onClicked:
                {
                    screen_1.showscreen2()
                    screen_1.hide()
                }
            }

            Button {
                text: "Load game"
                onClicked:
                {
                    screen_1.showscreen3()
                    screen_1.hide()
                }
            }

            Button {
                text: "Exit"
                onClicked: close()
            }
        }

        Rectangle {
            id: cellsRect
            width: 320
            height: 320
            color: "green"

            Grid {
                rows: 8
                columns: 8

                Repeater {
                    id: cellsRepeater
                    model: 64

                    Rectangle {
                        width: 40
                        height: 40
                        border.color: "#0B3B0B"
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        makeChessBoard()
    }
}
