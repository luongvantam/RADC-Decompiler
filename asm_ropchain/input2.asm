@section.main at 0xd730 backup 0xe9e0
@offset 0x00000000

lbl ADDR_D730
   setlr_pc
   qr0 = hex 00 d5, adr(ADDR_D9E6), adr(ADDR_D758), adr(ADDR_D77E)
   r0 = [er0]
   r1 = 0,rt
   call 15C78
   eval(adr(ADDR_D8A0) - 0x2)
   er0+=er0,er2+=er0,er0=[er2]
   [er4] = er0,pop er0,rt

lbl ADDR_D756
   hex ff ff

lbl ADDR_D758
   hex 3e 14
   hex 32 30

lbl ADDR_D75C
   adr(ADDR_D9F6)
   eval(adr(ADDR_DA3A) - 0x2)
   sp = er14,pop er14
   pop ea
   hex 00 d4
   er0 = hex 0f 00
   [ea+]=r0,r0-=1,bne

lbl ADDR_D774
   call 09E68
   hex 00 d5
   hex 01 30
   hex d2 03

lbl ADDR_D77E
   hex 02 00
   pop r0

lbl ADDR_D784
   hex 10 67
   call 2B9B8
   hex 4c d0
   r1 = 0,rt
   er2 = adr(ADDR_D9E0)
   er0+=er0,er2+=er0,er0=[er2]
   er14 = er0,pop xr0
   adr(ADDR_D75C)
   hex 02 d5
   hex 60 0d

lbl ADDR_D7A4
   hex 02 00
   calc_func
   call 09E68
   hex 02 d5
   adr(ADDR_D8EE)
   r0 = [er0]
   [er2] = r0,r0 = 0
   setlr_pc
   hex da 7b

lbl ADDR_D7C0
   hex 01 00
   adr(ADDR_DA44)
   adr(ADDR_D774)
   eval(adr(ADDR_D784) + dist.main)
   eval(adr(ADDR_D8E6) - 0x2)
   [er0] = er2,rt
   [er4] -= 1,rt
   DI,RT
   hex 74 1f

lbl ADDR_D7D8
   hex 02 00
   buffer_clear
   call 09E68
   hex 39 02
   hex 39 3e
   line_draw
   call 09E68
   hex 53 02
   hex 53 3e
   line_draw
   call 09E68
   hex 80 02
   hex 80 3e
   line_draw
   call 09E68
   hex 22 12
   hex 72 12
   line_draw
   call 09E68
   hex 22 22
   hex 72 22
   line_draw

lbl ADDR_D81A
   call 09E68
   hex 22 32
   hex 72 32
   line_draw
   setlr_pc
   call 09E68

lbl ADDR_D82E
   hex 00 d4
   adr(ADDR_D9BC)
   r0 = [er0]
   r1 = 0,rt
   er0+=er0,er2+=er0,er0=[er2]
   er2 = er0,er0 += er4,rt
   pop er0

lbl ADDR_D846
   hex 20 04
   line_print
   setlr_pc
   er8 = eval(adr(ADDR_D846) + dist.main)
   er2 = hex 1a 00
   [er8] += er2,pop xr8
   hex 30 30
   hex 30 30
   er4 = eval(adr(ADDR_D82E) + dist.main)
   [er4] += 1,rt
   er4 = eval(adr(ADDR_D87E) + dist.main)
   [er4] += 1,rt
   call 15C78
   eval(adr(ADDR_D9A6) - 0x2)

lbl ADDR_D87E
   call 1073F
   hex 30 30
   hex 68 9e
   hex 00 00
   hex 20 20
   eval(adr(ADDR_D846) + dist.main)
   [er2] = r0,r0 = 0
   call 09E68
   hex 3e 07
   eval(adr(ADDR_D87E) + dist.main)
   [er0] = er2,rt
   render.ddd4

lbl ADDR_D8A0
   er0 = adr(ADDR_D756)
   getscancode
   setlr_pc
   er0 = hex 67 36
   pop ea
   adr(ADDR_D9BC)
   ea_switchcase
   qr0 = [ea]
   er2 = er0,er0 += er4,rt
   er0 = [er2],r2 = 9,rt
   pop ea
   adr(ADDR_DA02)
   ea_switchcase
   qr0 = [ea]
   er2 = er0,er0 = er2,pop er8,rt
   adr(ADDR_D8EE)
   er0 = er8
   [er0] = er2,rt

lbl ADDR_D8E6
   pop qr0
   adr(ADDR_D9F2)

lbl ADDR_D8EC
   hex 0f d4

