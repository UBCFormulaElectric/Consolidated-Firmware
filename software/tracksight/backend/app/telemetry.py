from flask import jsonify, request, Flask, Response
from flask_socketio import SocketIO, emit
from process import SignalUtil  # Assuming SignalUtil is in a module named 'process'

from influx_handler import InfluxHandler as influx
from influx_handler import NoDataForQueryException


app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")
signal_util = SignalUtil.SignalUtil()  # Initialize your SignalUtil class

@app.route('/')
def hello_world():
    return "Wireless 2.0"

@socketio.on('connect')
def handle_connect():
    print("Client Connected")
    emit('message', 'You are connected to the server connect')

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
    except NoDataForQueryException as e:
        return responsify({"error": str(e)}), 400

    return responsify(data)

@app.route('/signal/<string:name>', methods=['GET'])
def return_signal(name):
    signal_data = signal_util.get_signal(name).to_dict()
    return responsify(signal_data)

def responsify(data):
    response = jsonify(data)
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response
    
if __name__ == '__main__':
    # app.run(debug=True)  # This starts another server

    # Start the Socket.IO server
    socketio.run(app, debug=True, allow_unsafe_werkzeug=True)
