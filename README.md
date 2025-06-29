# SCADA Server and Client (Modbus + OPC UA + Qt/QML)

This project demonstrates a minimal SCADA architecture using:

- `libmodbus` for Modbus communication (client ↔ server)
- `open62541` for OPC UA notification
- `Qt 6 + QML` for GUI (client-side)

## 🔧 Components

### ✅ `scada_server`
- Modbus TCP server (simulates analog and digital inputs)
- OPC UA server to notify updates from Modbus registers

### ✅ `scada_client`
- Modbus TCP client (writes analog/digital values)
- OPC UA client (subscribes to updates)
- Qt 6 / QML GUI to display live data

---

## ❗ Known Issue: Runtime Error

If you see this error when launching the client:
The procedure entry point nanosleep64 could not be located in the dynamic link library ...


💡 It means:
- `libmodbus.dll` was compiled using **MSYS2 MinGW**
- Your Qt application was built using **Qt's bundled MinGW**
- These runtimes are **incompatible**, causing symbol conflicts (e.g., duplicate `libwinpthread-1.dll`)

---

## 🛠️ Build Instructions (Client)

Make sure:
- `Qt 6.9.x` is installed
- `open62541` is integrated in your project
- `libmodbus` is rebuilt with Qt’s MinGW

Then open `scada_client.pro` in Qt Creator or build via CMake:
```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
