#include "S826.hpp"


int main(int argc, char **argv)
{
    S826 S;
    // Configure counter0 as PWM
    S.CreatePWM(0, 0, 900, 500);
    // Route counter0 output to dio0
    S.RouteCounterOutput(0, 0, 0);
    S.StartPWM(0, 0);
}


