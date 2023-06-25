.syntax unified
.cpu cortex-m7
.fpu softvfp
.thumb

.global irq_table

.word _sidata
.word _sdata
.word _edata
.word _sbss
.word _ebss

  .section .text.resetHandler
  .weak resetHandler
  .type resetHandler, %function
resetHandler:
  ldr   r0, =_estack
  mov   sp, r0          // set stack pointer.

  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit

  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

  // NO STATIC INITIALIZATION!

  // Call main.
  bl main

LoopForever:
    b LoopForever

  .size resetHandler, .-resetHandler

  .section .isr_vector,"a",%progbits
  .type irq_table, %object
  .size irq_table, .-irq_table

irq_table:
  .word _estack
  .word resetHandler

// offset = 0x008.
  .word nmiHandler
  .word hardFaultHandler
  .word	memManageHandler
  .word	busFaultHandler
  .word	usageFaultHandler
  .word	0
  .word	0
  .word	0
  .word	0
  .word	svcHandler
  .word	debugMonHandler
  .word	0
  .word	pendSvHandler
  .word	sysTickHandler

// offset = 0x040.
  .word	unhandledInterrupt040                         // Window Watchdog interrupt.
  .word	unhandledInterrupt044                         // PVD through EXTI line.
  .word	unhandledInterrupt048                         // RTC tamper, timestamp.
  .word	unhandledInterrupt04C                         // RTC Wakeup interrupt.
  .word	unhandledInterrupt050                         // Flash memory.
  .word	unhandledInterrupt054                         // RCC global interrupt.
  .word	unhandledInterrupt058                         // EXTI Line 0 interrupt.
  .word	unhandledInterrupt05C                         // EXTI Line 1 interrupt.
  .word	unhandledInterrupt060                         // EXTI Line 2 interrupt.
  .word	unhandledInterrupt064                         // EXTI Line 3interrupt.
  .word	unhandledInterrupt068                         // EXTI Line 4interrupt.
  .word	unhandledInterrupt06C                         // DMA1 Stream0.
  .word	unhandledInterrupt070                         // DMA1 Stream1.
  .word	unhandledInterrupt074                         // DMA1 Stream2.
  .word	unhandledInterrupt078                         // DMA1 Stream3.
  .word	unhandledInterrupt07C                         // DMA1 Stream4.
  .word	unhandledInterrupt080                         // DMA1 Stream5.
  .word	unhandledInterrupt084                         // DMA1 Stream6.
  .word	unhandledInterrupt088                         // ADC1 and ADC2.
  .word	unhandledInterrupt08C                         // FDCAN1 Interrupt 0.
  .word	unhandledInterrupt090                         // FDCAN2 Interrupt 0.
  .word	unhandledInterrupt094                         // FDCAN1 Interrupt 1.
  .word	unhandledInterrupt098                         // FDCAN2 Interrupt 1.
  .word	unhandledInterrupt09C                         // EXTI Line[9:5] interrupts.
  .word	unhandledInterrupt0A0                         // TIM1 break interrupt.
  .word	unhandledInterrupt0A4                         // TIM1 update interrupt.
  .word	unhandledInterrupt0A8                         // TIM1 trigger and commutation.
  .word	unhandledInterrupt0AC                         // TIM1 capture / compare.
  .word	unhandledInterrupt0B0                         // TIM2 global interrupt.
  .word	unhandledInterrupt0B4                         // TIM3 global interrupt.
  .word	unhandledInterrupt0B8                         // TIM4 global interrupt.
  .word	unhandledInterrupt0BC                         // I2C1 event interrupt.
  .word	unhandledInterrupt0C0                         // I2C1 global error interrupt.
  .word	unhandledInterrupt0C4                         // I2C2 event interrupt.
  .word	unhandledInterrupt0C8                         // I2C2 global error interrupt.
  .word	unhandledInterrupt0CC                         // SPI1 global interrupt.
  .word	unhandledInterrupt0D0                         // SPI2 global interrupt.
  .word	unhandledInterrupt0D4                         // USART1 global interrupt.
  .word	unhandledInterrupt0D8                         // USART2 global interrupt.
  .word	unhandledInterrupt0DC                         // USART3 global interrupt.
  .word	unhandledInterrupt0E0                         // EXTI Line[15:10] interrupts.
  .word	unhandledInterrupt0E4                         // RTC alarms (A and B).
  .word	0
  .word	unhandledInterrupt0EC                         // TIM8 and 12 break global.
  .word	unhandledInterrupt0F0                         // TIM8 and 13 update global.
  .word	unhandledInterrupt0F4                         // TIM8 and 14 trigger /commutation and global.
  .word	unhandledInterrupt0F8                         // TIM8 capture / compare.
  .word	unhandledInterrupt0FC                         // DMA1 Stream7.

