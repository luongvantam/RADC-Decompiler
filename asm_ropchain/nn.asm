@section.main at 0xd730 backup 0xe9e0
@offset 0x00000000

lbl ADDR_D730
   setlr_pc
   xr0 = eval(adr(ADDR_D81A) + dist.main), hex f5 d0
   [er2] = r0,r2 = 0
   [er0] = r2
   xr0 = hex 24 d3, hex 90 dc
   BL memcpy,pop er0
   hex 46 00
   call 23F84
   eval(adr(ADDR_D75E) + dist.main)
   hex 00 00
   hex 00 08
   adr(ADDR_D96C)

lbl ADDR_D75E
   hex 44 01

lbl ADDR_D760
   eval(adr(ADDR_DA7E) + dist.main)
   eval(adr(ADDR_D78C) + dist.main)
   hex 00 00
   line_print
   er0 = er6,er2 = er12
   [er8] += er2,pop xr8
   eval(adr(ADDR_D760) + dist.main)
   hex 00 00
   er2 = hex 09 00
   [er8] += er2,pop xr8
   adr(ADDR_D7D0)
   hex 00 00
   setlr_pc
   [er4] += 1,rt

lbl ADDR_D78C
   call 0981A
   adr(ADDR_D9FE)
   adr(ADDR_D9FE)
   xr0 = eval(adr(ADDR_D78C) + dist.main), hex 1a 98
   [er0] = er2,rt
   xr0 = eval(adr(ADDR_D75E) + dist.main), hex 44 01
   [er0] = er2,rt
   xr0 = eval(adr(ADDR_D760) + dist.main), eval(adr(ADDR_DA7E) + dist.main)
   [er0] = er2,rt
   render.ddd4
   er0 = er8
   getscancode
   setlr_pc
   call 17CA6
   pop er0

lbl ADDR_D7D0
   hex 00 00
   ea_switchcase
   er6 = [ea+]
   er0 = er8
   sp = er6,pop er8

lbl ADDR_D7E2
   eval(adr(ADDR_DA06) + dist.main)
   er2 = er0,er0 += er4,rt
   [er8] += er2,pop xr8
   hex 00 00
   hex 00 00
   er14 = eval(adr(ADDR_D810) - 0x2)
   hex 60 0d

lbl ADDR_D7F8
   hex 02 00
   er2 = er0,er0 += er4,rt
   er0 = eval(adr(ADDR_DA06) + dist.main)
   er0 = [er0],pop xr8,rt
   hex 00 00
   hex 00 00
   [er0] = r2

lbl ADDR_D810
   er14 = eval(adr(ADDR_D970) - 0x2)
   sp = er14,pop er14

lbl ADDR_D81A
   hex 00 00

lbl ADDR_D81C
   setlr_pc
   buffer_clear
   qr0 = hex 3d 1b, eval(adr(ADDR_D9FA) + dist.main), hex 00 00, hex 00 00
   line_print
   render.ddd4
   setlr_pc
   xr0 = eval(adr(ADDR_DA7E) + dist.main), hex cc 00
   er0 += er8,rt
   r0 = [er0]
   r1 = 0,rt
   er0 - er2_eq,r0 = 1|r0 = 0,rt
   er2 = adr(ADDR_D9C1)
   hex_to_dec
   r2 = r0,pop er0
   eval(adr(ADDR_DA34) + dist.main)
   er0 += er8,rt
   r0 = [er0]
   er0 *= r2,er2 = er0,er0 += er4,rt
   er2 = adr(ADDR_D9C8)
   hex_to_dec
   xr0 = adr(ADDR_D9AC), hex 56 d3
   calc_func
   xr0 = adr(ADDR_D9A6), hex 2e d3
   calc_func
   xr0 = adr(ADDR_D9A8), hex 60 d3
   calc_func
   xr0 = adr(ADDR_D9AA), hex 38 d3
   calc_func
   xr0 = adr(ADDR_D9A2), hex 42 d3
   calc_func
   xr0 = adr(ADDR_D9A4), hex 4c d3
   calc_func
   setlr_pc
   qr0 = eval(adr(ADDR_D81A) + dist.main), hex 48 00, adr(ADDR_D8F4), hex 00 00
   r0 = [er0]
   r1 = 0,rt
   er0 - er2_eq,r0 = 1|r0 = 0,rt
   er2 = hex 0e 00
   er0 *= r2,er2 = er0,er0 += er4,rt
   er14 = er0,pop xr0
   hex 42 d3
   hex 4c d3
   sp = er14,pop er14

lbl ADDR_D8F4
   adr(ADDR_D97A)
   er4 = eval(adr(ADDR_D81A) + dist.main)
   [er4] += 1,rt
   sp = er14,pop er14

lbl ADDR_D904
   verify_gt
   setlr_pc
   buffer_clear
   er0 = er2,rt
   er2 = adr(ADDR_D92A)
   er0+=er0,er2+=er0,er0=[er2]
   er14 = er0,pop xr0
   hex 11 11
   adr(ADDR_D9E3)
   sp = er14,pop er14