lbl ADDR_D8EE
   hex 00 00
   hex 30 30
   [er0] = er2,rt
   er0 = er2,rt
   er8 = er0
   er0 = er4,pop er4
   hex 67 36
   pop ea
   adr(ADDR_DA00)
   ea_switchcase
   er0 = er8
   call 19E5A
   er0 += er8,rt
   pop ea
   adr(ADDR_D9F0)
   [ea+] = er0
   r1=0,pop er12
   hex 0f 00
   er2=er12
   er0 - er2_gt,r0 = 0|r0 = 1,rt
   r0 - 01H_eq,r0 = 0|r0 = 1
   er2 = adr(ADDR_D952)
   [er2] = r0,r2 = 0
   er2 = adr(ADDR_D9F0)
   er0+=er0,er2+=er0,er0=[er2]
   r0 = [er0]
   r2 = r0,pop er0

lbl ADDR_D952
   hex 67 00
   call 19E5A
   er0 = er8

lbl ADDR_D95C
   [er0] = r2
   er2 = adr(ADDR_D9F0)
   er0 = [er2],r2 = 9,rt
   r2 = 0,r7 = 4
   [er0] = r2
   er2 = adr(ADDR_D952)
   er0 = [er2],r2 = 9,rt
   er2 = adr(ADDR_D9F0)
   er0+=er0,er2+=er0,er0=[er2]
   er2 = eval(adr(ADDR_D8EC) + dist.main)
   [er2] = er0,r2 = 0,pop er4,rt
   hex 67 36
   call 21634
   hex 68 9e
   hex 00 00
   hex 00 d5
   hex 02 36
   [er0] = r2
   call 21634

lbl ADDR_D9A6
   hex 68 9e
   hex 00 00
   adr(ADDR_DA44)
   adr(ADDR_D81A)
   [er0] = er2,rt
   call 15C78
   eval(adr(ADDR_DA3A) - 0x2)
   sp = er14,pop er14

lbl ADDR_D9BC
   adr(ADDR_DA1E)
   adr(ADDR_D9FD)
   adr(ADDR_DA09)
   adr(ADDR_DA13)
   adr(ADDR_DA27)
   adr(ADDR_DA31)
   adr(ADDR_D9DC)
   adr(ADDR_D9E4)
   adr(ADDR_D9EC)
   adr(ADDR_D9F4)
   adr(ADDR_DA1C)
   adr(ADDR_D9FC)
   adr(ADDR_DA08)
   adr(ADDR_DA12)
   adr(ADDR_DA26)
   adr(ADDR_DA30)

lbl ADDR_D9DC
   hex 36 00
   hex 01 08

lbl ADDR_D9E0
   adr(ADDR_D7D8)
   adr(ADDR_D7A4)

lbl ADDR_D9E4
   hex 37 00

lbl ADDR_D9E6
   hex ac 84
   hex 74 1f
   hex 60 0d

lbl ADDR_D9EC
   hex 38 00
   hex 01 10

lbl ADDR_D9F0
   hex 36 67

lbl ADDR_D9F2
   hex 36 67

lbl ADDR_D9F4
   hex 39 00

lbl ADDR_D9F6
   hex 87 30
   hex 2c 33
   hex 00 30

lbl ADDR_D9FC
   hex 31

lbl ADDR_D9FD
   hex 31
   hex 00 00

lbl ADDR_DA00
   adr(ADDR_D756)

lbl ADDR_DA02
   hex 40 08
   hex 01 00
   hex 04 00

lbl ADDR_DA08
   hex 31

lbl ADDR_DA09
   hex 32
   hex 00 30
   call 20440
   hex ff ff

lbl ADDR_DA12
   hex 31

lbl ADDR_DA13
   hex 33
   hex 00 30
   hex 80 08
   call 10003

lbl ADDR_DA1C
   hex 31 30

lbl ADDR_DA1E
   hex 00 30
   hex 80 04
   hex 00 00
   hex fc ff

lbl ADDR_DA26
   hex 31

lbl ADDR_DA27
   hex 34
   hex 00 30
   hex 80 01
   call 20004

lbl ADDR_DA30
   hex 31

lbl ADDR_DA31
   hex 35
   hex 00 30
   hex 80 02
   hex 05 00
   hex 00 ff

lbl ADDR_DA3A
   pop xr4,pop xr12
   adr(ADDR_D730)
   hex 0a 03
   eval(adr(ADDR_D730) + dist.main)

lbl ADDR_DA44
   hex 24 d7
   memcpy_auto_jump

