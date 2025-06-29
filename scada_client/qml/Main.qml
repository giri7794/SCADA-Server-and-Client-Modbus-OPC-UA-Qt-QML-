import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "SCADA Viewer"

    Column {
        spacing: 10
        anchors.centerIn: parent

        Text { text: "Analog 0: " + scada.analog0 }
        Text { text: "Analog 1: " + scada.analog1 }
        Text { text: "Digital 0: " + (scada.digital0 ? "ON" : "OFF") }
        Text { text: "Digital 1: " + (scada.digital1 ? "ON" : "OFF") }
    }
}
