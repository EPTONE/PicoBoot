.thumb_func
.global exec
.type loader, "function"
.p2align 4

exec:
  msr msp, r0
  bx r1
