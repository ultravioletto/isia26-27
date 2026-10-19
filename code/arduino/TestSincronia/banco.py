"""Banco di prova per TestSincronia: pilota i 3 CoreS3 via seriale e misura.

Uso: python banco.py [file.log]     (serve pyserial; porte nel dizionario PORTE)
Dura circa 7 minuti. Aprire le porte può riavviare i dispositivi."""
import re, sys, threading, time, statistics as st
import serial

PORTE = {"A": "COM4", "B": "COM3", "C": "COM5"}   # id 49, 53, 81
T0 = time.perf_counter()
righe = []            # (t_pc, nome, riga)
lock = threading.Lock()
porte = {}
log = open(sys.argv[1] if len(sys.argv) > 1 else "banco.log", "w", encoding="utf-8")


def ora():
    return time.perf_counter() - T0


def apri(nome):
    s = serial.Serial()
    s.port, s.baudrate, s.timeout = PORTE[nome], 115200, 0.002
    s.dtr = False
    s.rts = False
    s.open()
    return s


def lettore(nome):
    buf = b""
    while True:
        try:
            if nome not in porte:
                porte[nome] = apri(nome)
                with lock:
                    righe.append((ora(), nome, "#porta aperta"))
            d = porte[nome].read(max(1, porte[nome].in_waiting))
        except Exception as e:
            porte.pop(nome, None)
            time.sleep(0.3)
            continue
        t = ora()
        buf += d
        while b"\n" in buf:
            r, buf = buf.split(b"\n", 1)
            r = r.decode("utf-8", "replace").strip()
            with lock:
                righe.append((t, nome, r))
            log.write(f"{t:9.3f} {nome} {r}\n")


def manda(nome, cmd):
    log.write(f"{ora():9.3f} {nome} >>> {cmd}\n")
    for _ in range(20):
        try:
            porte[nome].write((cmd + "\n").encode())
            return
        except Exception:
            time.sleep(0.3)
    print(f"  !! impossibile mandare '{cmd}' a {nome}")


def aspetta(s):
    time.sleep(s)


def stato():
    da = ora()
    for n in PORTE:
        manda(n, "stato")
    time.sleep(0.4)
    out = {}
    with lock:
        for t, n, r in righe:
            if t >= da and " stato " in r:
                m = re.search(r"stato (\S+) sess (\d+) orig (\d+) passo (-?\d+) err (\S+) scarto (\S+) vicini (\d+)", r)
                if m:
                    out[n] = dict(ruolo=m[1], sess=int(m[2]), orig=int(m[3]), passo=int(m[4]), vicini=int(m[7]))
    for n in PORTE:
        s = out.get(n)
        print(f"    {n}: " + (f"{s['ruolo']:12s} sess {s['sess']:5d} orig {s['orig']:3d} passo {s['passo']:5d} vicini {s['vicini']}" if s else "nessuna risposta"))
    return out


def eventi(da, a=None, filtro=None):
    with lock:
        sel = [(t, n, r) for t, n, r in righe if t >= da and (a is None or t <= a)]
    for t, n, r in sel:
        if r.startswith("B ") or r.startswith("#"):
            continue
        if " stato " in r:
            continue
        if filtro and not re.search(filtro, r):
            continue
        print(f"    {t:7.2f}s {n}  {r}")


def allineamento(da, a):
    """Per ogni battito sentito da 2+ dispositivi: dispersione degli istanti
    ricostruiti (arrivo su PC meno ritardo del loop). Include il jitter USB."""
    gruppi = {}
    with lock:
        for t, n, r in righe:
            if da <= t <= a and r.startswith("B "):
                p = r.split()
                if len(p) == 4:
                    gruppi.setdefault((int(p[1]), int(p[2])), {})[n] = (t - float(p[3]) / 1000, float(p[3]))
    disp, rit = [], []
    for k, g in gruppi.items():
        rit += [v[1] for v in g.values()]
        if len(g) >= 2:
            ts = [v[0] * 1000 for v in g.values()]
            disp.append(max(ts) - min(ts))
    if not disp:
        print("    nessun battito comune")
        return
    disp.sort()
    print(f"    battiti comuni {len(disp)}: dispersione mediana {st.median(disp):.1f} ms, "
          f"90% {disp[int(len(disp) * .9) - 1]:.1f} ms, max {disp[-1]:.1f} ms")
    print(f"    ritardo del loop sul battito: mediana {st.median(rit):.1f} ms, max {max(rit):.1f} ms")


def errori_sync(da, a):
    e = []
    with lock:
        for t, n, r in righe:
            if da <= t <= a:
                m = re.search(r"err=(\S+) ms", r)
                if m:
                    e.append(abs(float(m[1])))
    if e:
        print(f"    err SYNC: {len(e)} campioni, mediana {st.median(e):.2f} ms, max {max(e):.2f} ms")


