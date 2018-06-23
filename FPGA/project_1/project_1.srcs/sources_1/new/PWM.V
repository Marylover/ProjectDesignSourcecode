`timescale 1ns/1ps
//100MHz??
module PWM(
    input     rst_n,
    input     clk,
    input     [7:0]angle,
    output    reg pwm1); 
    reg[31:0] counter1;

    always@(posedge clk or negedge rst_n)
        begin
        if(rst_n == 0)
            begin
            pwm1 <= 0;
            counter1 <= 0;
            end
        else
            begin 
            counter1 <= counter1 + 1;
            if(counter1 >= angle*1111 + 50000)
                begin
                pwm1 <= 0;
                end
            else
                begin
                pwm1 <= 1;
                end
            if(counter1 == 2000000 + 50000)
                begin
                counter1 <= 0;
                end
            end
        end
endmodule
