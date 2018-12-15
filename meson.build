project('libserial', 'c', version: '1.0')

sources = [
]

if target_machine.system() == 'windows'
    sources += 'src/serial_win32.c'
    sources += 'contrib/usleep.c'
endif

if target_machine.system() == 'linux'
    sources += 'src/serial_linux.c'
endif

if target_machine.system() == 'darwin'
    sources += 'src/serial_osx.c'
endif

if target_machine.system() == 'freebsd'
    sources += 'src/serial_bsd.c'
endif

incdir = include_directories('include')

libserial = static_library('libserial', sources,
        include_directories : incdir,
    )

test('Modem', executable('01_modem', 'test/01_modem.c', link_with: [libserial], include_directories : incdir, ))
