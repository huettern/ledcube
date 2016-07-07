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
    //cube_init();

    // test cube
    //cube_test();

    for(;;) {
        iprintf("ledcube> ");
        getchar();
        iprintf("\r\n");
    }
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
void FTM1_IRQHandler() {
    iprintf(" FTM1_IRQHandler\r\n");
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