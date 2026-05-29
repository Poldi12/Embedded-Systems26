# Embedded-Systems26

ESP32 project using PlatformIO + Arduino framework.

## VSCode Extensions

- **PlatformIO IDE** (`platformio.platformio-ide`) — provides build, upload, and serial monitor for ESP32. Automatically installs the ESP32 toolchain and Arduino framework on first build — no Arduino CLI or separate SDK needed.

## Linux Setup

### 1. Install udev rules
```bash
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
sudo udevadm control --reload-rules
sudo udevadm trigger
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
```

### 2. Log out and back in
Required for the group changes from step 1 to take effect.

### 3. Upload
Connect the ESP32 via USB, then in VSCode click the **→ Upload** button in the PlatformIO toolbar, or run:
```bash
pio run --target upload
```

The device will appear as `/dev/ttyUSB0`.

## Causes for Errors

### BRLTTY conflict
On Ubuntu/Debian, the BRLTTY (Braille display driver) claims the CH341 USB-serial chip used by many ESP32 boards, disconnecting it before PlatformIO can upload. Fix:
```bash
sudo apt remove brltty
```
Replug the ESP32 afterwards.
