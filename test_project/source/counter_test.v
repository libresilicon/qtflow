`include "counter.v"

`timescale 1ns/1ps

module counter_test();
    reg clk_tb,clr_tb;
    wire [3:0]q_tb;
    counter dut1(q_tb,clk_tb,clr_tb);
    initial 
        begin
        clr_tb=1'b0;
        #50 clr_tb=1'b1;
        #100 clr_tb=1'b0;

$dumpfile("counter_test.vcd");

$dumpvars(0,counter_test);

# 1501 $finish;
        end
        always 
            begin
            #50 clk_tb=1'b1;
            #50 clk_tb=1'b0;
            end
endmodule
