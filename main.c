//
// main.c
//

#include <stdio.h>
#include "freedom.h"
#include "common.h"

#include "ledcube.h"

extern char *_sbrk(int len);

// Main program
int main(void)
{
    char i;
    char *heap_end;
    
    // Initialize all modules
    uart_init(115200);
    setvbuf(stdin, NULL, _IONBF, 0);        // No buffering

    // Run tests
    tests();

    // Blink the green LED to indicate booting
    unsigned int pattern = 0b1100110011001100;
    while(pattern) {
        RGB_LED(0, 0, pattern & 1 ? 100 : 0);           // Set GREEN led based on LSB
        pattern >>= 1;
        delay(25);
    }

    // Welcome banner
    iprintf("\r\n\r\n====== LED CUBE ======\r\n");
    iprintf("Built: %s %s\r\n\r\n", __DATE__, __TIME__);
    heap_end = _sbrk(0);
    iprintf("Reset code: 0x%02x 0x%02x\r\n", RCM_SRS0, RCM_SRS1);
    iprintf("Heap:  %p to %p (%d bytes used)\r\n", __heap_start, heap_end, 
                heap_end - (char *)__heap_start);
    iprintf("Stack: %p to %p (%d bytes used)\r\n", &i, __StackTop, 
                (char *)__StackTop - &i);
    iprintf("%d bytes free\r\n\r\n\r\n", &i - heap_end);


    // init led cube
    cube_init();

    // test cube
    cube_test();

    for(;;) {
        iprintf("ledcube> ");
        getchar();
        iprintf("\r\n");
    }
}
