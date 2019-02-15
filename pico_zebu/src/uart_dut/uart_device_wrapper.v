//---------------------------------------------------------------------
// COPYRIGHT (C) 2018 SYNOPSYS INC.
// This software and the associated documentation are confidential and
// proprietary to Synopsys, Inc. Your use or disclosure of this software
// is subject to the terms and conditions of a written license agreement
// between you, or your company, and Synopsys, Inc. In the event of
// publications, the following notice is applicable:
//
// ALL RIGHTS RESERVED
//
// The entire notice above must be reproduced on all authorized copies.
//----------------------------------------------------------------------
`timescale 1ns/10ps

module uart_device_wrapper;

wire       clk;
wire       rstn;
wire       RTS_0, RTS_1;
wire       CTS_0, CTS_1;
wire       TxD_0, TxD_1;
wire       RxD_0, RxD_1;

wire       cmd;
wire       card_pres_mmc;

dut dut (
   .clk         (clk), 
   .rstn        (rstn),
   .RTS_0       (RTS_0),
   .CTS_0       (CTS_0), 
   .TxD_0       (TxD_0),
   .RxD_0       (RxD_0),
   .RTS_1       (RTS_1),
   .CTS_1       (CTS_1),
   .TxD_1       (TxD_1),
   .RxD_1       (RxD_1)
);

`ifdef ZEBU_NO_RTB
defparam uart_driver_0.clock_ctrl_0="clockPort0";
`endif
uart_driver uart_driver_0(
  .CTS          (RTS_0),
  .RTS          (CTS_0),
  .TxD          (RxD_0),
  .RxD          (TxD_0),
`ifndef ZEBU_NO_RTB
  .xtor_cclock0 (clk),
`endif
  .save         ()
);

`ifdef ZEBU_NO_RTB
defparam uart_driver_1.clock_ctrl_0="clockPort0";
`endif
uart_driver uart_driver_1(
  .CTS          (RTS_1),
  .RTS          (CTS_1),
  .TxD          (RxD_1),
  .RxD          (TxD_1),
`ifndef ZEBU_NO_RTB
  .xtor_cclock0 (clk),
`endif
  .save         ()
);

zceiClockPort clockPort0(
	.cclock       (clk),
	.cresetn      (rstn)
);

endmodule;
