module motor_top2(
    input     rst_n,
    input     clk,
    input knock,
    output [7:0]angle
    );
reg [7:0]angle_r;
reg  buff;
reg _buff;
always@(posedge clk or negedge rst_n)
    begin
        if(rst_n == 0)
            begin
            angle_r <= 0;
            end
        else
    _buff <= knock;
    buff <= _buff;
    case({_buff,buff})
    2'b10:
        begin
        angle_r <= 120;
        end
    2'b01:
        begin
        angle_r <= 180;
        end
    default:
        begin
        angle_r <= angle_r;
        end
    endcase

end
assign angle = angle_r;

endmodule
