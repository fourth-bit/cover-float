covergroup B1_cg (virtual coverfloat_interface CFI);

    option.per_instance = 0;

    FP_result_ops: coverpoint CFI.op {
        type_option.weight = 0;
        // all operations that produce (arbitrary) FP results
        // `include "FP_result_op_bins.svh"
            bins op_add    = {OP_ADD & ~32'hF};
            bins op_sub    = {OP_SUB & ~32'hF};
            bins op_mul    = {OP_MUL & ~32'hF};
            bins op_div    = {OP_DIV & ~32'hF};
            bins op_rem    = {OP_REM & ~32'hF};
            bins op_qc     = {OP_QC & ~32'hF};
            bins op_feq    = {OP_FEQ};
            bins op_sc     = {OP_SC & ~32'hF};
            bins op_flt    = {OP_FLT};
            bins op_fle    = {OP_FLE};
            bins op_min    = {OP_MIN & ~32'hF};
            bins op_max    = {OP_MAX & ~32'hF};
            bins op_csn    = {OP_CSN & ~32'hF};
            bins op_fsgnj  = {OP_FSGNJ};
            bins op_fsgnjn = {OP_FSGNJN};
            bins op_fsgnjx = {OP_FSGNJX};
            bins op_fma    = {OP_FMA & ~32'hF};
            bins op_fmadd  = {OP_FMADD};
            bins op_fmsub  = {OP_FMSUB};
            bins op_fnmadd = {OP_FNMADD};
            bins op_fnmsub = {OP_FNMSUB};
            bins op_sqrt   = {OP_SQRT & ~32'hF};
    }

    FP_src1_ops: coverpoint CFI.op {
        type_option.weight = 0;
        // all operations where the first operand is FP
        // `include "FP_src1_op_bins.svh"

        // 1 fp input
        bins op_sqrt   = {OP_SQRT & ~32'hF};
        bins op_cfi    = {OP_CFI & ~32'hF};
        bins op_fcvtw  = {OP_FCVTW};
        bins op_fcvtwu = {OP_FCVTWU};
        bins op_fcvtl  = {OP_FCVTL};
        bins op_fcvtlu = {OP_FCVTLU};
        bins op_cff    = {OP_CFF & ~32'hF}; 
        bins op_class  = {OP_CLASS & ~32'hF};
    }

    FP_src2_ops: coverpoint CFI.op {
        type_option.weight = 0;
        // all operations where the second operand is FP
        // `include "FP_src2_op_bins.svh"

        // 1 fp input
        bins op_sqrt   = {OP_SQRT & ~32'hF};
        bins op_cfi    = {OP_CFI & ~32'hF};
        bins op_fcvtw  = {OP_FCVTW};
        bins op_fcvtwu = {OP_FCVTWU};
        bins op_fcvtl  = {OP_FCVTL};
        bins op_fcvtlu = {OP_FCVTLU};
        bins op_cff    = {OP_CFF & ~32'hF}; 
        bins op_class  = {OP_CLASS & ~32'hF};

        // 2 fp inputs
        bins op_add    = {OP_ADD & ~32'hF};
        bins op_sub    = {OP_SUB & ~32'hF};
        bins op_mul    = {OP_MUL & ~32'hF};
        bins op_div    = {OP_DIV & ~32'hF};
        bins op_rem    = {OP_REM & ~32'hF};
        bins op_qc     = {OP_QC & ~32'hF};
        bins op_feq    = {OP_FEQ};
        bins op_sc     = {OP_SC & ~32'hF};
        bins op_flt    = {OP_FLT};
        bins op_fle    = {OP_FLE};
        bins op_min    = {OP_MIN & ~32'hF};
        bins op_max    = {OP_MAX & ~32'hF};
        bins op_csn    = {OP_CSN & ~32'hF};
        bins op_fsgnj  = {OP_FSGNJ};
        bins op_fsgnjn = {OP_FSGNJN};
        bins op_fsgnjx = {OP_FSGNJX};
    }

    FP_src3_ops: coverpoint CFI.op {
        type_option.weight = 0;
        // all operations where the third operand is FP
        // `include "FP_src3_op_bins.svh"

        // 1 fp input
        bins op_sqrt   = {OP_SQRT & ~32'hF};
        bins op_cfi    = {OP_CFI & ~32'hF};
        bins op_fcvtw  = {OP_FCVTW};
        bins op_fcvtwu = {OP_FCVTWU};
        bins op_fcvtl  = {OP_FCVTL};
        bins op_fcvtlu = {OP_FCVTLU};
        bins op_cff    = {OP_CFF & ~32'hF}; 
        bins op_class  = {OP_CLASS & ~32'hF};

        // 2 fp inputs
        bins op_add    = {OP_ADD & ~32'hF};
        bins op_sub    = {OP_SUB & ~32'hF};
        bins op_mul    = {OP_MUL & ~32'hF};
        bins op_div    = {OP_DIV & ~32'hF};
        bins op_rem    = {OP_REM & ~32'hF};
        bins op_qc     = {OP_QC & ~32'hF};
        bins op_feq    = {OP_FEQ};
        bins op_sc     = {OP_SC & ~32'hF};
        bins op_flt    = {OP_FLT};
        bins op_fle    = {OP_FLE};
        bins op_min    = {OP_MIN & ~32'hF};
        bins op_max    = {OP_MAX & ~32'hF};
        bins op_csn    = {OP_CSN & ~32'hF};
        bins op_fsgnj  = {OP_FSGNJ};
        bins op_fsgnjn = {OP_FSGNJN};
        bins op_fsgnjx = {OP_FSGNJX};
    
        // 3 fp inputs
        bins op_fma    = {OP_FMA & ~32'hF};
        bins op_fmadd  = {OP_FMADD};
        bins op_fmsub  = {OP_FMSUB};
        bins op_fnmadd = {OP_FNMADD};
        bins op_fnmsub = {OP_FNMSUB};

    }

    F32_src_fmt: coverpoint CFI.operandFmt == FMT_SINGLE {
        type_option.weight = 0;
        // single precision format for operands
        bins f32 = {1};
    }

    F32_result_fmt: coverpoint CFI.resultFmt == FMT_SINGLE {
        type_option.weight = 0;
        // single precision format for result
        bins f32 = {1};
    }

    F32_src1_basictypes: coverpoint CFI.a[31:0] {
        type_option.weight = 0;
        bins pos0             = {32'h00000000};
        bins neg0             = {32'h80000000};
        bins pos1             = {32'h3f800000};
        bins neg1             = {32'hbf800000};
        bins pos1p5           = {32'h3fc00000};
        bins neg1p5           = {32'hbfc00000};
        bins pos2             = {32'h40000000};
        bins neg2             = {32'hc0000000};
        bins posminnorm       = {32'h00800000};
        bins mnegminnorm      = {32'h80800000};
        bins posmaxnorm       = {32'h7f7fffff};
        bins negmaxnorm       = {32'hff7fffff};
        bins posnorm          = {[32'h00800000:32'h7f7fffff]};
        bins negnorm          = {[32'h80800000:32'hff7fffff]};
        bins posmax_subnorm   = {32'h007fffff};
        bins negmax_subnorm   = {32'h807fffff};
        bins posmid_subnorm   = {32'h00400000};
        bins negmid_subnorm   = {32'h80400000};
        bins posmin_subnorm   = {32'h00000001};
        bins negmin_subnorm   = {32'h80000001};
        bins pos_subnorm      = {[32'h00000001:32'h007fffff]};
        bins neg_subnorm      = {[32'h80000001:32'h807fffff]};
        bins posinfinity      = {32'h7f800000};
        bins neginfinity      = {32'hff800000};
        bins posQNaN          = {[32'h7fc00000:32'h7fffffff]};
        bins posSNaN          = {[32'h7f800001:32'h7fbfffff]};
        bins negQNaN          = {[32'hffc00000:32'hffffffff]};
        bins negSNaN          = {[32'hff800001:32'hffbfffff]};
    }

    F32_src2_basictypes: coverpoint CFI.b[31:0] {
        type_option.weight = 0;
        bins pos0             = {32'h00000000};
        bins neg0             = {32'h80000000};
        bins pos1             = {32'h3f800000};
        bins neg1             = {32'hbf800000};
        bins pos1p5           = {32'h3fc00000};
        bins neg1p5           = {32'hbfc00000};
        bins pos2             = {32'h40000000};
        bins neg2             = {32'hc0000000};
        bins posminnorm       = {32'h00800000};
        bins mnegminnorm      = {32'h80800000};
        bins posmaxnorm       = {32'h7f7fffff};
        bins negmaxnorm       = {32'hff7fffff};
        bins posnorm          = {[32'h00800000:32'h7f7fffff]};
        bins negnorm          = {[32'h80800000:32'hff7fffff]};
        bins posmax_subnorm   = {32'h007fffff};
        bins negmax_subnorm   = {32'h807fffff};
        bins posmid_subnorm   = {32'h00400000};
        bins negmid_subnorm   = {32'h80400000};
        bins posmin_subnorm   = {32'h00000001};
        bins negmin_subnorm   = {32'h80000001};
        bins pos_subnorm      = {[32'h00000001:32'h007fffff]};
        bins neg_subnorm      = {[32'h80000001:32'h807fffff]};
        bins posinfinity      = {32'h7f800000};
        bins neginfinity      = {32'hff800000};
        bins posQNaN          = {[32'h7fc00000:32'h7fffffff]};
        bins posSNaN          = {[32'h7f800001:32'h7fbfffff]};
        bins negQNaN          = {[32'hffc00000:32'hffffffff]};
        bins negSNaN          = {[32'hff800001:32'hffbfffff]};
    }

    F32_src3_basictypes: coverpoint CFI.c[31:0] {
        type_option.weight = 0;
        bins pos0             = {32'h00000000};
        bins neg0             = {32'h80000000};
        bins pos1             = {32'h3f800000};
        bins neg1             = {32'hbf800000};
        bins pos1p5           = {32'h3fc00000};
        bins neg1p5           = {32'hbfc00000};
        bins pos2             = {32'h40000000};
        bins neg2             = {32'hc0000000};
        bins posminnorm       = {32'h00800000};
        bins mnegminnorm      = {32'h80800000};
        bins posmaxnorm       = {32'h7f7fffff};
        bins negmaxnorm       = {32'hff7fffff};
        bins posnorm          = {[32'h00800000:32'h7f7fffff]};
        bins negnorm          = {[32'h80800000:32'hff7fffff]};
        bins posmax_subnorm   = {32'h007fffff};
        bins negmax_subnorm   = {32'h807fffff};
        bins posmid_subnorm   = {32'h00400000};
        bins negmid_subnorm   = {32'h80400000};
        bins posmin_subnorm   = {32'h00000001};
        bins negmin_subnorm   = {32'h80000001};
        bins pos_subnorm      = {[32'h00000001:32'h007fffff]};
        bins neg_subnorm      = {[32'h80000001:32'h807fffff]};
        bins posinfinity      = {32'h7f800000};
        bins neginfinity      = {32'hff800000};
        bins posQNaN          = {[32'h7fc00000:32'h7fffffff]};
        bins posSNaN          = {[32'h7f800001:32'h7fbfffff]};
        bins negQNaN          = {[32'hffc00000:32'hffffffff]};
        bins negSNaN          = {[32'hff800001:32'hffbfffff]};
    }

    F32_result_basictypes: coverpoint CFI.result[31:0] {
        type_option.weight = 0;
        bins pos0             = {32'h00000000};
        bins neg0             = {32'h80000000};
        bins pos1             = {32'h3f800000};
        bins neg1             = {32'hbf800000};
        bins pos1p5           = {32'h3fc00000};
        bins neg1p5           = {32'hbfc00000};
        bins pos2             = {32'h40000000};
        bins neg2             = {32'hc0000000};
        bins posminnorm       = {32'h00800000};
        bins mnegminnorm      = {32'h80800000};
        bins posmaxnorm       = {32'h7f7fffff};
        bins negmaxnorm       = {32'hff7fffff};
        bins posnorm          = {[32'h00800000:32'h7f7fffff]};
        bins negnorm          = {[32'h80800000:32'hff7fffff]};
        bins posmax_subnorm   = {32'h007fffff};
        bins negmax_subnorm   = {32'h807fffff};
        bins posmid_subnorm   = {32'h00400000};
        bins negmid_subnorm   = {32'h80400000};
        bins posmin_subnorm   = {32'h00000001};
        bins negmin_subnorm   = {32'h80000001};
        bins pos_subnorm      = {[32'h00000001:32'h007fffff]};
        bins neg_subnorm      = {[32'h80000001:32'h807fffff]};
        bins posinfinity      = {32'h7f800000};
        bins neginfinity      = {32'hff800000};
        bins posQNaN          = {[32'h7fc00000:32'h7fffffff]};
        bins posSNaN          = {[32'h7f800001:32'h7fbfffff]};
        bins negQNaN          = {[32'hffc00000:32'hffffffff]};
        bins negSNaN          = {[32'hff800001:32'hffbfffff]};
    }

    // main coverpoints

    `ifdef COVER_F32
        B1_F32_1_operands: cross FP_src1_ops,   F32_src1_basictypes,                                           F32_src_fmt;
        B1_F32_2_operands: cross FP_src2_ops,   F32_src1_basictypes, F32_src2_basictypes,                      F32_src_fmt;
        B1_F32_3_operands: cross FP_src3_ops,   F32_src1_basictypes, F32_src2_basictypes, F32_src3_basictypes, F32_src_fmt;
        B1_F32_result:     cross FP_result_ops, F32_result_basictypes,                                         F32_result_fmt;
    `endif // COVER_F32

    // `ifdef COVER_F32
    // `ifdef COVER_F64
    //     B1_F32_F64_operand: cross 
    //     B1_F64_F32_operand: cross
    //     B1_F32_F64_result:  cross 
    //     B1_F64_F32_result:  cross
    // `endif 
    // `endif // COVER_F32 && COVER_F64
endgroup