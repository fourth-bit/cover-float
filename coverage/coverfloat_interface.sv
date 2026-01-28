interface coverfloat_interface; import coverfloat_pkg::*; // TODO: add params for covervector / DUT modes?

    // logic         clk;

    // logic         valid;

    logic [31:0]  op;

    logic [7:0]  rm;

    // logic [31:0]  enableBits; // legacy, not required for riscv TODO: consider having coverage based on these as a config option
    
    logic [127:0] a, b, c;
    logic [7:0]   operandFmt; 

    logic [127:0] result;
    logic [7:0]   resultFmt;

    logic         intermS;
    logic [31:0]  intermX;
    logic [191:0] intermM;

    logic [7:0]  exceptionBits;

endinterface