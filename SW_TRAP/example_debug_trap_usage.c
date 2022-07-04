// Examples how to use this debug trap idea
// Just piece of code parts here and there

//-----------------------------------------------------------------------------

// inside isr.h
#define ISR_SUPERVISION_LIMIT   0xFFFF  // Max limit background task is allowed to increment without interrupts running
                                        // Tested with value 10: catch immediately and every time
                                        // Value 100: Catch only when increase input voltage slowly, not during normal op

// inside ISR:

	gu16_ISR_SuperVision = 0; // continuously reset to zero every time when ISR happenss. The simplest (shortest) code inside ISR.



// in background code (at the end of endless loop in main.c):

    // Supervision against stopped interrupts
    // Logic: gu16_ISR_SuperVision counter is incremented here. Every (any) ISR reset the counter to zero. If counter happens to
    // reach the limit value, then it is judged that interrupts are stopped, which will lead to trap/controlled MCU reset
    gu16_ISR_SuperVision++;
    if (gu16_ISR_SuperVision >= ISR_SUPERVISION_LIMIT)
    {
        CONTROLLED_MCU_RESET(code_branch_id_16);
    }

//-----------------------------------------------------------------------------
// example how used in empty 'illegal' default: of switch-case.
// every default: in whole SW was placed similar code, each with unique id

        case CC_VISUAL_INDICATION_REQ:
            u8_error_flags = u8_Visual_Indication(au8_data_buffer[VISUAL_INDICATION_INDEX]);
            if(u8_error_flags == NO_ERROR)
            {
                au8_data_buffer[MESSAGE_INDEX] = CC_VISUAL_INDICATION_CFM;
            }
            else
            {
                au8_data_buffer[MESSAGE_INDEX] = CC_VISUAL_INDICATION_REJ;
                au8_data_buffer[ERROR_CODE_INDEX] = u8_error_flags;
            }
            break;

        default:
            CONTROLLED_MCU_RESET(code_branch_id_22);
            break;
    }

    return;
}

//-----------------------------------------------------------------------------
// another example of trap in empty default: 

	...
    case PDU2_GET_CB_MODULE_PID_REQ:
        u8_error_flags = u8_ECB_Get_ECM_PID(au8_data_buffer[SUB_UNIT_ID_INDEX]);
        break;

    default:
        // NA
        CONTROLLED_MCU_RESET(code_branch_id_23);
        break;
    }


//-----------------------------------------------------------------------------
// example of empty else branch in if, else-if, else 

	...
       }
    }
    else
    {
    	// Empty branch
        CONTROLLED_MCU_RESET(code_branch_id_09); // Note! This activates during the testing in debug mode when slowly decrease input voltage below -37.50V level
    }

    return;
}

//-----------------------------------------------------------------------------
// example of unused interrupt service routine, which like other traps, should not ever happen

#pragma interruptlow  ISR_Low //lint !e975
void ISR_Low(void)  //lint !e714 !e765
{
    CONTROLLED_MCU_RESET(code_branch_id_12);
    return;
}

//-----------------------------------------------------------------------------
// example how the same trap mechanims was used to handle unexpexted MCU resets:


// v_Resolve_MCU_RESET(); was called in the beginning of main.c

// Reading MCU datasheet very carefully reveals way to detect unexpected MCU reset
// Not all MCUs has this many different reasons for MCU resets

// Unexpected MCU reset in a PDU is typically a very bad thing to happen, because then radios (PDU outputs) will also turn OFF unexpectedly
// Even short unexpected output OFF-ON should not ever happen in field

