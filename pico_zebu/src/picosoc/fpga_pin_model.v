module fpga_pin_model(
    input doe,
    input pdo,
    output ind,
    inout pin
);
    assign pin = doe ? pdo : 1'bz;
    assign ind = doe ? pdo : pin;

endmodule
