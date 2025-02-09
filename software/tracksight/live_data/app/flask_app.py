# FLASK APP
from flask import Flask, Blueprint
from flask_socketio import SocketIO
from flask_cors import CORS
# EXPORTS app
app = Flask(__name__)
CORS(app)
api = Blueprint('api', __name__)
# SocketIO
sio = SocketIO(app)