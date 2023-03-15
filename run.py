import sys
from os import listdir as ld, system

lib = '-lglew32 -lopengl32 -L.'
exc = 'a.exe'

# systems that use elf
if sys.platform in ('linux'):
	lib = '-lGLEW -lGL'
	exc = './a.out'

CMD = 'g++{} -Wall -Wextra -pedantic-errors{} -lSDL2 {}' \
	' && {}'.format(
		''.join(' ' + x for x in ld() if x.split('.')[-1] in ('cpp', 'cc')),
		''.join(' ' + x for x in sys.argv[1:]),
		lib,
		exc
	);

print(CMD)

try:
	print(f'finished ({system(CMD)})')
except:
	print('%s: aborted!' % sys.argv[0])
