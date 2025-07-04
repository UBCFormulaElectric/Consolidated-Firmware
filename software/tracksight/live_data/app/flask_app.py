# FLASK APP
from flask import Flask
from flask_cors import CORS

# EXPORTS app
app = Flask(__name__)
CORS(app)
