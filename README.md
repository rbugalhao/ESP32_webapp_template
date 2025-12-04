# ESP32-C5 Web Control Project

Before compiling the project, it's necessery to generate the `web_assets.h` file from the `web/` folder.

Execute:

```bash
python embed_web.py
```

Compile:

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c5 .
```

Upload:

```bash
arduino-cli upload -p COM3 --fqbn esp32:esp32:esp32c5 .
```
