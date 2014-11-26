#!/usr/bin/scons

env = Environment(platform="win32", tools=["mingw"])

CCFLAGS = [
    "-Iinclude"
]

source = [
    "src/serial.c"
]

libserial = env.StaticLibrary(source=source, target="serial", CCFLAGS=CCFLAGS)

test = env.Program(source="test.c", CCFLAGS=CCFLAGS, LIBS=[libserial])

Default([libserial, test])
