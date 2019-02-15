
// No timescale specified
module Gceuk5(Sceuk5, Xceuk5, Bdeuk5, Edeuk5, Mdeuk5);

parameter Aeeuk5 = 3'b0;
parameter Ieeuk5 = 3'b1;
parameter Qeeuk5 = 3'b010;
parameter Zeeuk5 = 3'b011;
parameter Gfeuk5 = 3'b100;
parameter Pfeuk5 = 3'b101;
parameter Wfeuk5 = 1'b0;
parameter Zfeuk5 = 1'b1;
parameter Cgeuk5 = 1'bx;
parameter Egeuk5 = 8;

input Sceuk5;
input Xceuk5;
input Bdeuk5;
output [7:0] Edeuk5;
output Mdeuk5;

reg [2:0] Ngeuk5;
reg [2:0] Ygeuk5;
reg Eheuk5;
reg Nheuk5;
reg [3:0] Aieuk5;
reg Oieuk5;
reg Ajeuk5;
reg [7:0] Mjeuk5;
reg Wjeuk5;
reg [3:0] Dkeuk5;
reg Rkeuk5;
reg Ykeuk5;
reg Ileuk5;
reg Mdeuk5;

assign Edeuk5 = Mjeuk5;

always @(posedge Xceuk5) if (~Sceuk5) begin
Nheuk5 <= 1;
Eheuk5 <= 1;
end
else
begin
Nheuk5 <= Bdeuk5;
Eheuk5 <= Nheuk5;
end
always @(posedge Xceuk5) if (~Sceuk5) begin
Aieuk5 <= 0;
end
else if (Oieuk5) begin
Aieuk5 <= 0;
end
else begin
Aieuk5 <= (Aieuk5 + 1);
end
always @(posedge Xceuk5) if ((~Sceuk5) || Ajeuk5) begin
Mjeuk5 <= 0;
end
else if (Wjeuk5) begin
Mjeuk5[6:0] <= Mjeuk5[7:1];
Mjeuk5[7] <= Eheuk5;
end
always @(posedge Xceuk5) if (~Sceuk5) begin
Dkeuk5 <= 0;
end
else if (Rkeuk5) begin
Dkeuk5 <= (Dkeuk5 + 1);
end
else if (Ykeuk5) begin
Dkeuk5 <= 0;
end
always @(posedge Xceuk5) if (~Sceuk5) begin
Ygeuk5 <= Aeeuk5;
end
else begin
Ygeuk5 <= Ngeuk5;
end
always @(Ygeuk5 or Eheuk5 or Aieuk5 or Dkeuk5) begin
Ngeuk5 = Ygeuk5;
Oieuk5 = Zfeuk5;
Ajeuk5 = Wfeuk5;
Wjeuk5 = Wfeuk5;
Rkeuk5 = Wfeuk5;
Ykeuk5 = Wfeuk5;
Ileuk5 = Ileuk5;
case (Ygeuk5)
Aeeuk5: begin
if (~Eheuk5) begin
Ngeuk5 = Qeeuk5;
end
else
begin
Ngeuk5 = Ieeuk5;
Ykeuk5 = Zfeuk5;
Ileuk5 = Wfeuk5;
end
end
Ieeuk5: begin
if (~Eheuk5) begin
Ngeuk5 = Qeeuk5;
end
else
begin
Ngeuk5 = Ieeuk5;
Ykeuk5 = Zfeuk5;
Ileuk5 = Ileuk5;
end
end
Qeeuk5: begin
Ileuk5 = Wfeuk5;
Ajeuk5 = Zfeuk5;
if (Aieuk5 == 4'h4) begin
if (~Eheuk5) begin
Ngeuk5 = Zeeuk5;
end
else begin
Ngeuk5 = Ieeuk5;
end
end
else
begin
Ngeuk5 = Qeeuk5;
Oieuk5 = Wfeuk5;
end
end
Zeeuk5: begin
if (Aieuk5 == 4'he) begin
if (Dkeuk5 == Egeuk5) begin
Ngeuk5 = Pfeuk5;
end
else
begin
Ngeuk5 = Gfeuk5;
end
end
else
begin
Ngeuk5 = Zeeuk5;
Oieuk5 = Wfeuk5;
end
end
Gfeuk5: begin
Wjeuk5 = Zfeuk5;
Rkeuk5 = Zfeuk5;
Ngeuk5 = Zeeuk5;
end
Pfeuk5: begin
Ngeuk5 = Ieeuk5;
Ileuk5 = Zfeuk5;
end
default: begin
Ngeuk5 = 3'bxxx;
Oieuk5 = Cgeuk5;
Wjeuk5 = Cgeuk5;
Rkeuk5 = Cgeuk5;
Ykeuk5 = Cgeuk5;
Ileuk5 = Cgeuk5;
end
endcase
end
always @(posedge Xceuk5) if (~Sceuk5) begin
Mdeuk5 <= 0;
end
else begin
Mdeuk5 <= Ileuk5;
end
endmodule

// No timescale specified
module Emeuk5(Xceuk5, Sceuk5, Smeuk5, Vmeuk5, Dneuk5, Pneuk5);

parameter Xneuk5 = 3'b0;
parameter Eoeuk5 = 3'b010;
parameter Moeuk5 = 3'b011;
parameter Toeuk5 = 3'b100;
parameter Bpeuk5 = 3'b101;
parameter Ipeuk5 = 2'b0;
parameter Tpeuk5 = 2'b1;
parameter Dqeuk5 = 2'b10;
parameter Wfeuk5 = 1'b0;
parameter Zfeuk5 = 1'b1;
parameter Cgeuk5 = 1'bx;
parameter Egeuk5 = 8;

input Xceuk5;
input Sceuk5;
output Smeuk5;
input Dneuk5;
input [7:0] Vmeuk5;
output Pneuk5;

reg [2:0] Ngeuk5;
reg [2:0] Ygeuk5;
reg Oqeuk5;
reg Dreuk5;
reg [4:0] Aieuk5;
reg Nreuk5;
reg [7:0] Xreuk5;
reg [3:0] Mseuk5;
reg Wseuk5;
reg Kteuk5;
reg [1:0] Yteuk5;
reg Smeuk5;
reg Lueuk5;
reg Pneuk5;

always @(Xreuk5 or Yteuk5) case (Yteuk5)
Ipeuk5:
Smeuk5 = Wfeuk5;
Tpeuk5:
Smeuk5 = Zfeuk5;
Dqeuk5:
Smeuk5 = Xreuk5[0];
default:
Smeuk5 = Cgeuk5;
endcase
always @(posedge Xceuk5) if (~Sceuk5) begin
Aieuk5 <= 0;
end
else if (Nreuk5) begin
Aieuk5 <= (Aieuk5 + 1);
end
else begin
Aieuk5 <= 0;
end
always @(posedge Xceuk5) if (~Sceuk5) begin
Xreuk5 <= 0;
end
else if (Oqeuk5) begin
Xreuk5 <= Vmeuk5;
end
else if (Dreuk5) begin
Xreuk5[6:0] <= Xreuk5[7:1];
Xreuk5[7] <= Zfeuk5;
end
else begin
Xreuk5 <= Xreuk5;
end
always @(posedge Xceuk5) if (~Sceuk5) begin
Mseuk5 <= 0;
end
else if (Wseuk5) begin
Mseuk5 <= 0;
end
else if (Kteuk5) begin
Mseuk5 <= (Mseuk5 + 1);
end
always @(posedge Xceuk5) if (~Sceuk5) begin
Ygeuk5 <= Xneuk5;
end
else begin
Ygeuk5 <= Ngeuk5;
end
always @(Ygeuk5 or Dneuk5 or Aieuk5 or Mseuk5) begin
Ngeuk5 = Ygeuk5;
Oqeuk5 = Wfeuk5;
Nreuk5 = Wfeuk5;
Dreuk5 = Wfeuk5;
Wseuk5 = Wfeuk5;
Kteuk5 = Wfeuk5;
Yteuk5 = Tpeuk5;
Lueuk5 = Wfeuk5;
case (Ygeuk5)
Xneuk5: begin
if (Dneuk5) begin
Ngeuk5 = Eoeuk5;
Oqeuk5 = Zfeuk5;
end
else
begin
Ngeuk5 = Xneuk5;
Lueuk5 = Zfeuk5;
end
Wseuk5 = Zfeuk5;
end
Eoeuk5: begin
Yteuk5 = Ipeuk5;
if (Aieuk5 == 4'hf) begin
Ngeuk5 = Moeuk5;
Kteuk5 = Zfeuk5;
end
else
begin
Ngeuk5 = Eoeuk5;
Nreuk5 = Zfeuk5;
end
end
Moeuk5: begin
Yteuk5 = Dqeuk5;
Ngeuk5 = Moeuk5;
Nreuk5 = Zfeuk5;
if (Mseuk5 == Egeuk5) begin
if (Aieuk5 == 4'hf) begin
Ngeuk5 = Bpeuk5;
Nreuk5 = Wfeuk5;
end
end
else
begin
if (Aieuk5 == 4'he) begin
Ngeuk5 = Toeuk5;
Kteuk5 = Zfeuk5;
Nreuk5 = Wfeuk5;
end
end
end
Toeuk5: begin
Yteuk5 = Dqeuk5;
Ngeuk5 = Moeuk5;
Dreuk5 = Zfeuk5;
end
Bpeuk5: begin
Yteuk5 = Tpeuk5;
if (Aieuk5 == 4'hf) begin
Ngeuk5 = Xneuk5;
Lueuk5 = Zfeuk5;
end
else
begin
Ngeuk5 = Bpeuk5;
Nreuk5 = Zfeuk5;
end
end
default: begin
Ngeuk5 = 3'bxxx;
Oqeuk5 = Cgeuk5;
Nreuk5 = Cgeuk5;
Dreuk5 = Cgeuk5;
Wseuk5 = Cgeuk5;
Kteuk5 = Cgeuk5;
Yteuk5 = 2'bxx;
Lueuk5 = Cgeuk5;
end
endcase
end
always @(posedge Xceuk5) if (~Sceuk5) begin
Pneuk5 <= 0;
end
else begin
Pneuk5 <= Lueuk5;
end
endmodule

// No timescale specified
module dut(clk, rstn, RTS_0, CTS_0, TxD_0, RxD_0, RTS_1, CTS_1, TxD_1, RxD_1);

input clk;
input rstn;
output TxD_0;
input RxD_0;
output RTS_0;
input CTS_0;
output TxD_1;
input RxD_1;
output RTS_1;
input CTS_1;

wire [7:0] Exeuk5;
wire Oxeuk5;
wire Eyeuk5;
reg Zyeuk5;
reg Rzeuk5;
reg K0fuk5;
reg E1fuk5;
reg [7:0] S1fuk5;
wire [7:0] C2fuk5;
wire M2fuk5;
wire C3fuk5;
reg X3fuk5;
reg P4fuk5;
reg I5fuk5;
reg C6fuk5;
reg [7:0] Q6fuk5;
reg A7fuk5;
wire P7fuk5;
wire P8fuk5;

assign RTS_0 = 0;
assign RTS_1 = 0;
assign Eyeuk5 = ((((K0fuk5 == 0) && (Rzeuk5 == 0)) && (Zyeuk5 == 1)) && (Oxeuk5 
== 1));
assign C3fuk5 = ((((I5fuk5 == 0) && (P4fuk5 == 0)) && (X3fuk5 == 1)) && (M2fuk5 
== 1));

Emeuk5 H7fuk5( .Xceuk5 (clk),  .Sceuk5 (A7fuk5),  .Smeuk5 (TxD_0),  .Vmeuk5
(S1fuk5),  .Dneuk5 (E1fuk5),  .Pneuk5 (P7fuk5));
Gceuk5 Z7fuk5( .Sceuk5 (A7fuk5),  .Xceuk5 (clk),  .Bdeuk5 (RxD_0),  .Edeuk5
(Exeuk5),  .Mdeuk5 (Oxeuk5));
Emeuk5 H8fuk5( .Xceuk5 (clk),  .Sceuk5 (A7fuk5),  .Smeuk5 (TxD_1),  .Vmeuk5
(Q6fuk5),  .Dneuk5 (C6fuk5),  .Pneuk5 (P8fuk5));
Gceuk5 Z8fuk5( .Sceuk5 (A7fuk5),  .Xceuk5 (clk),  .Bdeuk5 (RxD_1),  .Edeuk5
(C2fuk5),  .Mdeuk5 (M2fuk5));

always @(posedge clk or negedge rstn) begin
if (~rstn) begin
A7fuk5 <= 1'b0;
end
else
begin
A7fuk5 <= 1'b1;
end
end
always @(posedge clk) begin
if (!A7fuk5) begin
Zyeuk5 <= 0;
Rzeuk5 <= 0;
K0fuk5 <= 0;
end
else
begin
Zyeuk5 <= Oxeuk5;
Rzeuk5 <= Zyeuk5;
K0fuk5 <= Rzeuk5;
end
end
always @(posedge clk) begin
if (!A7fuk5) begin
S1fuk5 <= 0;
E1fuk5 <= 0;
end
else
begin
if (C3fuk5) begin
S1fuk5 <= C2fuk5;
end
E1fuk5 <= C3fuk5;
end
end
always @(posedge clk) begin
if (!A7fuk5) begin
X3fuk5 <= 0;
P4fuk5 <= 0;
I5fuk5 <= 0;
end
else
begin
X3fuk5 <= M2fuk5;
P4fuk5 <= X3fuk5;
I5fuk5 <= P4fuk5;
end
end
always @(posedge clk) begin
if (!A7fuk5) begin
Q6fuk5 <= 0;
C6fuk5 <= 0;
end
else
begin
if (Eyeuk5) begin
Q6fuk5 <= Exeuk5;
end
C6fuk5 <= Eyeuk5;
end
end
endmodule

// END: VCS tokens
