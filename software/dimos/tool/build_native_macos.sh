#!/usr/bin/env bash
# Xcode "Build dimos_jsoncan" phase (Runner target): builds native/ with CMake and
# copies libdimos_jsoncan.dylib into the app's Frameworks/, where jsoncan_ffi.dart loads it.
set -euo pipefail

# Xcode runs build phases with a minimal PATH.
export PATH="/opt/homebrew/bin:/usr/local/bin:$HOME/.cargo/bin:$PATH"

DIMOS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$DIMOS_DIR/build/native-macos/$CONFIGURATION"

cmake -S "$DIMOS_DIR/native" -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES="${ARCHS// /;}" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET="$MACOSX_DEPLOYMENT_TARGET"
cmake --build "$BUILD_DIR"

FRAMEWORKS_DIR="$TARGET_BUILD_DIR/$FRAMEWORKS_FOLDER_PATH"
mkdir -p "$FRAMEWORKS_DIR"
cp -f "$BUILD_DIR/libdimos_jsoncan.dylib" "$FRAMEWORKS_DIR/"
codesign --force --sign "${EXPANDED_CODE_SIGN_IDENTITY:--}" "$FRAMEWORKS_DIR/libdimos_jsoncan.dylib"
