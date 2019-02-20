/*
*  PicoSoC - A simple example SoC using PicoRV32
*
*  Copyright (C) 2017  Clifford Wolf <clifford@clifford.at>
*
*  Permission to use, copy, modify, and/or distribute this software for any
*  purpose with or without fee is hereby granted, provided that the above
*  copyright notice and this permission notice appear in all copies.
*
*  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
*  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
*  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
*  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
*  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
*  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
*/

`timescale 1 ns / 1 ps

module testbench;

    wire RXD;
    wire TXD;
    wire nrst;
    wire [7:0] leds;
    wire clk;

    reg zebu_rst_n;
    reg [4:0] rst_counter;
    always @(posedge clk) 
    begin
            if (rst_counter < 5) begin
                        rst_counter <= rst_counter + 1;
                        zebu_rst_n <= 1;
                    end 
            else if(rst_counter < 25) begin
                        rst_counter <= rst_counter + 1;
                        zebu_rst_n <= 0;
                    end
            else begin
                        zebu_rst_n <= 1;
                    end
    end 

    picoram uut (
        .clk      (clk      ),
        .leds     (leds     ),
        .reset_n  (zebu_rst_n),
        .ser_rx   (RXD),
        .ser_tx   (TXD),
        .irq_5      (1'b0),
        .irq_6      (1'b0),
        .irq_7      (1'b0)
    );

	initial $readmemh("/proj/mcu_zebu/users/b51677/picorv32/pico_zebu/src/rom_fw.hex", testbench.uut.memory.mem);

    `ifdef ZEBU_NO_RTB
        defparam uart_driver_0.clock_ctrl_0="clockPort0";
    `endif
    uart_driver uart_driver_0(
        .TxD          (RXD),
        .RxD          (TXD),
        `ifndef ZEBU_NO_RTB
            .xtor_cclock0 (clk),
        `endif
        .save         ()
    );

        zceiClockPort clockPort0(
            .cclock       (clk)
        );

endmodule
