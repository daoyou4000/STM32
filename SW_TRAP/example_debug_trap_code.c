// Example code about debug traps, blinking of the Id of the trap, or reset MCU

// In some .c file [in other project it was StressTest.c and StressTest.h]:
bool gb_Time_To_Reset = FALSE;

#ifdef DEBUG_TRAPS_ENABLED			// if defined, then enable debug code

// These defines blinking rate. About 100ms ON, 300ms OFF, 2.5 sec pause
#define DELAY_BLINK_ON          196605      // (3 * 0xFFFF)
#define DELAY_BLINK_OFF         393210      // (6 * 0xFFFF)
#define DELAY_BLINK_PAUSE       2424795     // (25 * 0xFFFF)

// This function, when called, blinks RED LED sharply based on the given argument
// which is Id for 'illegal code branch'
// 'Blinks sharply' means ~100ms ON, 300ms OFF, 2.5 sec pause after number of blinks
// This is repeated forever. So once trap is activated, then 'SW is stopped' and just
// blinks all the time. With this blinking rate it is possible to count number of blinks
// up to ~30 blinks, two times in row. Then cycle Power OFF-ON to let SW start to run again.
void DEBUG_TRAP_CODE_BLINK(illegal_code_branch_id_enum u8NumberOfBlinks)
{
    uint32 u32Delay = 0;
    uint8 u8NumberOfBlinksCpy;

    DisableInterrupts();
    WDTCONbits.SWDTEN = 0; // disable watchdog

    LED_RED_OFF;
    LED_YELLOW_OFF;
    LED_GREEN_OFF;

    while (1) //lint !e716 // stay here until power off
    {
        u8NumberOfBlinksCpy = (uint8) u8NumberOfBlinks;
        while (u8NumberOfBlinksCpy--)
        {
            LED_RED_ON;
            u32Delay = DELAY_BLINK_ON;
            while (u32Delay--)
            {
            }

            LED_RED_OFF;
            u32Delay = DELAY_BLINK_OFF;
            while (u32Delay--)
            {
            }
        }

        u32Delay = DELAY_BLINK_PAUSE;
        while (u32Delay--)
        {
        }
    }

}

#else

// placement for normal mode code, if any

#endif

