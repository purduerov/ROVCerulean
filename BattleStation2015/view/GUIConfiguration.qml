import QtQuick 2.4
import QtQuick.Controls 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.0

ROVCard {
    id: configuration
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 10

    height: (parent.height/2) - 10

    title: "Configuration"
    headerColor: mainColor

    //Serial and Controller Config
    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        spacing: 10

        Label {
            text: "Serial Device:"
            anchors.left: parent.left;
            anchors.right: parent.right
            font.bold: true
            font.pixelSize: 18
            color: "white"
        }

        ComboBox {
            id: serialCombo
            height: 30
            anchors.left: parent.left;
            anchors.right: parent.right
            model: controller.SerialDevices
            enabled: !controller.Running
            onModelChanged: if (count == 1) currentIndex = 0;
            onCurrentIndexChanged: controller.SerialSelect(currentIndex-1)
        }

        Label {
            text: "Joystick 1:"
            anchors.left: parent.left;
            anchors.right: parent.right

            font.bold: true
            font.pixelSize: 18
            color: "white"
        }

        ComboBox {
            id: joystick1Combo
            height: 30
            anchors.left: parent.left;
            anchors.right: parent.right;
            model: controller.JoystickDevices
            enabled: !controller.Running
            onModelChanged: if (count == 1) currentIndex = 0;
            onCurrentIndexChanged: controller.Joystick1Select(currentIndex-1)
        }

        Label {
            text: "Joystick 2:"
            anchors.left: parent.left;
            anchors.right: parent.right
            font.bold: true
            font.pixelSize: 18
            color: "white"
        }

        ComboBox {
            id: joystick2Combo
            height: 30
            anchors.left: parent.left;
            anchors.right: parent.right
            model: controller.JoystickDevices
            enabled: !controller.Running
            onModelChanged: if (count == 1) currentIndex = 0;
            onCurrentIndexChanged: controller.Joystick2Select(currentIndex-1)
         }


        Row {
            width: 320
            height: 30
            spacing: 20

            anchors.horizontalCenter: parent.horizontalCenter

            ROVButton {
                id: refresh
                height: 30
                width: 150
                text: "Refresh"
                fontSize: 20
                enabled: !controller.Running
                onClicked: controller.RefreshLists()
            }

            ROVButton {
                id: serialControl
                height: 30
                width: 150

                text: (!controller.Running) ? "Connect" : "Stop"
                enabled: (serialCombo.count > 1 && serialCombo.currentIndex >= 1 && joystick1Combo.count > 1 && joystick1Combo.currentIndex >= 1)
                fontSize: 20
                onClicked: {
                    if (serialCombo.count > 1 && serialCombo.currentIndex >= 1 && joystick1Combo.count > 1 && joystick1Combo.currentIndex >= 1)
                        controller.Running = !controller.Running
                }
            }
        }
    }
}
