import csv
import os

from code_generation.jsoncan.src.json_parsing.json_can_parsing import \
    JsonCanParser
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

# Define the InfluxDB connection
bucket = "bucket"
org = "org"
token = "H_csyksUJJion_wiM9HCop4wX4vGEs2mgRQylaBOoKY_cYOgJRCVKZfPtI0TCL0sV0kWkKIitIOq51iEE_6tSg=="
url = "http://localhost:8086"
csv_file_path = 'scripts\data.csv'
write_client = InfluxDBClient(url=url, token=token, org=org)
write_api = write_client.write_api(write_options=SYNCHRONOUS)

# Define the path to the JSON CAN 
ROOT_PATH = "can_bus/quadruna/"
megs = JsonCanParser(can_data_dir=ROOT_PATH).make_database().msgs
id_to_meg = {}

for value in range(5):
  point = (
    Point("measurement1")
    .tag("tagname1", "tagvalue1")
    .field("field1", value)
    .field("field2", value * 2)
  )
  write_api.write(bucket=bucket, org="org", record=point)


# build the map from id to message
for meg in megs:
    id_to_meg[meg.id] = meg

def get_byte_array_from_row(row): 
    byte_array = []
    for i in range(0, 8):
        byte_array.append(int(row[f"Data_{i}"]))
    return bytes(byte_array)

def get_signal_value(byte_array, start_bit, bit_width):
    # Initialize variables
    value = 0    
    # Iterate over the bits to be extracted
    for i in range(bit_width):
        current_byte_index = (start_bit + i) // 8
        current_bit_index = (start_bit + i) % 8
        
        # get value bit by bit
        bit = (byte_array[current_byte_index] >> current_bit_index) & 1
        value |= (bit << i)
    
    return value
        
points = []
with open(csv_file_path, mode='r') as file:
    csv_reader = csv.DictReader(file)
    
    # Process each row in the CSV
    for line, row in enumerate(csv_reader):
        timestamp = row["Timestamp"]
        data_id = int(row["ID"])
        try:
            meg = id_to_meg[data_id]
        except KeyError:
            print(f"ID {data_id} not found on line {line}")
            continue
        byte_array = get_byte_array_from_row(row)
        point = Point(meg.name).tag("id", data_id)
        for signal in meg.signals:
            signal_name = signal.name
            signal_value = get_signal_value(byte_array, signal.start_bit, signal.bits)
            point = point.field(signal_name, signal_value)
        points.append(point)
        # write_api.write(bucket=bucket, org="org", record=point)
        
        # Write the Point to InfluxDB
# close the client

for line, point in enumerate(points):
    try:
        write_api.write(bucket=bucket, org="org", record=point)
        if(line % 100 == 0):
            print(f"Written {line} points")
        if(line == 1000):
            break
            
    except Exception as e:
        print(f"Error writing point: {e}")
        breakpoint()
write_client.close()

 