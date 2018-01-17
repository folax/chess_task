import QtQuick 2.0
import QtQuick.Controls 1.4

ApplicationWindow {

    // Loaders;
    Loader {
        id: screen_1;
    }

    Loader {
        id: screen_2;
    }

    Loader {
        id: screen_3;
    }


    // Connections;
    Connections {
        target: screen_1.item

        onExitWindow: {
            close()
        }

        onShowscreen2: {
            screen_2.setSource("screen_2.qml")
        }

        onShowscreen3: {
            screen_3.setSource("screen_3.qml")
        }
    }

    Connections {
        target: screen_2.item

        onExitWindow: {
            screen_1.setSource("screen_1.qml")
        }
    }

    Connections {
        target: screen_3.item

        onExitWindow: {
            screen_1.setSource("screen_1.qml")
        }

        onOpenScreen2: {
            screen_2.setSource("screen_2.qml")
        }
    }

    // Run first window;
    Component.onCompleted:
    {
        screen_1.setSource("screen_1.qml")
    }
}
