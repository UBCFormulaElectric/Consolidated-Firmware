MEMORY
{
  DTCMRAM (xrw)    : ORIGIN = 0x20000000,   LENGTH = 128K
  ram           (xrw) : ORIGIN = 0x24000000, LENGTH = 0x4FFF0
  boot_request_ram (xrw) : ORIGIN = 0x2404FFF0, LENGTH = 0x10
  boot_code     (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
  app_metadata  (rx)  : ORIGIN = 0x08020000, LENGTH = 1K
  app_code      (rx)  : ORIGIN = 0x08020400, LENGTH = 895K
}

SECTIONS
{
  .boot_request (NOLOAD) :
  {
    KEEP(*(.boot_request))
  } > boot_request_ram
}

__boot_code_start__    = ORIGIN(boot_code);
__boot_code_size__     = LENGTH(boot_code);
__app_metadata_start__ = ORIGIN(app_metadata);
__app_metadata_size__  = LENGTH(app_metadata);
__app_code_start__     = ORIGIN(app_code);
__app_code_size__      = LENGTH(app_code);