#ifdef DEBUG_TRAPS_ENABLED
void v_Resolve_MCU_RESET(void)
{
    /*
    The PIC18F87K22 family of devices differentiates
    between various kinds of Reset:
    a) Power-on Reset (POR)
    b) MCLR Reset during normal operation
    c) MCLR Reset during power-managed modes
    d) Watchdog Timer (WDT) Reset (during execution)
    e) Configuration Mismatch (CM) Reset
    f) Brown-out Reset (BOR)
    g) RESET Instruction
    h) Stack Full Reset
    i) Stack Underflow Reset
    */
    
    // 2. Then check which bits are set/cleared
    
    if ( gu8_STKPTR_REGISTER & STACK_FULL )
    {
        // bit 7 STKFUL: Stack Full Flag bit(1)
        // 1 = Stack has become full or overflowed
        // 0 = Stack has not become full or overflowed

        CONTROLLED_MCU_RESET(code_branch_id_17);
    }
    
    if ( gu8_STKPTR_REGISTER & STACK_UNDERFLOW )
    {
        // bit 6 STKUNF: Stack Underflow Flag bit(1)
        // 1 = Stack underflow has occurred
        // 0 = Stack underflow did not occur

        CONTROLLED_MCU_RESET(code_branch_id_18);
    }
    
    if ( (gu8_RCON_REGISTER & CONFIGURATION_MISMATCH) == RESET_OCCURED )
    {
        // bit 5 CM: Configuration Mismatch Flag bit
        // 1 = A Configuration Mismatch Reset has not occurred
        // 0 = A Configuration Mismatch Reset has occurred
        // Must be set in software after a Configuration Mismatch Reset occurs
        RCONbits.CM = 1;
    
        CONTROLLED_MCU_RESET(code_branch_id_19);
    }
    
    if ((gu8_RCON_REGISTER & RESET_INSTRUCTION) == RESET_OCCURED ) 
    {
        // bit 4 RI: RESET Instruction Flag bit
        // 1 = The RESET instruction was not executed (set by firmware only)
        // 0 = The RESET instruction was executed causing a device Reset
        // Must be set in software after a Brown-out Reset occurs
        RCONbits.RI = 1;
        RCONbits.POR = 1;
        RCONbits.BOR = 1;

        // CONTROLLED_MCU_RESET(code_branch_id_20); Commented out because traps are enabled -> it comes here after Uin falls below -37.5V
    }
    
    if ((gu8_RCON_REGISTER & WATCHDOG_TIMEOUT) == RESET_OCCURED )
    {
        // bit 3 TO: Watchdog Time-out Flag bit
        // 1 = Set by power-up, CLRWDT instruction or SLEEP instruction
        // 0 = A WDT time-out occurred
        ClrWdt(); // Clear/kick watchdog timer

        CONTROLLED_MCU_RESET(code_branch_id_03);
    }

    if ( ((gu8_RCON_REGISTER & BROWN_OUT_RESET) == RESET_OCCURED) && ((gu8_RCON_REGISTER & POWER_ON_RESET) != RESET_OCCURED ))
    {
        /*
        5.4.1 DETECTING BOR
        The BOR bit always resets to "0" on any Brown-out
        Reset or Power-on Reset event. This makes it difficult
        to determine if a Brown-out Reset event has occurred
        just by reading the state of BOR alone. A more reliable
        method is to simultaneously check the state of both
        POR and BOR. This assumes that the POR bit is reset
        to "1" in software immediately after any Power-on Reset
        event. If BOR is "0" while POR is "1", it can be reliably
        assumed that a Brown-out Reset event has occurred.
        */

        // bit 0 BOR: Brown-out Reset Status bit
        // 1 = A Brown-out Reset has not occurred (set by firmware only)
        // 0 = A Brown-out Reset occurred
        // Must be set in software after a Brown-out Reset occurs
        RCONbits.BOR = 1;

        // bit 1 POR: Power-on Reset Status bit
        // 1 = A Power-on Reset has not occurred (set by firmware only)
        // 0 = A Power-on Reset occurred (must be set in software after a Power-on Reset occurs)
        // Note! Set also POR bit here because code will fall into below while(1) loop, and in debugging environment
        // MCU is reset manually via IDE. In that case RCONbits.POR is left to zero, if it was zero before.
        // This leads erronous detection of BOR
        RCONbits.POR = 1;

        CONTROLLED_MCU_RESET(code_branch_id_02);
    }

    if ((gu8_RCON_REGISTER & POWER_ON_RESET) == RESET_OCCURED )
    {
        // bit 1 POR: Power-on Reset Status bit
        // 1 = A Power-on Reset has not occurred (set by firmware only)
        // 0 = A Power-on Reset occurred (must be set in software after a Power-on Reset occurs)
        RCONbits.POR = 1;
    }

    return;
}
#endif

//-----------------------------------------------------------------------------
// example how to use null-pointer traps:

bool b_I2C_Write_Bytes(uint8 u8_address, uint8* pu8_data, uint8 u8_data_length)
{
    ASSERT_NULL_PTR(pu8_data, null_pointer_id_05);

	// "asser null pointer" means "check if the pointer value == 0" (which should never happen)

	// see #define ASSERT_NULL_PTR in example_debug_trap_code.h

//-----------------------------------------------------------------------------
