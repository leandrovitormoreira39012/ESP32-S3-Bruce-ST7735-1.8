import os
import subprocess
from pathlib import Path

patch_file = Path("patches/fastled_fix.patch")
lib_dir = Path(".pio/libdeps/esp32-s3-st7735/FastLED")

if lib_dir.exists():
    os.chdir(lib_dir)
    result = subprocess.run(
        ["git", "apply", "../../../../patches/fastled_fix.patch"],
        capture_output=True,
        text=True
    )
    if result.returncode == 0:
        print("✅ Patch aplicado com sucesso!")
    else:
        print("⚠️ Já estava aplicado ou erro ignorado")
else:
    print("ℹ️ Biblioteca ainda não baixada — pronto para aplicar na compilação")
