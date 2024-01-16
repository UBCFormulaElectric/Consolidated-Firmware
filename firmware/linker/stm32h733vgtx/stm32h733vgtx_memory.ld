MEMORY
{
  ram  (xrw)    : ORIGIN = 0x24000000,   LENGTH = 320K
  boot_code    (rx)    : ORIGIN = 0x8000000,   LENGTH = 128K
  app_metadata    (rx)    : ORIGIN = 0x8020000,   LENGTH = 1K
  app_code     (rx)    : ORIGIN = 0x8020400,  LENGTH = 895K
}

__boot_code_start__ = ORIGIN(boot_code);
__boot_code_size__ = LENGTH(boot_code);
__app_metadata_start__ = ORIGIN(app_metadata);
__app_metadata_size__  = LENGTH(app_metadata);
__app_code_start__ = ORIGIN(app_code);
__app_code_size__  = LENGTH(app_code);