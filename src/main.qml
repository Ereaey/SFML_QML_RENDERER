import QtQuick 2.15
import QtQuick.Window 2.15

import SFML 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "yellow"
    View
    {
        id:viewSFML
        anchors.fill: parent
    }
    Rectangle
    {
        color: "red"
        anchors.bottom: parent.bottom

        height: 30
        width: parent.width
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                viewSFML.updateItem();
            }
        }
    }
}
