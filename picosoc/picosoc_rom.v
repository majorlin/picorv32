`timescale 1 ns / 1 ps
module picosoc_rom #(
	parameter integer WORDS = 16*256
) (
	input clk, resetn,
	input valid,
	output ready,
	input [17:0] addr,
	output reg [31:0] rdata
);
	reg [31:0] mem [0:WORDS-1] /* synthesis syn_ramstyle="block_ram" */;

    assign ready = 1'b1;

	always @(posedge clk) begin
		rdata <= valid ? mem[addr] : 32'h0;
	end
endmodule
