E:\environment-setup-armv8a-poky-linux.bat

& "E:\sysroots\x86_64-w64-mingw32\usr\bin\cmake.exe" -G Ninja -DPLATFORM=dimos -DHANDLE_DEPS=ON -DFIX_FORMATTING=OFF -DTARGET=dev -DUSE_COMMIT_INFO=MINIMAL -S . -B ./build_dimos_deploy