load_thruna:
	cmake -S . -B build_fw_deploy -DPLATFORM=thruna_firmware -DTARGET=deploy -DHANDLE_DEPS=ON -DFIX_FORMATTING=OFF -DUSE_COMMIT_INFO=MINIMAL

load_quadruna:
	cmake -S . -B build_fw_deploy -DPLATFORM=thruna_firmware -DTARGET=deploy -DHANDLE_DEPS=ON -DFIX_FORMATTING=OFF -DUSE_COMMIT_INFO=MINIMAL

load_dimos_dev:
	cmake -S . -B build_dimos_dev -G Ninja -DPLATFORM=dimos -DHANDLE_DEPS=ON -DFIX_FORMATTING=OFF -DTARGET=dev -DUSE_COMMIT_INFO=MINIMAL

load_dimos:
	cmake -S . -B build_dimos -G Ninja -DPLATFORM=dimos -DHANDLE_DEPS=ON -DFIX_FORMATTING=OFF -DTARGET=deploy -DUSE_COMMIT_INFO=MINIMAL