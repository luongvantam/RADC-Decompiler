@section.main at 0xd730 backup 0xe9e0
@offset 0x30300000

lbl ADDR_D730
   xr0 = hex 21 21, eval(adr(ADDR_D746) + dist.main)
   line_print
   render.ddd4
   hex 30 30
   hex 33 30
   hex 00 00

lbl ADDR_D746
   "ABCD"
   hex 00 00
   hex 00 00
   hex 00 00

