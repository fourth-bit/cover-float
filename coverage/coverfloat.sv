`include "coverfloat_pkg.sv"
`include "coverfloat_interface.sv"
`include "coverfloat_coverage.sv"

module coverfloat (); import coverfloat_pkg::*; // TODO: maybe rename...

    logic clk = 0;
    logic [31:0] vectornum;
    logic [`COVER_VECTOR_WIDTH - 1:0] covervectors [10000:0];

    coverfloat_coverage coverage_inst;
    coverfloat_interface CFI();

    initial begin

        $readmemb("../tests/covervectors/test.txt", covervectors); // TODO: need to replace with many coverage vector files eventually...

        vectornum = 0;
        
        // CFI           = new();
        coverage_inst = new(CFI);

    end

    initial begin
        clk = 0; forever #5 clk = ~clk;
    end

    always @(posedge clk) begin
        {CFI.op, CFI.rm, CFI.a, CFI.b, CFI.c, CFI.operandFmt, CFI.result, 
         CFI.resultFmt, CFI.exceptionBits, CFI.intermS, CFI.intermX, CFI.intermM}       = covervectors[vectornum];
    end

    always @(negedge clk) begin
        // collect coverage 
        coverage_inst.sample();

        $display("test number %d with operation code %h", vectornum, CFI.op);

        vectornum = vectornum + 1;

        if (covervectors[vectornum] === `COVER_VECTOR_WIDTH'bx) $stop;
    end

endmodule