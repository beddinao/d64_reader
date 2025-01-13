# *`d64_reader`*

```
usage: ./d64_reader <path to .d64 file>

creates a file for each PRG found in the disk image,
assigning it a random name, and saves it in the current directory.
```

```
 > make
cc -Iinclude -c src/d64_reader.c -o build/d64_reader.o
cc -o d64_reader   build/d64_reader.o
 > ./d64_reader Footprint-Intro.d64
PRG file created at "./BFHZUTMBM.prg"
 > xxd -g 1 BFHZUTMBM.prg|head
00000000: 01 08 0b 08 e6 07 9e 32 30 36 31 00 00 00 ba bd  .......2061.....
00000010: 22 14 9d fc 00 ca d0 f7 a0 35 4c d4 13 87 d8 31  "........5L....1
00000020: 0f f7 0c 9f e2 e4 89 81 cd 94 1d 1c c4 f0 e1 98  ................
00000030: 97 4d 00 71 0c 48 9f 08 8f ea 08 cf 94 c9 e4 dc  .M.q.H..........
00000040: 64 84 83 6a 8c a0 91 35 9f c7 f1 df 8c 93 c3 c5  d..j...5........
00000050: 03 07 8c 41 1b 0f 9a c8 40 18 94 b1 c0 c8 cc ef  ...A....@.......
00000060: 1c aa a2 f4 0d 3e 36 0f 87 6e 81 e3 3a 38 e0 1c  .....>6..n..:8..
00000070: 8d 01 b5 40 70 1a ed aa 83 cf d1 9d 03 67 0b 82  ...@p........g..
00000080: d1 c3 98 4e 43 15 4a 9b fa 60 ef 85 01 85 4d 83  ...NC.J..`....M.
00000090: 0d 33 96 f8 04 0a 21 00 42 b8 6b c1 9f d6 83 ad  .3....!.B.k.....
```
` -> $801 = the two bytes header specifying the PRG load location`
