// 8-bit binary counter
module counter #(
    parameter WIDTH = 8
)(
    input  logic             clk,    // clock signal
    input  logic             rst,    // synchronous reset
    input  logic             en,     // enable counter
    output logic [WIDTH-1:0] count   // counter output
);

    // Trigger on the rising edge of the clock
    always_ff @(posedge clk) begin
        if (rst) count <= {WIDTH{1'b0}};          // reset to 0
        else     count <= count + {{(WIDTH-1){1'b0}}, en};  // increment by 1 if enabled
    end
        
endmodule
