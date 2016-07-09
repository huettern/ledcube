//
// main.c
//

#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <assert.h>


#include "freedom.h"
#include "common.h"

#include "ledcube.h"

extern char *_sbrk(int len);

char linebuf[100];
uint8_t linebufptr = 0;


char** str_split(char* a_str, const char a_delim);
void parseLine();

void print_bin(uint32_t n);

// Main program
int main(void)
{
    uint8_t ctr8 = 0;
    uint32_t ctr32 = 0;
    char i;
    char *heap_end;
    
    // FTFA_FOPT |= (1<<2);

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
    //cube_test();

    // test
    // tuColor cl;
    // cl.rgb.r = 255;
    // cl.rgb.g = 0x0f;
    // cl.rgb.b = 1;

    // iprintf("Color r: %d\r\n", cl.rgb.r);
    // iprintf("Color g: %d\r\n", cl.rgb.g);
    // iprintf("Color b: %d\r\n", cl.rgb.b);
    // iprintf("Color all: %d\r\n", cl.all);


    cube_set_z(0x01);
    cube_output_enable(0xff);
    while(1) {
        cube_write_driver(ctr32);
        print_bin(ctr32);
        ctr32 <<= 1;
        if(ctr32 == 0) {
            ctr32 = 1;
            ctr8 <<= 1;
            if(ctr8 == 0x08) ctr8= 1;
            cube_set_z(ctr8);
        }
        delay(500);
    }
    
    iprintf("cube_run ");
    while(1) {
        cube_run();
    }

    iprintf("ledcube> ");
    for(;;) {

        i = getchar();
        if(i == 13) {
            iprintf("\r\n");
            parseLine();
            iprintf("ledcube> ");
        } else {
            linebuf[linebufptr++] = i;
            putchar(i);
        }
        cube_dbg();
    }
}


void parseLine() {
    uint8_t i = 0;
    uint8_t num;
    // char* cmd = strchr(linebuf, ' ');
    // char* data = strncpy(data, linebuf + cmd+1, 20);
    // int idx = cmd - linebuf[0];
    if(linebufptr == 0) {   
        return;
    }
    char** splt = str_split(linebuf,' ');
    if(strcmp(splt[0], "spi") == 0) {
        num = atoi(splt[1]);
        iprintf("spi: %d\r\n\r\n", num);
        cube_spi_write(num);
    }

    if(strcmp(splt[0], "z") == 0) {
        num = atoi(splt[1]);
        iprintf("z: %d\r\n\r\n", num);
        cube_set_z(1<<num);
    }

    linebufptr = 0;
    for(; i < 100; i++)
        linebuf[i] = 0;
}


char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            //assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void print_bin(uint32_t n) {
    uint8_t ctr = 0;
    for(;ctr < 32; ctr++) {
        if(n & (1<<(31-ctr))) {
            iprintf("1");
        } else {
            iprintf("0");
        }
    }
    iprintf("\r\n");
}

void NMI_Handler() {
    iprintf("NMI_Handler\r\n");
}
// void HardFault_Handler() {
//     iprintf("HardFault_Handler\r\n");
// }
// void SVC_Handler() {
//     iprintf("SVC_Handler\r\n");
// }
void PendSV_Handler() {
    iprintf("PendSV_Handler\r\n");
}
void SysTick_Handler() {
    iprintf("SysTick_Handler\r\n");
}

void DMA0_IRQHandler() {
    iprintf(" DMA0_IRQHandler\r\n");
}
void DMA1_IRQHandler() {
    iprintf(" DMA1_IRQHandler\r\n");
}
void DMA2_IRQHandler() {
    iprintf(" DMA2_IRQHandler\r\n");
}
void DMA3_IRQHandler() {
    iprintf(" DMA3_IRQHandler\r\n");
}
void MCM_IRQHandler() {
    iprintf(" MCM_IRQHandler\r\n");
}
void FTFL_IRQHandler() {
    iprintf(" FTFL_IRQHandler\r\n");
}
void PMC_IRQHandler() {
    iprintf(" PMC_IRQHandler\r\n");
}
void LLW_IRQHandler() {
    iprintf(" LLW_IRQHandler\r\n");
}
void I2C0_IRQHandler() {
    iprintf(" I2C0_IRQHandler\r\n");
}
void I2C1_IRQHandler() {
    iprintf(" I2C1_IRQHandler\r\n");
}
void SPI0_IRQHandler() {
    iprintf(" SPI0_IRQHandler\r\n");
}
void SPI1_IRQHandler() {
    iprintf(" SPI1_IRQHandler\r\n");
}
void UART1_IRQHandler() {
    iprintf(" UART1_IRQHandler\r\n");
}
void UART2_IRQHandler() {
    iprintf(" UART2_IRQHandler\r\n");
}
void ADC0_IRQHandler() {
    iprintf(" ADC0_IRQHandler\r\n");
}
void CMP0_IRQHandler() {
    iprintf(" CMP0_IRQHandler\r\n");
}
void FTM0_IRQHandler() {
    iprintf(" FTM0_IRQHandler\r\n");
}
void FTM2_IRQHandler() {
    iprintf(" FTM2_IRQHandler\r\n");
}
void RTC_Alarm_IRQHandler() {
    iprintf(" RTC_Alarm_IRQHandler\r\n");
}
void RTC_Seconds_IRQHandler() {
    iprintf(" RTC_Seconds_IRQHandler\r\n");
}
void PIT_IRQHandler() {
    iprintf(" PIT_IRQHandler\r\n");
}
void USBOTG_IRQHandler() {
    iprintf(" USBOTG_IRQHandler\r\n");
}
void DAC0_IRQHandler() {
    iprintf(" DAC0_IRQHandler\r\n");
}
void TSI0_IRQHandler() {
    iprintf(" TSI0_IRQHandler\r\n");
}
void MCG_IRQHandler() {
    iprintf(" MCG_IRQHandler\r\n");
}
void LPTimer_IRQHandler() {
    iprintf(" LPTimer_IRQHandler\r\n");
}
void PORTA_IRQHandler() {
    iprintf(" PORTA_IRQHandler\r\n");
}
void PORTD_IRQHandle() {
    iprintf(" PORTD_IRQHandle\r\n");
}