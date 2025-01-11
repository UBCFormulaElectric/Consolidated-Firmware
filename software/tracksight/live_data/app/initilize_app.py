from flask import Flask
from flask_socketio import SocketIO
import os
from dotenv import load_dotenv

from api import api

#creating the app and returning a flask object
def create_app():
    app = Flask(__name__)
    app.register_blueprint(api, url_prefix='/api')
    # load_dotenv()
    # _secret_key = os.getenv('MYSQL_HOST')
    # app.config["SECRET_KEY"] = _secret_key
    return app

#creating SocketIO object
def create_sio(app):
    sio = SocketIO(app)
    return sio
