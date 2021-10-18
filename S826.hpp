#include "826api.h"

class S826 {
    protected: 
        uint board;
        int errcode;
        int boardflags;

    public:
        S826();
        ~S826();
        int CreatePWM(uint board, uint ctr, uint ontime, uint offtime); // Configure a PWM generator, arguments in microseconds 
        int StartPWM(uint board, uint ctr); 
        int SetPWM(uint board, uint ctr, uint ontime, uint offtime);
        int RouteCounterOutput(uint board, uint ctr, uint dio); //route a counter's ExtOut signal to a DIO pin
        void PrintError();
};
