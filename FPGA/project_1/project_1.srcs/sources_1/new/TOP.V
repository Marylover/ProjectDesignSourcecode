module top(
    input clk,
    input rst,
    input [3:0]dat,
    output pwm1,
    output pwm2,
    input knock
);
wire [7:0]angle ;
wire [7:0]angle2 ;
wire rst_n;
PWM PWM(
    .clk(clk),
    .rst_n(rst_n),
    .angle(angle),
    .pwm1(pwm1)
);
PWM PWM2(
    .clk(clk),
    .rst_n(rst_n),
    .angle(angle2),
    .pwm1(pwm2)
);
motor_top motor_top(
    .clk(clk),
    .rst_n(rst_n),
    .dat(dat),
    .angle(angle)
);
motor_top2 motor_top2(
    .clk(clk),
    .rst_n(rst_n),
    .knock(knock),
    .angle(angle2)
);
assign rst_n = ~rst;
endmodule

