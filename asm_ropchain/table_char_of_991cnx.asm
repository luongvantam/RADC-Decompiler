@section.main at 0xd71e backup 0xe9e0
@offset 0x00000000

lbl ADDR_D71E
   hex 00 00
   eval(adr(ADDR_D71E) + dist.main)
   adr(ADDR_D71E)
   eval(adr(ADDR_D732) - 0xC)

lbl ADDR_D726
   hex 00 00
   hex 00 04
   hex 00 00
   hex 00 00
   call 0CD8E

lbl ADDR_D732
   setlr_pc
   setsfr
   buffer_clear
   xr0 = hex 01 01, adr(ADDR_D8EE)
   line_print
   pop xr0
   hex 0c 11

lbl ADDR_D750
   adr(ADDR_D900)

lbl ADDR_D751
   line_print
   pop xr0
   hex 0c 21

lbl ADDR_D75C
   adr(ADDR_D910)

lbl ADDR_D75D
   line_print
   pop xr0
   hex 0c 31

lbl ADDR_D768
   adr(ADDR_D920)

lbl ADDR_D769
   line_print
   pop xr0
   hex 01 11

lbl ADDR_D774
   adr(ADDR_DA00)

lbl ADDR_D775
   line_print
   pop xr0
   hex 01 21

lbl ADDR_D780
   eval(adr(ADDR_DA04) - 0x2)

lbl ADDR_D781
   line_print
   pop xr0
   hex 01 31

lbl ADDR_D78C
   adr(ADDR_DA04)

lbl ADDR_D78D
   line_print
   render.ddd4
   call 121A8
   adr(ADDR_D7B4)
   getscancode
   setlr_pc
   call 1D52C
   adr(ADDR_DAF6)
   adr(ADDR_DAF6)
   call 17B40
   call 121A8

lbl ADDR_D7B4
   hex 00 00
   ea_switchcase
   er6 = [ea+]
   sp = er6,pop er8

lbl ADDR_D7C2
   eval(adr(ADDR_D750) + dist.main)
   call 18814
   hex f0 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D75C) + dist.main)
   hex 00 00
   call 18814

lbl ADDR_D7D6
   hex f0 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D768) + dist.main)
   hex 00 00
   call 18814
   hex f0 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D774) + dist.main)
   hex 00 00
   call 18814
   hex fe 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D780) + dist.main)
   hex 00 00
   call 18814
   hex fe 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D78C) + dist.main)
   hex 00 00
   call 18814
   hex fe 00
   [er8] += er2,pop xr8
   hex 00 00
   hex 00 00
   er6 = eval(adr(ADDR_D884) - 0x2)
   sp = er6,pop er8

lbl ADDR_D822
   eval(adr(ADDR_D750) + dist.main)
   call 18814
   hex 10 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D75C) + dist.main)
   hex 00 00
   call 18814
   hex 10 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D768) + dist.main)
   hex 00 00
   call 18814
   hex 10 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D774) + dist.main)
   hex 00 00
   call 18814
   hex 02 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D780) + dist.main)
   hex 00 00
   call 18814
   hex 02 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D78C) + dist.main)
   hex 00 00
   call 18814
   hex 02 00
   [er8] += er2,pop xr8
   hex 00 00
   hex 00 00
   er6 = eval(adr(ADDR_D884) - 0x2)
   sp = er6,pop er8
   hex 00 00

lbl ADDR_D884
   xr0 = eval(adr(ADDR_D751) + dist.main), hex d9 00
   [er0] = r2
   xr0 = eval(adr(ADDR_D75D) + dist.main), hex d9 00
   [er0] = r2
   xr0 = eval(adr(ADDR_D769) + dist.main), hex d9 00
   [er0] = r2
   xr0 = eval(adr(ADDR_D775) + dist.main), hex da 00
   [er0] = r2
   xr0 = eval(adr(ADDR_D781) + dist.main), hex da 00
   [er0] = r2
   xr0 = eval(adr(ADDR_D78D) + dist.main), hex da 00
   [er0] = r2
   xr0 = hex f5 d0, hex 01 00
   [er0] = r2
   call 18814
   hex 00 04
   call 13236
   hex 00 00
   eval(adr(ADDR_D71E) + dist.main)
   adr(ADDR_D71E)
   adr(ADDR_D726)
   call 0CD8E

lbl ADDR_D8EE
   hex 20 30
   hex 31 32
   hex 33 34
   hex 35 36
   hex 37 38
   hex 39 41
   hex 42 43
   hex 44 45
   hex 46 00

lbl ADDR_D900
   hex 20 20
   hex 02 03
   hex 20 05
   hex 06 07
   hex 08 09
   hex 0a 0b
   hex 0c 0d
   hex 0e 0f

lbl ADDR_D910
   hex 10 11
   hex 12 13
   hex 14 15
   hex 16 17
   hex 18 19
   hex 1a 1b
   hex 1c 1d
   hex 1e 1f

lbl ADDR_D920
   hex 20 21
   hex 22 23
   hex 24 25
   hex 26 27

