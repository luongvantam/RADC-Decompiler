@section.main at 0xd730 backup 0xe9e0
@offset 0x00000000

lbl ADDR_D730
   DI,RT
   call 086C4

lbl ADDR_D738
   hex 00 d4
   hex 39 d1
   r0 = [er0]
   call 1D806
   [er2] = r0,r2 = 0
   call 2242C
   er2 = er0,er0 += er4,rt
   er0 = er8
   [er0] = r2
   pop er0

lbl ADDR_D75C
   eval(adr(ADDR_D9F0) + dist.main)
   er0 = [er0],pop xr8,rt
   eval(adr(ADDR_D79E) + dist.main)
   hex 00 00
   call 0835A
   render.ddd4
   setlr_pc
   er2 = 1,r0 = r2,rt
   [er8] += er2,pop xr8
   eval(adr(ADDR_D75C) + dist.main)
   hex 00 00
   call 0902E
   hex 02 00
   [er8] += er2,pop xr8
   eval(adr(ADDR_D738) + dist.main)
   hex 00 00
   er2 = 1,r0 = r2,rt
   [er8] += er2,pop xr8
   hex 00 00
   hex 00 00
   er14 = eval(adr(ADDR_D824) - 0xC)

lbl ADDR_D79E
   call 2110D

lbl ADDR_D7A2
   hex 00 d4
   hex da 61

lbl ADDR_D7A6
   hex aa d4
   hex 44 07
   hex 00 00
   hex 40 07
   qr0 = hex 0d 00, eval(adr(ADDR_D79E) + dist.main), adr(ADDR_D7EA), hex 01 00
   [er2] = r0,r2 = 0
   call 0902E
   hex f5 d0
   call 1651A
   call 0902E
   eval(adr(ADDR_D738) + dist.main)
   call 1651A
   er0 += er4,rt
   getscancode
   setlr_pc
   setsfr
   pop ea,pop xr4
   adr(ADDR_DA10)
   hex 00 00

lbl ADDR_D7EA
   hex 11 45
   er0 = er6,er2 = er12
   ea_switchcase
   xr0 = eval(adr(ADDR_D75C) + dist.main), eval(adr(ADDR_D9F0) + dist.main)
   [er0] = er2,rt
   call 1A55A
   hex 38 1d

lbl ADDR_D806
   hex 02 00
   xr0 = adr(ADDR_D832), adr(ADDR_D95E)
   [er0] = er2,rt
   xr0 = hex 00 d4, hex 40 00
   hex 26 90

lbl ADDR_D81E
   hex 00 00
   hex ec 7e

lbl ADDR_D822
   hex 00 00

lbl ADDR_D824
   setlr_pc
   call 13236
   hex 10 03
   eval(adr(ADDR_D730) + dist.main)
   adr(ADDR_D730)

lbl ADDR_D832
   eval(adr(ADDR_D730) - 0xC)
   er0 = er8
   call 0CD8A

lbl ADDR_D83C
   adr(ADDR_D85C)
   er14 = sp,rt
   call 10744
   call 25366
   hex 00 00
   adr(ADDR_D900)

lbl ADDR_D84E
   pop ea,pop xr4
   eval(adr(ADDR_D7A2) + dist.main)

lbl ADDR_D854
   hex 02 00
   hex 00 fb
   pop xr0

lbl ADDR_D85C
   hex 11 45
   eval(adr(ADDR_D7A6) + dist.main)
   er8 = [er0],rt
   er0 += er4,rt
   r0 = [er0]
   call 14F02
   call 16CE6
   er4 += er0,r8 = r8,rt
   r0 = [er0]
   call 1651A
   er0 = er4,pop er4
   hex 00 00
   er0 += er8,rt

lbl ADDR_D88A
   er4 += er0,r8 = r8,rt
   r0 = [er0]
   ea_switchcase
   call 19BF0
   xr0 = adr(ADDR_D8B6), hex 00 00
   er2 += er8,rt
   [er0] = er2,rt
   er0 = er10,pop xr8
   adr(ADDR_D920)
   hex 00 00
   er10 = er0,rt

lbl ADDR_D8B6
   hex ff ff
   hex 01 00
   pop er0

