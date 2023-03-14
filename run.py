from sys import argv
from os import listdir as ld, system

CMD = 'g++%s -Wall -Wextra -pedantic-errors ' \
	'-lSDL2 -lglew32 -lopengl32 -L.%s -o prog && prog.exe' % (
		''.join(' ' + x for x in ld()
			if x.split('.')[-1] in ('cc', 'cpp')),
		''.join(' ' + x for x in argv[1:])
	)

print(CMD)

print(f'finished ({system(CMD)})')

system('del prog.exe')
