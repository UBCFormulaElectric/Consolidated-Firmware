import pytest
import json
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(__file__)))
from telemetry import app, socketio  # Replace 'your_app_file' with the actual filename of your Flask app

@pytest.fixture
def client():
    app.config['TESTING'] = True
    client = app.test_client()
    socketio_client = socketio.test_client(app)
    return client, socketio_client

def test_hello_world(client):
    client, _ = client
    response = client.get('/')
    assert response.status_code == 200
    assert b"Wireless 2.0" in response.data

def test_socketio_connect(client):
    _, socketio_client = client
    received_events = socketio_client.get_received()
    assert len(received_events) == 1
    assert received_events[0]['name'] == 'message'

def test_socketio_message(client):
    _, socketio_client = client
    socketio_client.emit('data', 'Test message')
    received_events = socketio_client.get_received()

def test_get_signals(client):
    _, socket_client = client
    req = '{"ids": ["Signal1"]}'
    print(req, type(req))
    x = json.loads(req)
    print("here", x)
    socket_client.emit('signals', json.loads(req))
    events = socket_client.get_received()
    print(events)
    assert False

