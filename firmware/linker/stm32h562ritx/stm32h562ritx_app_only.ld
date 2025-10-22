/* Entry Point */
ENTRY(Reset_Handler)

/* Define stack pointer at the top of SRAM3 */
_estack = ORIGIN(SRAM3) + LENGTH(SRAM3);
/* Define minimal heap and stack size */
_Min_Heap_Size = 0x0;    /* required amount of heap */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Define memory regions based on STM32H562 */
MEMORY
{
  FLASH     (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
  SRAM1_2   (xrw) : ORIGIN = 0x20000000, LENGTH = 256K   /* SRAM1 + SRAM2 */
  SRAM3     (xrw) : ORIGIN = 0x30000000, LENGTH = 320K
  BACKUPRAM (xrw) : ORIGIN = 0x38800000, LENGTH = 4K 
}

/* Output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* Define the end of all FLASH sections */
  _flash_end = .;

  /* used by the startup to initialize data */
  _sidata = _flash_end;

  /* Initialized data section (VMA in SRAM, LMA in FLASH) */
  .data : AT (_flash_end)
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    *(.RamFunc)
    *(.RamFunc*)
    . = ALIGN(4);
    _edata = .;
  } >SRAM1_2

  /* Uninitialized data */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >SRAM1_2

  /* Heap and stack */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE (end = .);
    PROVIDE (_end = .);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >SRAM1_2

  /* Backup RAM */
  .backup_bss (NOLOAD) :
  {
    *(.bkup)
  } >BACKUPRAM

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
