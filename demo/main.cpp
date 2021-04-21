#include "mbed.h"
#include "mbed_rpc.h"

/**
 *  This example program has been updated to use the RPC implementation in the new mbed libraries.
 *  This example demonstrates using RPC over serial
**/
RpcDigitalOut myled1(LED1,"myled1");
RpcDigitalOut myled3(LED3,"myled3");
BufferedSerial pc(USBTX, USBRX);
void LEDControl(Arguments *in, Reply *out);
RPCFunction rpcLED(&LEDControl, "LEDControl");
int x, y;

int main() {
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class

    // receive commands, and send back the responses
    char buf[256], outbuf[256];

    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");

    while(1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = fgetc(devin);
            if (recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);
    }
}

// Make sure the method takes in Arguments and Reply objects.
void LEDControl (Arguments *in, Reply *out)   {
    bool on1 = 0, on2 = 0;

    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<int>();
    y = in->getArg<int>();

    // Have code here to call another RPC function to wake up specific led or close it.
    char buffer[200], outbuf[256];
    char strings1[20], strings2[20];
    int i;
    for (i = 0; i < 10; i++) {
        if (x && !y) { 
            on1 = !on1;
            sprintf(strings1, "/myled1/write %d", on1);
            strcpy(buffer, strings1);
            RPC::call(buffer, outbuf); 
            out->putData(buffer);
        } else if (!x && y){
            on2 = !on2;
            sprintf(strings2, "/myled3/write %d", on2);
            strcpy(buffer, strings2);
            RPC::call(buffer, outbuf); 
            out->putData(buffer);
            
        }
        ThisThread::sleep_for(500ms);
    }
}/*
void LEDControl (Arguments *in, Reply *out)   {
    bool success = true;

    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<double>();
    y = in->getArg<double>();

    // Have code here to call another RPC function to wake up specific led or close it.
    char buffer[200], outbuf[256];
    char strings[20];
    int led = x;
    int on = y;
    sprintf(strings, "/myled%d/write %d", led, on);
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    if (success) {
        out->putData(buffer);
    } else {
        out->putData("Failed to execute LED control.");
    }
}*/