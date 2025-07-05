import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "SCADA Client (Modbus+OPC UA)"

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text { text: scada ? "Analog 0: " + scada.analog0 : "Analog 0: N/A" }
        Text { text: scada ? "Analog 1: " + scada.analog1 : "Analog 1: N/A" }
        Text { text: scada ? "Digital 0: " + (scada.digital0 ? "ON" : "OFF") : "Digital 0: N/A" }
        Text { text: scada ? "Digital 1: " + (scada.digital1 ? "ON" : "OFF") : "Digital 1: N/A" }
    }
}
