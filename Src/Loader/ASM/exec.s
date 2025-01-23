.thumb_func
.global exec_app
.type loader, "function"
.p2align 4

exec_app:
  msr msp, r0
  bx r1
