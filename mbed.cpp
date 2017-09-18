//mbed SPI Slave
//Conection Master <-> Slave
//MOSI(D11) <-> MOSI(D11)
//MISO(D12) <-> MISO(D12)
//SCK(D13) <-> SCK(D13)
//A0(A0) <-> A3(A3)

#include "mbed.h"

DigitalOut interruptPin(D8);
SPISlave spi(D11, D12, D13, A3); //MOSI MISO SCLK SSEL

int main()
{
    char i = 0;

    //SPI Configuration
    spi.format(8, 3);
    spi.frequency(1000000);

    //Main Program
    while(1) {
        interruptPin = 0;
        wait(0.1);
        interruptPin = 1;
        //SPI Communication(Slave)
        if(spi.receive()) {
            spi.reply(i);
            i++;
        }
        wait(0.1);
    }
}
