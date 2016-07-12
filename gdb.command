target remote localhost:1234
set architecture i386
b *0x1000203
c
x/12i 0x100045
x/24x 0x102018
x/24x 0x103000
