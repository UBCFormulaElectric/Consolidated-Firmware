from code_generation.jsoncan.src.json_parsing.json_can_parsing import JsonCanParser
import os
import csv
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "your_bucket_name"
org = "your_organization"
token = "your_token"
url = "http://localhost:8086"
csv_file_path = 'data.csv'
ROOT_PATH = "can_bus/quadruna/"

megs = JsonCanParser(can_data_dir=ROOT_PATH).make_database().msgs
id_to_meg = {}
client = InfluxDBClient(url=url, token=token, org=org)
write_api = client.write_api(write_options=SYNCHRONOUS)


# build the map from id to message
for meg in megs:
    id_to_meg[meg.id] = meg

with open(csv_file_path, mode='r') as file:
    csv_reader = csv.DictReader(file)
    
    # Process each row in the CSV
    for row in csv_reader:
        timestamp = row["Timestamp"]
        dlc = row["DLC"]
        data_id = row["ID"]
        

        point = Point(data_id).time(timestamp, WritePrecision.NS)
        for key, value in row.items():
            if key == 'time':  # Assuming you have a 'time' column for timestamps
                point.time(value, WritePrecision.NS)
            else:
                try:
                    # Try to convert to float if possible
                    value = float(value)
                    point.field(key, value)
                except ValueError:
                    point.tag(key, value)
        
        # Write the Point to InfluxDB
        write_api.write(bucket=bucket, org=org, record=point)

# Close the client
client.close()
