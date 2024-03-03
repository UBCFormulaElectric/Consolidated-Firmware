$rootDir = Resolve-Path ../../
mkdir "$rootDir\validationtools\validationtools\chimera\proto_libs" -ErrorAction SilentlyContinue
Get-ChildItem "$rootDir\firmware\chimera\proto" -Filter "*.proto" |
Foreach-Object {
    protoc `
        -I "$rootDir\firmware\chimera\proto" `
        --python_out "$rootDir\validationtools\validationtools\chimera\proto_libs" $_.FullName

    protol `
    --in-place `
    --python-out "$rootDir\validationtools\validationtools\chimera\proto_libs" `
    protoc --proto-path="$rootDir\firmware\chimera\proto" $_.FullName
}