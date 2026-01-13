MEMORY
{
  rtcram (xrw) : ORIGIN = 0x20000000,   LENGTH = 128K
  ram (xrw) : ORIGIN = 0x20020000,   LENGTH = 0x7FFE0
  boot_request_ram (xrw) : ORIGIN = 0x2009FFF0, LENGTH = 0x10
  boot_code (rx) : ORIGIN = 0x8000000,    LENGTH = 128K
  app_metadata (rx) : ORIGIN = 0x8020000, LENGTH = 1K
  app_code (rx)  : ORIGIN = 0x8020400,   LENGTH = 1919K
}

SECTIONS
{
  /* SEGGER RTT buffer placement */
  .rtt (NOLOAD) :
  {
    KEEP(*(.rtt))
  } > rtcram

  /* Boot request section in RAM */
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