def titolo(s):
    print(f"\n=== {s}  (t={ora():.1f}s)")


for n in PORTE:
    threading.Thread(target=lettore, args=(n,), daemon=True).start()
time.sleep(3)
for n in PORTE:
    manda(n, "fine")
    manda(n, "durata 600")   # le prove T1..T9 non devono finire per durata
    manda(n, "battiti 1")
time.sleep(1)

titolo("T1 ingresso: A avvia, B e C devono entrare")
t = ora(); manda("A", "avvia"); aspetta(5)
eventi(t); stato()

titolo("T2 regime per 30 s")
t = ora(); aspetta(30)
allineamento(t, ora()); errori_sync(t, ora())

titolo("T3 C isolato 10 s (sotto la soglia di uscita)")
t = ora(); manda("C", "isola 10000"); aspetta(16)
eventi(t); stato()
t2 = ora(); aspetta(10); allineamento(t2, ora())

titolo("T4 B in pausa 3 s (loop fermo)")
t = ora(); manda("B", "pausa 3000"); aspetta(10)
eventi(t); allineamento(t + 5, ora())

titolo("T5 origine A isolata 10 s: B deve prendere l'orologio, poi A riprende")
t = ora(); manda("A", "isola 10000"); aspetta(18)
eventi(t); stato()
t2 = ora(); aspetta(10); allineamento(t2, ora())

titolo("T6 origine in pausa 8 s (loop fermo)")
s = stato(); orig = next((n for n, v in s.items() if v["ruolo"] == "ORIGINE"), "A")
print(f"    origine attuale: {orig}")
t = ora(); manda(orig, "pausa 8000"); aspetta(16)
eventi(t); stato()
t2 = ora(); aspetta(10); allineamento(t2, ora())

def fini(da):
    """Istanti (sul PC) in cui ogni dispositivo ha chiuso la sessione per durata o FINE."""
    with lock:
        f = [(t, n, r) for t, n, r in righe if t >= da and "fine sessione" in r]
    for t, n, r in f:
        print(f"    {t:7.3f}s {n}  {r}")
    if len(f) >= 2:
        ts = [t for t, n, r in f]
        print(f"    tutte le fini entro {(max(ts) - min(ts)) * 1000:.1f} ms (include il jitter USB)")


titolo("T7 C isolato 20 s: non esce, suona da solo e si riallinea")
t = ora(); manda("C", "isola 20000"); aspetta(26)
eventi(t); stato()

titolo("T8 C riavviato")
t = ora(); manda("C", "riavvia"); aspetta(10)
manda("C", "battiti 1"); manda("C", "durata 600")
eventi(t); stato()
t2 = ora(); aspetta(10); allineamento(t2, ora())

titolo("T9 B e C avviano insieme: vince B (id 53 < 81)")
for n in PORTE:
    manda(n, "fine")
aspetta(3)
t = ora(); manda("B", "avvia"); manda("C", "avvia"); aspetta(8)
eventi(t); stato()
t2 = ora(); aspetta(10); allineamento(t2, ora())

titolo("D1 durata 40 s: tutti escono allo stesso passo, anche C che non sente la radio")
for n in PORTE:
    manda(n, "fine")
    manda(n, "durata 40")
aspetta(2)
t = ora(); manda("A", "avvia"); aspetta(30)
manda("C", "isola 15000")   # alla fine C non sente FINE ne SYNC
aspetta(14)
fini(t); stato()

titolo("D2 durante il riposo il tocco non fa niente")
t = ora(); manda("B", "avvia"); aspetta(2)
eventi(t, filtro="riposo|->"); stato()

titolo("D3 dopo il riposo si riparte")
aspetta(30)
t = ora(); manda("B", "avvia"); aspetta(5)
eventi(t, filtro="->"); stato()

titolo("D4 a una sessione con meno di 10 s residui non si entra")
for n in PORTE:
    manda(n, "fine")
    manda(n, "riposo 1000")
aspetta(2)
manda("A", "durata 20")
t = ora(); manda("C", "isola 13000"); manda("A", "avvia")   # C torna con 7 s residui
aspetta(16)
eventi(t, filtro="->|fine sessione"); stato()
aspetta(6)

titolo("deriva lunga: A avvia, 3 minuti di regime (durata 240 s)")
for n in PORTE:
    manda(n, "fine")
    manda(n, "riposo 30000")
manda("A", "durata 240")
aspetta(2)
t = ora(); manda("A", "avvia"); aspetta(5)
t2 = ora(); aspetta(180)
allineamento(t2, ora()); errori_sync(t2, ora()); stato()
for n in PORTE:
    manda(n, "battiti 0")
    manda(n, "fine")
time.sleep(0.5)
log.close()
