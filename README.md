# ESP32-C5 Web Control Project

Antes de compilar o projeto, é necessário gerar o ficheiro `web_assets.h` a partir da pasta `web/`.

Execute:

```bash
python embed_web.py
```

Compilar:

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c5 .
```

Upload:

```bash
arduino-cli upload -p COM3 --fqbn esp32:esp32:esp32c5 .
```
