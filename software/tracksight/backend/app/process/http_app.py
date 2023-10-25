from flask import Blueprint, jsonify
from process import SignalUtil

# HTTP processes for data that is not live
app = Blueprint('http_app', __name__)
signal_util = SignalUtil.SignalUtil()

@app.route('/')
def hello_world():
    return "Wireless 2.0"

# returns all available signals to the client
@app.route('/signal', methods=['GET'])
def return_all_available_signals():
    signals = signal_util.get_all_signals()
    signal_names = list(signals.keys())

    response = jsonify(signal_names)
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response

# returns signal data to client based on signal name
@app.route('/signal/<string:name>', methods=['GET'])
def return_signal(name):
    signal_data = signal_util.get_signal(name).to_dict()
    response = jsonify(signal_data)
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response
