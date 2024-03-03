$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Get-ChildItem $scriptDir"\proto" -Filter "*.proto" |
Foreach-Object {
    protoc -I proto --python_out ../../firmware/chimera/proto_libs $_.FullName

    protol `
    --in-place `
    --python-out ../../firmware/chimera/proto_libs `
    protoc --proto-path=proto $_.FullName
}