lbl ADDR_D92A
   adr(ADDR_D936)

lbl ADDR_D92C
   adr(ADDR_D92C)
   er4 = adr(ADDR_D938)
   hex 2a 33

lbl ADDR_D936
   hex 01 00

lbl ADDR_D938
   call 23EC1
   xr0 = hex 01 01, eval(adr(ADDR_D9CC) + dist.main)
   call 23EC2
   xr0 = hex 09 09, eval(adr(ADDR_D9DB) + dist.main)
   call 23EC2
   xr0 = hex 39 39, eval(adr(ADDR_D9E9) + dist.main)
   call 23EC2
   render.ddd4
   waitshift
   setlr_pc

lbl ADDR_D96C
   buffer_clear

lbl ADDR_D970
   xr0 = adr(ADDR_D996), hex 2e d7
   hex d8 39

lbl ADDR_D97A
   hex 01 00
   DI,RT
   xr0 = adr(ADDR_D99A), hex 01 00
   [er0] = er2,rt
   pop qr0
   hex 9a 03
   eval(adr(ADDR_D730) + dist.main)
   adr(ADDR_D730)

lbl ADDR_D996
   adr(ADDR_D81A)
   hex 32 89

lbl ADDR_D99A
   hex 2c 01
   hex 00 00
   sp = er6,pop er8

lbl ADDR_D9A2
   adr(ADDR_D9AE)

lbl ADDR_D9A4
   adr(ADDR_D9B8)

lbl ADDR_D9A6
   adr(ADDR_D9BD)

lbl ADDR_D9A8
   adr(ADDR_D9C1)

lbl ADDR_D9AA
   adr(ADDR_D9C4)

lbl ADDR_D9AC
   adr(ADDR_D9C8)

lbl ADDR_D9AE
   hex 42 c8
   hex 32 a7
   hex 31 30
   hex 30 a8
   hex 43 00

lbl ADDR_D9B8
   hex 33 33
   hex 33 33
   hex 00

lbl ADDR_D9BD
   hex 46
   hex a6 42
   hex 00

lbl ADDR_D9C1
   hex 00
   hex 00 00

lbl ADDR_D9C4
   hex 47 a6
   hex 43 00

lbl ADDR_D9C8
   hex 00 00
   hex 00 00

lbl ADDR_D9CC
   "NEURAL NETWORK"
   hex 00

lbl ADDR_D9DB
   "this is"
   hex 00

lbl ADDR_D9E3
   hex 31
   hex 00 00
   hex 00 30
   hex 00

lbl ADDR_D9E9
   "cre:@luongvantam"
   hex 00

lbl ADDR_D9FA
   hex 6c 6f
   hex 61 64

lbl ADDR_D9FE
   hex 69 6e
   hex 67 2e
   hex 2e 2e
   hex 20 00

lbl ADDR_DA06
   eval(adr(ADDR_DA7E) + dist.main)
   hex 80 04
   adr(ADDR_D7E2)
   hex f7 ff
   hex 40 08
   adr(ADDR_D7E2)
   hex 09 00
   hex 40 04
   adr(ADDR_D7E2)
   hex ff ff
   hex 80 08
   adr(ADDR_D7E2)
   hex 01 00
   hex 01 01
   adr(ADDR_D7F8)
   hex cc 00
   hex 10 40
   adr(ADDR_D7F8)
   hex cd 00
   hex 80 01
   adr(ADDR_D97A)
   hex 00 00
   hex 0e d8

lbl ADDR_DA34
   hex 64 64
   hex 5e 40
   hex 5c 84
   hex 75 64
   hex 00 64
   hex 62 4c
   hex 54 63
   hex 51 70
   hex 64 00
   hex 64 69
   hex 4c 82
   hex bb 48
   hex 5a 64
   hex 00 64
   hex 53 58
   hex 8f c8
   hex 46 30
   hex 64 00
   hex 64 37
   hex 43 96
   hex c6 44
   hex 38 64
   hex 00 64
   hex 51 36
   hex 80 99
   hex 36 42
   hex 64 00
   hex 64 62
   hex 2f 4b
   hex 67 3f
   hex 63 69
   hex 00 64
   hex 64 65
   hex 40 68
   hex 77 7c
   hex 6d 00
   hex 00 00

lbl ADDR_DA7E
   hex cd cd
   hex cd cd
   hex cd cd
   hex cd cd
   hex 00 cd
   hex cd cd
   hex cd cd
   hex cd cd
   hex cd 00
   hex cd cd
   hex cd cd
   hex cd cd
   hex cd cd
   hex 00 cd
   hex cd cd
   hex cd cd
   hex cd cd
   hex cd 00
   hex cd cd
   hex cd cd
   hex cd cd
   hex cd cd
   hex 00 cd
   hex cd cd
   hex cd cd
   hex cd cd
   hex cd 00
   hex cd cd
   hex cd cd
   hex cd cd
   hex cd cd
   hex 00 cd
   hex cd cd
   hex cd cd
   hex cd cd
   hex cd 00
   hex 00 00
   hex 00 00

