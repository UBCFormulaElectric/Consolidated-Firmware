# api blueprints
from flask import Blueprint
http = Blueprint("util", __name__)

@http.route("/")
def index():
    return "<p>Welcome to the UBC Formula Electric Telemetry Backend</p>"

@http.route("/health", methods=["GET"])
def hello_world():
    return "<p>Hello, World!</p>"
