import glob
import pathlib
import os
import shutil
import subprocess
import sys
import tarfile
import urllib.request

def exe(cmd: str) -> None:
    subprocess.call(cmd.split());

def mkdir(path: str) -> None:
    if not os.path.exists(path): os.makedirs(path)

def rm(path: str) -> None:
    if not os.path.exists(path): return
    if os.path.isdir(path): shutil.rmtree(path)
    else: os.remove(path)

def cp(source: str, destination: str) -> None:
    shutil.copy(source, destination)

def exeExists(exe: str) -> bool:
    return shutil.which(exe) != None

def globDir(dirpath: str) -> list[str]:
    res = []
    res = glob.glob(dirpath + '/**/*.c', recursive=True)
    res += glob.glob(dirpath + '/**/*.h', recursive=True)
    return res

def main(args: list[str]) -> None:
    if len(args) == 0:
        print("no args provided")
        return

    if args[0] == "ctemplate":
        mkdir("./gen")
        exe("ctemplate ./test/types.ctypes ./src/zsl.ht ./gen/zsl.h")

    if args[0] == "proj":
        main(["ctemplate"])
        exe("cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug")
        if sys.platform != "win32": cp("./build/compile_commands.json", "./compile_commands.json")

    if args[0] == "test":
        main(["proj"])
        exe("cmake --build build")
        exe("./build/ztest")

    if args[0] == "format":
        paths = globDir("./src") + globDir("./test")
        for path in paths:
            exe("clang-format -i --style=file " + path)

    if args[0] == "tidy":
        for path in globDir("./src"):
            exe("clang-tidy " + path)

    if args[0] == "check":
        main(["clean"])
        main(["ctemplate"])
        main(["proj"])
        main(["format"])
        main(["tidy"])
        exe("cmake --build build")
        exe("./build/ztest")

    if args[0] == "clean":
        rm("./build")
        rm("./gen")

main(sys.argv[1:])
