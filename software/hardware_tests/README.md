# Python Scripts to Test the Modems for Telemetry

Each file should describe the action on each side of the two computers for the set up. There is no virtual env so you do have to install pyserial on your computer.

The default port is for linux users, however to change it.


## For Proto Testing

you need to run a command in order to generate the proto_pb2.py file. That comamnd is done via command line (for now) and is 

protoc -I=. --python_out=. FILE.proto

where FILE is the name of the proto file (in the current case that will be simple)