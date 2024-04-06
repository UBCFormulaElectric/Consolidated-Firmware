from flask import Flask, request, jsonify
import json
import os
from flask_cors import CORS
from process.http_app import app as http_app
from process.socket_app import socketio as socket_app


app = Flask(__name__)
CORS(app)
app.register_blueprint(http_app)

JSON_FILE = './dashboards.json'

def read_json_file():
    if not os.path.exists(JSON_FILE):
        return {}
    with open(JSON_FILE, 'r') as file:
        return json.load(file)

def write_json_file(data):
    with open(JSON_FILE, 'w') as file:
        json.dump(data, file, indent=4)

@app.route('/get-data')
def get_data():
    path = request.args.get('path')
    data = read_json_file()
    # Access data by path. For simplicity, this example assumes 'path' directly maps to key(s) in the JSON.
    # Adjust based on your data structure.
    path_data = data.get(path, 'No data found')
    return jsonify(path_data)

@app.route('/save-data', methods=['POST'])
def save_data():
    content = request.json
    data_to_save = content['data']
    data = read_json_file()
    # may want to add error handling when there is duplicate name? 
    name = data_to_save["dbname"]
    data['dashboards'][name] = data_to_save
    write_json_file(data)
    return jsonify({'message': 'Data saved successfully'})

@app.route('/delete-data', methods=['POST'])
def delete_data():
    content = request.json
    path = content['path']
    print(path)
    data = read_json_file()

    # Split the path to get the section (e.g., 'dashboards') and the key (e.g., 'ass')
    path_parts = path.split('/')
    if len(path_parts) == 2 and path_parts[0] == 'dashboards' and path_parts[1] in data.get('dashboards', {}):
        # Delete the specific dashboard
        del data['dashboards'][path_parts[1]]
        write_json_file(data)
        return jsonify({'message': 'Data deleted successfully'})
    else:
        return jsonify({'error': 'Data not found'}), 404


if __name__ == '__main__':
    socket_app.init_app(app)  # Initialize the Socket.IO app with the main app
    socket_app.run(app, debug=True, allow_unsafe_werkzeug=True, host='0.0.0.0')
