//! If DEBUG_TRAPS_ENABLED is active (not commented out), then code will stop if a trap "CONTROLLED_MCU_RESET() is hit
//! If DEBUG_TRAPS_ENABLED is is not active (//commented out), then MCU will be RESET if a trap "CONTROLLED_MCU_RESET() is hit
//!     After MCU reset code continues to execute normally. This is practically self-recover.
//!     In normal conditions any traps should not ever be hit. There should be no logical path to any traps.
// #define DEBUG_TRAPS_ENABLED        //!< Comment out this line from production code. Enable the line for debug code

#ifdef DEBUG_TRAPS_ENABLED

typedef enum    // The state machine states
{
    code_branch_id_01   = 1,
    code_branch_id_02,
    code_branch_id_03,
    code_branch_id_04,
    code_branch_id_05,
    code_branch_id_06,
    code_branch_id_07,
    code_branch_id_08,
    code_branch_id_09,
    code_branch_id_10,
    code_branch_id_11,
    code_branch_id_12,
    code_branch_id_13,
    code_branch_id_14,
    code_branch_id_15,
    code_branch_id_16,
    code_branch_id_17,
    code_branch_id_18,
    code_branch_id_19,
    code_branch_id_20,
    code_branch_id_21,
    code_branch_id_22,
    code_branch_id_23,
    code_branch_id_24,
    code_branch_id_25,
    code_branch_id_26,

    null_pointer_id_01,
    null_pointer_id_02,
    null_pointer_id_03,
    null_pointer_id_04,
    null_pointer_id_05,
    null_pointer_id_06,
    null_pointer_id_07,
    null_pointer_id_08,
    null_pointer_id_09,
    null_pointer_id_10,
    null_pointer_id_11,
    null_pointer_id_12,
    null_pointer_id_13

} illegal_code_branch_id_enum;

void DEBUG_TRAP_CODE_BLINK(illegal_code_branch_id_enum u8NumberOfBlinks);

// debug code:
#define CONTROLLED_MCU_RESET(u8_Id_of_illegal_code_branch)				{ DEBUG_TRAP_CODE_BLINK(u8_Id_of_illegal_code_branch); } // call a function named DEBUG_TRAP_CODE_BLINK, which leads infinite loop and blinks red LED

#define ASSERT_NULL_PTR(check_ptr, u8_Id_of_illegal_code_branch)	    { if (check_ptr == NULL_PTR) { DEBUG_TRAP_CODE_BLINK(u8_Id_of_illegal_code_branch); } }

#else

// Production code:
#define CONTROLLED_MCU_RESET(u8_Id_of_illegal_code_branch)	            { gb_Time_To_Reset = TRUE; }	// In production code the argument is not used

#define ASSERT_NULL_PTR(check_ptr, u8NumberOfBlinks)  		            { if (check_ptr == NULL_PTR) { gb_Time_To_Reset = TRUE; } } // else nothing

#endif

extern bool gb_Time_To_Reset;
