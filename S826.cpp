#include "S826.hpp"
#include <iostream>

S826::S826() {
    board      = 0;                        // change this if you want to use other than board number 0
    errcode     = S826_ERR_OK;  
    boardflags  = S826_SystemOpen();        // open 826 driver and find all 826 boards
    if (boardflags < 0)
        errcode = boardflags;                       // problem during open
    else if ((boardflags & (1 << board)) == 0) {
        int i;
        printf("TARGET BOARD of index %d NOT FOUND\n",board);         // driver didn't find board you want to use
        for (i = 0; i < 8; i++) {
            if (boardflags & (1 << i)) {
                printf("board %d detected. try \"./s826demo %d\"\n", i, i);
            }
        }
    }
    PrintError();
};

int S826::CreatePWM(uint board, uint ctr, uint ontime, uint offtime)
{
    S826_CounterModeWrite(board, ctr, // Configure counter for PWM:
    S826_CM_K_1MHZ | // clock = internal 1 MHz
    S826_CM_UD_REVERSE | // count down
    S826_CM_PX_START | S826_CM_PX_ZERO | // preload @startup and counts==0
    S826_CM_BP_BOTH | // use both preloads (toggle)
    S826_CM_OM_PRELOAD); // assert ExtOut during preload0 interval
    SetPWM(board, ctr, ontime, offtime); // Program initial on/off times.
};

int S826::StartPWM(uint board, uint ctr)
{
    return S826_CounterStateWrite(board, ctr, 1); // Start the PWM generator.
}

int S826::SetPWM(uint board, uint ctr, uint ontime, uint offtime)
{
    S826_CounterPreloadWrite(board, ctr, 0, ontime); // On time in us.
    S826_CounterPreloadWrite(board, ctr, 1, offtime); // Off time in us.
};

//route a counter's ExtOut signal to a DIO pin
int S826::RouteCounterOutput(uint board, uint ctr, uint dio)
{
    uint data[2]; // dio routing mask
    if ((dio >= S826_NUM_DIO) || (ctr >= S826_NUM_COUNT))
    return S826_ERR_VALUE; // bad channel number
    if ((dio & 7) != ctr)
    return S826_ERR_VALUE; // counter output can't be routed to dio
    // Route counter output to DIO pin:
    S826_SafeWrenWrite(board, S826_SAFEN_SWE); // Enable writes to DIO signal router.
    S826_DioOutputSourceRead(board, data); // Route counter output to DIO
    data[dio > 23] |= (1 << (dio % 24)); // without altering other routes.
    S826_DioOutputSourceWrite(board, data);
    return S826_SafeWrenWrite(board, S826_SAFEN_SWD); // Disable writes to DIO signal router.
};

void S826::PrintError(){ //return error code 
    switch (errcode)
    {
        case S826_ERR_OK:           break;
        case S826_ERR_BOARD:        printf("Illegal board number"); break;
        case S826_ERR_VALUE:        printf("Illegal argument"); break;
        case S826_ERR_NOTREADY:     printf("Device not ready or timeout"); break;
        case S826_ERR_CANCELLED:    printf("Wait cancelled"); break;
        case S826_ERR_DRIVER:       printf("Driver call failed"); break;
        case S826_ERR_MISSEDTRIG:   printf("Missed adc trigger"); break;
        case S826_ERR_DUPADDR:      printf("Two boards have same number"); break;S826_SafeWrenWrite(board, 0x02);
        case S826_ERR_BOARDCLOSED:  printf("Board not open"); break;
        case S826_ERR_CREATEMUTEX:  printf("Can't create mutex"); break;
        case S826_ERR_MEMORYMAP:    printf("Can't map board"); break;
        default:                    printf("Unknown error"); break;
    }
};

S826::~S826(){
    S826_SystemClose();
};
