"""
Rigenera i file app_*.h della Raccolta a partire dagli sketch originali.

Ogni sketch viene passato al preprocessore di Arduino (arduino-cli compile
--preprocess), che aggiunge i prototipi delle funzioni come succede quando lo
compili da solo. Poi le righe #include vanno in cima e tutto il resto viene
chiuso in un namespace, così gli sketch convivono nello stesso firmware
anche se usano gli stessi nomi (setup, loop, W, H, tela...).

Uso: dopo aver modificato uno sketch originale, lancia
    python aggiorna.py
e ricompila Raccolta. I file app_*.h non vanno modificati a mano.
"""

import os
import subprocess
import sys

QUI = os.path.dirname(os.path.abspath(__file__))
ARDUINO = os.path.dirname(QUI)
FQBN = "m5stack:esp32:m5stack_cores3"

# namespace, cartella dello sketch (l'ordine conta: l'Esploratore include SD.h prima di M5CoreS3.h)
APP = [
    ("esploratore", "EsploratoreCoreS3"),
    ("sonora", "ProvaSonora"),
    ("sincronia", "TestSincronia"),
    ("blob", "ProvaBlob"),
    ("sciame", "ProvaSciame"),
    ("scatola", "ProvaScatola"),
    ("cimatica", "ProvaCimatica"),
    ("interferenze", "InterferencePatterns"),
]

CLI_POSSIBILI = [
    r"C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe",
    os.path.expandvars(r"%LOCALAPPDATA%\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"),
    "arduino-cli",
]


def trova_cli():
    for c in CLI_POSSIBILI:
        if c == "arduino-cli" or os.path.isfile(c):
            return c
    sys.exit("arduino-cli non trovato")


def genera(cli, ns, cartella):
    sketch = os.path.join(ARDUINO, cartella)
    r = subprocess.run([cli, "compile", "--preprocess", "--fqbn", FQBN, sketch],
                       capture_output=True, text=True, encoding="utf-8")
    if r.returncode != 0:
        sys.exit(f"{cartella}: il preprocessore ha dato errore\n{r.stderr}")
    include, corpo = [], []
    for riga in r.stdout.splitlines():
        if riga.lstrip().startswith("#include"):
            include.append(riga.strip())
        else:
            corpo.append(riga)
    testo = [
        f"// Generato da aggiorna.py a partire da {cartella}/{cartella}.ino: non modificare qui.",
        "#pragma once",
        *include,
        f"namespace {ns} {{",
        *corpo,
        f"}}  // namespace {ns}",
        "",
    ]
    with open(os.path.join(QUI, f"app_{ns}.h"), "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(testo))
    print(f"app_{ns}.h  <-  {cartella}")


if __name__ == "__main__":
    cli = trova_cli()
    for ns, cartella in APP:
        genera(cli, ns, cartella)
