module fpga_pin_model(
    input doe,
    input do,
    output ind,
    inout pin
);
    assign pin = doe ? do : 1'bz;
    assign ind = doe ? do : pin;

endmodule