lbl ADDR_D8BE
   hex 11 00
   call 08D56
   r0 = [er0]
   er8 = er0
   er0 = er4,pop er4
   hex 00 00
   er2 = er0,er0 += er4,rt
   r0 = [er0]
   er0 += er8,rt
   call 1651A
   er0 = er10,pop xr8
   adr(ADDR_D920)
   hex 00 00
   er2 = er0,er0 += er4,rt
   call 08F18
   call 1651A
   xr0 = adr(ADDR_D964), adr(ADDR_D968)
   hex 16 39

lbl ADDR_D900
   hex 01 00
   er2 = 1,r0 = r2,rt
   er14 = hex 42 d8
   [er8] += er2,pop xr8
   adr(ADDR_D854)
   hex 00 00
   er2 = 1,r0 = r2,rt
   [er8] += er2,pop xr8

lbl ADDR_D91C
   adr(ADDR_D968)
   adr(ADDR_D994)

lbl ADDR_D920
   call 21110

lbl ADDR_D924
   adr(ADDR_D95E)
   hex 86 9a
   eval(adr(ADDR_D940) + dist.main)
   [er8] += er2,pop xr8
   adr(ADDR_D8BE)
   hex 36 1d
   call 1DA92
   hex 4a d8
   hex 20 00
   [er8] += er2,pop xr8
   hex 00 00

lbl ADDR_D940
   adr(ADDR_D924)
   er14 = er0,pop xr0
   hex 02 00
   adr(ADDR_D854)
   call 1651A
   qr0 = hex 10 00, adr(ADDR_D920), hex 00 00, hex 01 00
   call 1651A

lbl ADDR_D95E
   call 08B13

lbl ADDR_D962
   adr(ADDR_D81E)

lbl ADDR_D964
   adr(ADDR_D81E)
   hex 40 07

lbl ADDR_D968
   hex 01 00
   qr0 = hex 0c f0, hex 02 00, eval(adr(ADDR_D91C) + dist.main), hex 00 e4
   r0 = [er0]
   call 1D806
   call 14F02
   r0 = [er0]
   call 24A20
   er0 *= r2,er2 = er0,er0 += er4,rt
   er8 = er0
   hex 36 1d

lbl ADDR_D994
   hex 02 00
   er0 = hex 0c f0
   r0 = [er0]
   adr(ADDR_D88A)
   hex 00 00
   er0 += 1,rt
   er2 = er0,er0 += er4,rt
   call 0E296
   adr(ADDR_D962)
   hex 00 00
   sp = [er8],pop er8

lbl ADDR_D9B8
   hex fc ff
   hex 04 05
   hex 06 07
   hex 08 09
   hex 0a 0b
   hex 0c 0d
   hex 0e 0f

lbl ADDR_D9C6
   hex 04 00
   hex 08 09
   hex 0a 0b
   hex 04 05
   hex 06 07
   hex 00 01
   hex 02 03

lbl ADDR_D9D4
   hex 01 00
   hex 02 06
   hex 0a 0e
   hex 01 05
   hex 09 0d
   hex 00 04
   hex 08 0c

lbl ADDR_D9E2
   hex ff ff
   hex 01 05
   hex 09 0d
   hex 02 06
   hex 0a 0e
   hex 03 07
   hex 0b 0f

lbl ADDR_D9F0
   hex 40 02
   hex 50 02
   hex 60 02
   hex 70 02
   hex 40 12
   hex 50 12
   hex 60 12
   hex 70 12
   hex 40 22
   hex 50 22
   hex 60 22
   hex 70 22
   hex 40 32
   hex 50 32
   hex 60 32
   hex 70 32

lbl ADDR_DA10
   hex 80 04
   adr(ADDR_D9B8)
   adr(ADDR_D83C)
   hex 40 08
   adr(ADDR_D9C6)
   adr(ADDR_D83C)
   hex 80 08
   adr(ADDR_D9D4)
   adr(ADDR_D83C)
   hex 40 04
   adr(ADDR_D9E2)
   adr(ADDR_D83C)
   hex 04 10
   hex 00 00
   adr(ADDR_D806)
   adr(ADDR_D822)

