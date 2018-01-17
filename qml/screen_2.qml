import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

ApplicationWindow {
    id: screen_2
    title: qsTr("Chess_screen_2")
    visible: true
    width: 800
    height: 600
    color: "#5C74CD"
    signal exitWindow

    onClosing:
    {
        logic.clear()
        screen_2.exitWindow()
    }

    property int squareSize: 70

    property var images: [
        {'imgPath' : ":/images/black_pawn.png"},
        {'imgPath' : ":/images/white_pawn.png"},
        {'imgPath' : ":/images/black_rook.png"},
        {'imgPath' : ":/images/white_rook.png"},
        {'imgPath' : ":/images/black_knight.png"},
        {'imgPath' : ":/images/white_knight.png"},
        {'imgPath' : ":/images/black_bishop.png"},
        {'imgPath' : ":/images/white_bishop.png"},
        {'imgPath' : ":/images/black_queen.png"},
        {'imgPath' : ":/images/white_queen.png"},
        {'imgPath' : ":/images/black_king.png"},
        {'imgPath' : ":/images/white_king.png"}
    ]

    function getImageSource(type)
    {
        var path = "";
        if(type === 0)
            path = "qrc:/images/black_king.png"
        if(type === 1)
            path = "qrc:/images/black_queen.png"
        if(type === 2)
            path = "qrc:/images/black_rook.png"
        if(type === 3)
            path = "qrc:/images/black_bishop.png"
        if(type === 4)
            path = "qrc:/images/black_knight.png"
        if(type === 5)
            path = "qrc:/images/black_pawn.png"
        if(type === 6)
            path = "qrc:/images/white_king.png"
        if(type === 7)
            path = "qrc:/images/white_queen.png"
        if(type === 8)
            path = "qrc:/images/white_rook.png"
        if(type === 9)
            path = "qrc:/images/white_bishop.png"
        if(type === 10)
            path = "qrc:/images/white_knight.png"
        if(type === 11)
            path = "qrc:/images/white_pawn.png"
        return path
    }

    function setCellColor(data)
    {
        var color = ""
        var cnt = data
        if(cnt < 8)
        {
            color = data % 2 == 0 ? "grey" : "black"
        }
        else
        {
            cnt++
            color = data % 2 == 0 ? "grey" : "black"
        }
        return color
    }

    Item {
        id: gameBoard
        anchors.centerIn: parent
        x: 0
        y: 0
        width : logic.boardSize * squareSize
        height: logic.boardSize * squareSize

        Image {
            id: boardImage
            source: "/images/chess_board.jpg"
            height: gameBoard.height
            width: gameBoard.width
        }

        Repeater {
            model: logic

            delegate: Image {
                height: squareSize - 10
                width : squareSize - 10

                x: (squareSize * positionX) + 5
                y: (squareSize * positionY) + 5

                source: getImageSource(type)

                MouseArea {
                    anchors.fill: parent
                    drag.target: parent

                    property int startX: 0
                    property int startY: 0

                    onPressed: {
                        startX = parent.x;
                        startY = parent.y;
                    }

                    onReleased: {
                        var fromX = startX / squareSize;
                        var fromY = startY / squareSize;
                        var toX   = parent.x / squareSize;
                        var toY   = parent.y / squareSize;

                        if(!logic.move(fromX, fromY, toX, toY))
                        {
                            parent.x = startX;
                            parent.y = startY;
                        }
                    }
                }
            }
        }

        ColumnLayout {
            anchors.left: boardImage.right
            anchors.verticalCenter: boardImage.verticalCenter
            Layout.alignment: Qt.AlignVCenter

            Button {
                id: stopGameButton
                text: "Stop game"


                onClicked: {
                    screen_2.exitWindow()
                    logic.resetGame()
                    close()
                }
            }

            Button {
                id: saveGameButton
                text: "Save game"

                onClicked: {
                    if(logic.saveFile())
                        statusText.text = "File saved"
                    else
                        statusText.text = "File save error"
                }
            }

            Text {
                id: statusText
                color: "yellow"
            }
        }
    }
}

