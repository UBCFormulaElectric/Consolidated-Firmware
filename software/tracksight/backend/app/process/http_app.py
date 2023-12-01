from flask import Blueprint, jsonify, request
from process import SignalUtil
from influx_handler import InfluxHandler as influx
from influx_handler import NoDataForQueryException

# HTTP processes for data that is not live
app = Blueprint('http_app', __name__)
signal_util = SignalUtil.SignalUtil()

@app.route('/')
def hello_world():
    return "Wireless 2.0"

# function set for returning signal names
@app.route('/signal', methods=['GET'])
def return_all_available_signals():
    signals = signal_util.get_all_signals() 
    signal_names = list(signals.keys())  # returns list of keys 

    return responsify(signal_names)

@app.route('/signal/measurement', methods=['GET'])
def return_all_measurements():
    measurements = influx.get_measurements()
    return responsify(measurements)

@app.route('/signal/fields/<string:measurement>', methods=['GET'])
def return_all_fields_for_measurement(measurement):
    fields = influx.get_fields(measurement)

    return responsify(fields)

@app.route('/query', methods=['GET'])
def return_query():
    params = request.args
    measurement = params.get("measurement")
    fields = params.get("fields").split(",")
    start_epoch = params.get("start_epoch")
    end_epoch = params.get("end_epoch")

    try:
        data = influx.query(measurement, fields, [start_epoch, end_epoch])
        print(data)
    except NoDataForQueryException as e:
        return responsify({"error": str(e)}), 400

    return responsify(data)

# function set for return signals 
@app.route('/signal/<string:name>', methods=['GET'])
def return_signal(name):
    signal_data = signal_util.get_signal(name).to_dict()
    return responsify(signal_data)

def responsify(data):
    response = jsonify(data)
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response
