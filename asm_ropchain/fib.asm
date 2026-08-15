@section.main at 0xd730 backup 0xe9e0
@offset 0x00000000

lbl ADDR_D730
   setlr
   setsfr
   buffer_clear
   xr0 = adr(ADDR_D7DE), hex 42 d3
   calc_func
   xr0 = adr(ADDR_D7DA), hex 2e d3
   calc_func
   xr0 = adr(ADDR_D7DC), hex 38 d3
   calc_func
   xr0 = adr(ADDR_D7E0), hex 6a d3
   calc_func
   setlr
   er0 = hex 6a d3
   r0 = [er0]
   r1 = 0,rt
   er2 = hex 00 00
   er0 - er2_eq,r0 = 1|r0 = 0,rt
   er2 = adr(ADDR_D7EE)
   er0+=er0,er2+=er0,er0=[er2]
   er14 = er0,pop xr0
   hex 00 00
   hex 00 00
   hex 60 0d

lbl ADDR_D79C
   hex 02 00
   xr0 = hex 2e d3, hex 00 d4
   num_to_str
   xr0 = hex 01 01, hex 00 d4
   line_print
   render.ddd4
   hex 30 30

lbl ADDR_D7BC
   hex 03 00
   DI,RT
   xr0 = hex 30 d6, hex 84 d1
   BL strcpy
   er14 = hex 2e d6
   sp = er14,pop er14
   hex 00 00

lbl ADDR_D7DA
   adr(ADDR_D7E2)

lbl ADDR_D7DC
   adr(ADDR_D7E4)

lbl ADDR_D7DE
   adr(ADDR_D7E6)

lbl ADDR_D7E0
   adr(ADDR_D7EA)

lbl ADDR_D7E2
   hex 43 00

lbl ADDR_D7E4
   hex 44 00

lbl ADDR_D7E6
   hex 42 a6
   hex 43 00

lbl ADDR_D7EA
   hex 48 a7
   hex 31 00

lbl ADDR_D7EE
   adr(ADDR_D7BC)
   adr(ADDR_D79C)
   hex 00 00
   hex 00 00

