.thumb_func
.global loader
.type loader, "function"
.p2align 4

loader:
  msr msp, r0
  bx r1
