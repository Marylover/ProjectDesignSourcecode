module motor_top(
    input     rst_n,
    input     clk,
    input [3:0]dat,
    output [7:0]angle
    );
reg [7:0]angle_r;
    
always@(posedge clk or negedge rst_n)
    begin
        if(rst_n == 0)
            begin
            angle_r <= 0;
            end
        else
    case(dat)
    0:
        begin
        angle_r <= 0;
        end
    1:
        begin
        angle_r <= 22;
        end
    2:
        begin
        angle_r <= 42;
        end
    3:
        begin
        angle_r <= 61;
        end
    4:
        begin
        angle_r <= 80;
        end
    5:
        begin
        angle_r <= 99;
        end
    6:
        begin
        angle_r <= 116;
        end
    7:
        begin
        angle_r <= 137;
        end
    8:
        begin
        angle_r <= 159;
        end
    endcase

end
assign angle = angle_r;

endmodule

