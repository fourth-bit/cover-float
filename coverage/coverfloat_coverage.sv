import coverfloat_pkg::*;
class coverfloat_coverage;

    `include "covergroups/B1.svh"
    /* ... */

    virtual coverfloat_interface CFI;
    // B1_cg B1;
    // ...

    // constructor (initializes covergroups)
    function new (virtual coverfloat_interface CFI);
        this.CFI = CFI;

        B1_cg = new(CFI);
        /* ... */
    endfunction

    
    function void sample();
        
        // Call sample functions (probably `include 'd)
        B1_cg.sample();
        // ...

    endfunction

endclass