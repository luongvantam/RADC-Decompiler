@section.main at 0xd730 backup 0xe9e0
@offset 0x30300000

lbl ADDR_D730
   setlr
   setsfr
   xr0 = hex 11 d1, hex c1 00
   [er0] = r2
   xr0 = hex 13 d1, hex 01 00
   [er0] = r2
   xr0 = hex 37 d1, hex 0a 00
   [er0] = r2
   xr0 = hex 19 01, adr(ADDR_D896)
   BL line_print
   xr0 = hex 47 21, hex 2d 00
   char_print_1byte
   waitshift_blank
   er0 = hex 01 01
   delay
   buffer_clear
   xr0 = hex 2e d3, hex 01 01
   num_frombyte
   xr0 = adr(ADDR_D866), hex 38 d3
   calc_func
   xr0 = adr(ADDR_D866), adr(ADDR_D86E)
   [er0] = er2,rt
   xr0 = adr(ADDR_D866), hex 42 d3
   calc_func
   xr0 = adr(ADDR_D866), adr(ADDR_D86E)
   [er0] = er2,rt
   xr0 = hex 38 d3, adr(ADDR_D87D)
   num_to_str
   er0 = hex 00 00
   xr0 = hex 42 d3, adr(ADDR_D880)
   num_to_str
   er0 = hex 00 00
   xr0 = hex 11 d1, hex 89 00
   [er0] = r2
   xr0 = adr(ADDR_D86C), hex 6c d1
   verify_func
   er0 = hex 6c d1
   num_to_byte
   setlr
   r1 = 0,rt
   er2 = hex 01 00
   er0 - er2_eq,r0 = 1|r0 = 0,rt
   er2 = adr(ADDR_D868)
   er0+=er0,er2+=er0,er0=[er2]
   er2 = er0,er0 += er4,rt
   er0 = hex 24 11
   BL line_print
   render.ddd4
   xr0 = hex 30 20, adr(ADDR_D87D)
   BL line_print
   xr0 = hex 60 20, adr(ADDR_D880)
   BL line_print
   render.ddd4
   qr0 = hex 30 d6, hex 84 d1, hex 30 30, hex 2e d6
   BL strcpy
   sp = er6,pop er8

lbl ADDR_D866
   adr(ADDR_D86E)

lbl ADDR_D868
   adr(ADDR_D886)

lbl ADDR_D86A
   adr(ADDR_D888)

lbl ADDR_D86C
   adr(ADDR_D873)

lbl ADDR_D86E
   hex 87 30
   hex 2c 37
   hex 00

lbl ADDR_D873
   hex 43
   hex a5 44
   hex 00 00
   hex 00 00
   hex 00 00
   hex 00

lbl ADDR_D87D
   hex 30
   hex 30 00

lbl ADDR_D880
   hex 30 30
   hex 00 30
   hex 30 00

lbl ADDR_D886
   hex 20 00

lbl ADDR_D888
   hex 70 65
   hex 6e 2d
   hex 73 68
   hex 6f 6f
   hex 74 20
   hex 6f 75
   hex 74 00

lbl ADDR_D896
   hex 41 49
   hex 20 73
   hex 63 6f
   hex 72 65
   hex 20 70
   hex 72 65
   hex 64 69
   hex 63 74
   hex 6f 72
   hex 00