lbl ADDR_D928
   hex 28 29
   hex 2a 2b
   hex 2c 2d
   hex 2e 2f
   hex 30 31
   hex 32 33
   hex 34 35
   hex 36 37
   hex 38 39
   hex 3a 3b
   hex 3c 3d
   hex 3e 3f
   hex 40 41
   hex 42 43
   hex 44 45
   hex 46 47
   hex 48 49
   hex 4a 4b
   hex 4c 4d
   hex 4e 4f
   hex 50 51
   hex 52 53
   hex 54 55
   hex 56 57
   hex 58 59
   hex 5a 5b
   hex 5c 5d
   hex 5e 5f
   hex 60 61
   hex 62 63
   hex 64 65
   hex 66 67
   hex 68 69
   hex 6a 6b
   hex 6c 6d
   hex 6e 6f
   hex 70 71
   hex 72 73
   hex 74 75
   hex 76 77
   hex 78 79
   hex 7a 7b
   hex 7c 7d
   hex 7e 7f
   hex 80 81
   hex 82 83
   hex 84 85
   hex 86 87
   hex 88 89
   hex 8a 8b
   hex 8c 8d
   hex 8e 8f
   hex 90 91
   hex 92 93
   hex 94 95
   hex 96 97
   hex 98 99
   hex 9a 9b
   hex 9c 9d
   hex 9e 9f
   hex a0 a1
   hex a2 a3
   hex a4 a5
   hex a6 a7
   hex a8 a9
   hex aa ab
   hex ac ad
   hex ae af
   hex b0 b1
   hex b2 b3
   hex b4 b5
   hex b6 b7
   hex b8 b9
   hex ba bb
   hex bc bd
   hex be bf
   hex c0 c1
   hex c2 c3
   hex c4 c5
   hex c6 c7
   hex c8 c9
   hex ca cb
   hex cc cd
   hex ce cf
   hex d0 d1
   hex d2 d3
   hex d4 d5
   adr(ADDR_D7D6)

lbl ADDR_D9D8
   adr(ADDR_D9D8)
   hex da db
   hex dc dd
   hex de df
   hex e0 e1
   hex e2 e3
   hex e4 e5
   hex e6 e7
   eval(adr(ADDR_D726) + dist.main)
   eval(adr(ADDR_D928) + dist.main)
   hex ec ed
   hex ee ef
   hex 20 20
   hex 20 20
   hex 20 20
   hex 20 20
   hex 20 20
   hex 20 20
   hex 20 20
   hex 20 20

lbl ADDR_DA00
   hex 30 00
   hex 31 00

lbl ADDR_DA04
   hex 32 00
   hex 33 00
   hex 34 00
   hex 35 00
   hex 36 00
   hex 37 00
   hex 38 00
   hex 39 00
   hex 41 00
   hex 42 00
   hex 43 00
   hex 44 00
   hex 45 00
   hex 46 00
   hex 30 00
   hex 31 00
   hex 32 00
   hex 33 00
   hex 34 00
   hex 35 00
   hex 36 00
   hex 37 00
   hex 38 00
   hex 39 00
   hex 41 00
   hex 42 00
   hex 43 00
   hex 44 00
   hex 45 00
   hex 46 00
   hex 30 00
   hex 31 00
   hex 32 00
   hex 33 00
   hex 34 00
   hex 35 00
   hex 36 00
   hex 37 00
   hex 38 00
   hex 39 00
   hex 41 00
   hex 42 00
   hex 43 00
   hex 44 00
   hex 45 00
   hex 46 00
   hex 30 00
   hex 31 00
   hex 32 00
   hex 33 00
   hex 34 00
   hex 35 00
   hex 36 00
   hex 37 00
   hex 38 00
   hex 39 00
   hex 41 00
   hex 42 00
   hex 43 00
   hex 44 00
   hex 45 00
   hex 46 00
   hex 30 00
   hex 31 00
   hex 32 00
   hex 33 00
   hex 34 00
   hex 35 00
   hex 36 00
   hex 37 00
   hex 38 00
   hex 39 00
   hex 41 00
   hex 42 00
   hex 43 00
   hex 44 00
   hex 45 00
   hex 46 00
   hex 30 00
   hex 31 00
   hex 32 00
   hex 33 00
   hex 34 00
   hex 35 00
   hex 36 00
   hex 37 00
   hex 38 00
   hex 39 00
   hex 41 00
   hex 42 00
   hex 43 00
   hex 44 00
   hex 45 00
   hex 46 00
   hex 30 00
   hex 31 00
   hex 32 00
   hex 33 00
   hex 34 00
   hex 35 00
   hex 36 00
   hex 37 00
   hex 38 00
   hex 39 00
   hex 41 00
   hex 42 00
   hex 43 00
   hex 44 00
   hex 45 00
   hex 46 00
   hex 30 00
   hex 31 00
   hex 32 00
   hex 33 00
   hex 34 00
   hex 35 00
   hex 36 00
   hex 37 00
   hex 38 00
   hex 39 00
   hex 41 00

lbl ADDR_DAF6
   hex 42 00
   hex 43 00
   hex 44 00
   hex 45 00
   hex 46 00
   hex 80 04
   adr(ADDR_D7C2)
   hex 40 08
   adr(ADDR_D822)
   hex 00 00
   hex 82 d8
   hex ff ff

