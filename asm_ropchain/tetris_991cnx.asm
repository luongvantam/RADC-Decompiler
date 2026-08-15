@section.main at 0xd730 backup 0xe9e0

lbl ADDR_D730
   hex 00 00
   eval(adr(ADDR_D754) + dist.main)
   call 0E6E0
   call 1D0F5
   hex 00 00
   hex 00 00
   [er0] = r2
   setlr_pc
   DI,RT
   call 18814
   hex 7c 02
   hex 8e cd

lbl ADDR_D754
   hex 00 00
   hex 00 00
   hex 00 00
   call 0E6E0
   hex ea e3
   call 18814
   hex 7c 02
   call 0CD8E
   setlr_pc
   setsfr
   buffer_clear
   xr0 = hex 4e e9, hex 08 00
   memzero
   call 121A8
   hex 59 e9
   r0 = [er0]
   setlr_pc
   hex_byte
   er2 = er0,er0 += er4,rt
   er8 = hex 4e e9
   [er8] += er2,pop xr8
   hex 50 e9
   hex 00 00
   call 121A8
   hex 58 e9
   r0 = [er0]
   setlr_pc
   hex_byte
   er2 = er0,er0 += er4,rt
   [er8] += er2,pop xr8
   hex 54 e9
   hex 00 00
   call 121A8
   adr(ADDR_D8D2)
   call 2363E
   r0 = [er0]
   setlr_pc
   hex_byte
   er2 = er0,er0 += er4,rt
   [er8] += er2,pop xr8
   hex 00 00
   hex 00 00
   xr0 = hex 48 0c, hex 4e e9
   line_print
   xr0 = hex 52 28, hex 54 e9
   line_print
   render.ddd4
   call 121A8
   hex ac e4
   getscancode
   setlr_pc
   call 1D52C
   hex b6 e5
   hex b6 e5
   call 17B40
   call 121A8
   hex 00 00
   ea_switchcase
   er6 = [ea+]
   er0 = er8
   sp = er6,pop er8
   hex 58 e9
   er2 = er0,er0 += er4,rt
   [er8] += er2,pop xr8
   hex be e5
   hex 00 00
   xr0 = hex 4c e9, hex 00 00
   [er0] = r2
   sp = [er8],pop er8
   hex 00 00
   setlr_pc
   call 1827C
   hex 00 11
   er0 += er4,rt
   er10 = er0,rt
   call 121A8
   adr(ADDR_D8C6)
   call 2363E
   call 18814
   hex 00 00
   er0 - er2_gt,r0 = 0|r0 = 1,rt
   call 18814
   hex 4e 00
   call 1827C
   hex 2a e5
   er0 *= r2,er2 = er0,er0 += er4,rt
   er14 = er0,pop xr0
   hex 00 00
   hex 00 00
   er0 = er10,pop xr8
   hex 00 00
   hex 00 00
   er2 = er0,er0 += er4,rt
   call 10740
   er8 = hex 4c e9
   [er8] += er2,pop xr8
   hex 00 00
   hex 00 00
   call 121A8
   adr(ADDR_D8C6)
   call 2363E
   er2 = er0,er0 += er4,rt
   call 121A8
   adr(ADDR_D8D2)
   call 2363E

lbl ADDR_D8C6
   [er0] = r2
   xr0 = hex 4c e9, hex 02 00

lbl ADDR_D8D2
   memzero
   er8 = hex 58 e9
   call 18814
   hex 01 00
   [er8] += er2,pop xr8
   hex be e5
   hex 00 00
   sp = [er8],pop er8
   call 121A8
   hex 10 00
   call 1827C
   hex 00 e0
   er0 *= r2,er2 = er0,er0 += er4,rt
   er2 = er0,er0 += er4,rt
   er8 = hex 4c e9
   [er8] += er2,pop xr8
   hex 00 00
   hex 00 00
   pop qr0
   hex 58 e9
   sp = [er8],pop er8
   hex 00 00
   call 18814
   hex 7c 02
   call 13236
   hex 00 00
   hex e0 e6
   hex e0 e3
   hex ea e3
   call 0CD8E
   call 0E5A6
   hex be e4
   hex ff ff
   hex 80 08
   hex be e4
   hex 01 00
   hex 80 04
   call 0E4BE
   hex 40 08
   call 0E4BE
   hex 80 01
   call 0E4BE
   hex 80 02
   call 0E4BE
   hex 10 40
   call 0E4DC
   hex 01 01
   call 1E4DC
   hex 01 02
   hex dc e4
   call 1FF02
   hex dc e4
   hex 03 ff
   hex 02 01
   hex dc e4
   hex 04 ff
   hex 02 02
   hex dc e4
   hex 05 ff
   hex 02 04
   hex dc e4
   hex 06 ff
   hex 04 01
   hex dc e4
   hex 07 ff
   hex 04 02
   hex dc e4
   hex 08 ff
   hex 04 04
   hex dc e4
   call 0FF09
   hex dc e4
   call 0FF0A
   hex dc e4
   call 0FF0B
   hex dc e4
   call 0FF0C
   hex dc e4
   call 0FF0D
   hex dc e4
   call 0FF0E
   hex dc e4
   hex 0f ff
   hex 04 10
   call 0E5A0
   call 0E5A6
   hex 00 00
   hex 00 00
   hex 00 00
   hex 00 00
   hex 00 00
   call 0E9E0

