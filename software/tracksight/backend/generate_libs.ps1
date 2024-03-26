$rootDir = Resolve-Path "$PSScriptRoot/../../../"
mkdir "$rootDir\software\tracksight\backend\util_scripts" -ErrorAction SilentlyContinue
Get-ChildItem "$rootDir\firmware\quadruna\VC\proto" -Filter "*.proto" |
Foreach-Object {
    protoc `
        -I "$rootDir\firmware\quadruna\VC\proto" `
        --python_out "$rootDir\software\tracksight\backend\util_scripts" $_.FullName

    protol `
    --in-place `
    --python-out "$rootDir\software\tracksight\backend\util_scripts" `
    protoc --proto-path="$rootDir\firmware\quadruna\VC\proto" $_.FullName
}