// offset = 0x100.
  .word	unhandledInterrupt100                         // FMC global interrupt.
  .word	unhandledInterrupt104                         // SDMMC1 global interrupt.
  .word	unhandledInterrupt108                         // TIM5 global interrupt.
  .word	unhandledInterrupt10C                         // SPI3 global interrupt.
  .word	unhandledInterrupt110                         // UART4 global interrupt.
  .word	unhandledInterrupt114                         // UART5 global interrupt.
  .word	unhandledInterrupt118                         // TIM6 global interrupt.
  .word	unhandledInterrupt11C                         // TIM7 global interrupt.
  .word	unhandledInterrupt120                         // DMA2 Stream0 interrupt.
  .word	unhandledInterrupt124                         // DMA2 Stream1 interrupt.
  .word	unhandledInterrupt128                         // DMA2 Stream2 interrupt.
  .word	unhandledInterrupt12C                         // DMA2 Stream3 interrupt.
  .word	unhandledInterrupt130                         // DMA2 Stream4 interrupt.
  .word	unhandledInterrupt134                         // Ethernet global interrupt.
  .word	unhandledInterrupt138                         // Ethernet wakeup through EXTI.
  .word	unhandledInterrupt13C                         // CAN2TX interrupts.
  .word	0
  .word	0
  .word	0
  .word	0
  .word	unhandledInterrupt150                         // DMA2 Stream5 interrupt.
  .word	unhandledInterrupt154                         // DMA2 Stream6 interrupt.
  .word	unhandledInterrupt158                         // DMA2 Stream7 interrupt.
  .word	unhandledInterrupt15C                         // USART6 global interrupt.
  .word	unhandledInterrupt160                         // I2C3 event interrupt.
  .word	unhandledInterrupt164                         // I2C3 error interrupt.
  .word	handledUsbInterruptOut                        // USB OTG HS Out.
  .word	handledUsbInterruptIn                         // USB OTG HS In.
  .word	handledUsbInterruptWakeup                     // USB OTG HS Wakeup.
  .word	handledUsbInterruptGeneral                    // USB OTG HS IRQ.
  .word	unhandledInterrupt178                         // DCMI global interrupt.
  .word	unhandledInterrupt17C                         // Crypto global interrupt.
  .word	0
  .word	unhandledInterrupt184                         // Floating point unit interrupt.
  .word	unhandledInterrupt188                         // UART7 global interrupt.
  .word	unhandledInterrupt18C                         // UART8 global interrupt.
  .word	unhandledInterrupt190                         // SPI4 global interrupt.
  .word	unhandledInterrupt194                         // SPI5 global interrupt.
  .word	unhandledInterrupt198                         // SPI6 global interrupt.
  .word	unhandledInterrupt19C                         // SAI1 global interrupt.
  .word	unhandledInterrupt1A0                         // LCD-TFT global interrupt.
  .word	unhandledInterrupt1A4                         // LCD-TFT error interrupt.
  .word	unhandledInterrupt1A8                         // DMA2D global interrupt.
  .word	0
  .word	unhandledInterrupt1B0                         // OCTOSPI1 global interrupt.
  .word	unhandledInterrupt1B4                         // LPTIM1 global interrupt.
  .word	unhandledInterrupt1B8                         // HDMI-CEC global interrupt.
  .word	handledI2c4                                   // I2C4 event interrupt.
  .word	handledI2c4                                   // I2C4 error interrupt.
  .word	unhandledInterrupt1C4                         // SPDIFRX global interrupt.
  .word	0
  .word	0
  .word	0
  .word	0
  .word	unhandledInterrupt1D8                         // DMAMUX1 overrun interrupt.
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	unhandledInterrupt1F8                         // DFSDM1 filter 0 interrupt.
  .word	unhandledInterrupt1FC                         // DFSDM1 filter 1 interrupt.

// offset = 0x200.
  .word	unhandledInterrupt200                         // DFSDM1 filter 2 interrupt.
  .word	unhandledInterrupt204                         // DFSDM1 filter 3 interrupt.
  .word	0
  .word	unhandledInterrupt20C                         // SWPMI global interrupt.
  .word	unhandledInterrupt210                         // TIM15 global interrupt.
  .word	unhandledInterrupt214                         // TIM16 global interrupt.
  .word	unhandledInterrupt218                         // TIM17 global interrupt.
  .word	unhandledInterrupt21C                         // MDIOS wakeup.
  .word	unhandledInterrupt220                         // MDIOS global interrupt.
  .word	0
  .word	unhandledInterrupt228                         // MDMA Global interrupt.
  .word	0
  .word	unhandledInterrupt230                         // SDMMC2 global interrupt.
  .word	unhandledInterrupt234                         // HSEM global interrupt 1.
  .word	0
  .word	unhandledInterrupt23C                         // ADC3 global interrupt.
  .word	unhandledInterrupt240                         // DMAMUX D3 overrun interrupt.
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	unhandledInterrupt264                         // COMP1 and COMP2 global interrupt.
  .word	unhandledInterrupt268                         // LPTIM2 timer interrupt.
  .word	unhandledInterrupt26C                         // LPTIM2 timer interrupt.
  .word	unhandledInterrupt260                         // LPTIM2 timer interrupt.
  .word	unhandledInterrupt274                         // LPTIM2 timer interrupt.
  .word	unhandledInterrupt278                         // LPUART global interrupt.
  .word	0
  .word	unhandledInterrupt280                         // Clock Recovery System global interrupt.
  .word	0
  .word	unhandledInterrupt288                         // SAI4 global interrupt.
  .word	0
  .word	0
  .word	unhandledInterrupt294                         // WKUP1 to WKUP6 pins.
  .word	unhandledInterrupt298                         // OCTOSPI2 global interrupt.

/************************ (C) COPYRIGHT STMicroelectonics *****END OF FILE****/
