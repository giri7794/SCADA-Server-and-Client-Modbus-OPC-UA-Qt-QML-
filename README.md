# SCADA Server and Client (Modbus + OPC UA + Qt/QML)

This project demonstrates a minimal SCADA architecture using:

- `libmodbus` for Modbus communication (client ↔ server)
- `open62541` for OPC UA notification
- `Qt 6 + QML` for GUI (client-side)

---

## 🔧 Components

### ✅ `scada_server`
- Modbus TCP server (simulates analog and digital inputs)
- OPC UA server exposes those registers as nodes
- Sends notifications to OPC UA clients on Modbus data updates

---

## ✅ `scada_client` (Console-based Client)
- Modbus TCP client (writes analog and digital values periodically)
- OPC UA client subscribes to updates from server
- Console-based, useful for testing communication logic without GUI

### 🔄 Use Case:
> Embedded SCADA client running in a headless environment for testing or logging purposes.

---

## ✅ `qtonly_scada_client` (Qt/QML GUI Client)
- Uses `QModbusTcpClient` via `QtSerialBus` to write Modbus registers
- Uses `QOpcUaClient` to subscribe to register updates from server
- QML GUI displays real-time analog/digital values

### 🖥️ Use Case:
> Interactive SCADA operator panel for monitoring and controlling analog/digital values via GUI.

---

## 🛠️ Build Instructions (Qt GUI Client)

Make sure:
- `Qt 6.9.x` is installed with `SerialBus` and `OpcUa` modules
- `open62541` is integrated into your Qt installation
- `libmodbus` is rebuilt using the same Qt MinGW toolchain

Then build using:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
