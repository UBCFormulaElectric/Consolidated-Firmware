from flask import Blueprint, jsonify, request
from logger import logger
from middleware.candb import live_can_db

sub_handler = Blueprint("subtable_handler", __name__)
SUB_TABLE = {}

@sub_handler.route("/subscribe", methods=["POST"])
def subscribe():
	data = request.get_json()
	signal_name = data.get("signal", None)
	if not signal_name:
		return jsonify({"error": "Failed to subscribe: no signal provided"}), 400
	if type(signal_name) is not str:
		return jsonify({"error": "Failed to subscribe: signal must be a string"}), 400

	request_sid = data.get("sid", None)
	if not request_sid:
		return jsonify({"error": "Failed to subscribe: no SID provided"}), 400
	if type(signal_name) is not str:
		return jsonify({"error": "Failed to subscribe: signal must be a string"}), 400

	# note this checks a proxy, namely whether the mapping between signal name and it's corresponding message exists
	valid_signal = live_can_db.signals_to_msgs.get(signal_name) is not None
	if not valid_signal:
		logger.error(f"{request_sid} failed to subscribe to {signal_name}")
		return jsonify({"error": "Invalid signal"}), 400

	if request_sid not in SUB_TABLE:
		return jsonify({"error": "Failed to subscribe: SID is not connected"}), 400
	SUB_TABLE[request_sid].add(signal_name)

	logger.info(f"{request_sid} subscribed to {signal_name}")
	return jsonify({"msg": f"Subscribed {request_sid} to {signal_name}"}), 200

@sub_handler.route("/unsubscribe", methods=["POST"])
def unsubscribe():
	data = request.get_json()
	signal_name = data.get("signal", None)
	if not signal_name:
		return jsonify({"error": "Failed to subscribe: no signal provided"}), 400
	if type(signal_name) is not str:
		return jsonify({"error": "Failed to subscribe: signal must be a string"}), 400

	request_sid = data.get("sid", None)
	if request_sid is None:
		return jsonify({"error": "Failed to subscribe: no SID provided"}), 400
	if type(signal_name) is not str:
		return jsonify({"error": "Failed to subscribe: signal must be a string"}), 400

	if request_sid not in SUB_TABLE:
		logger.error(f"{request_sid} failed to unsubscribe to {signal_name}: not subscribed")
		return jsonify({"error": "Not subscribed"}), 400
	if signal_name not in SUB_TABLE[request_sid]:
		logger.warning(f"{request_sid} failed to unsubscribe to {signal_name}")
		return jsonify({"error": "Invalid signal"}), 400
	SUB_TABLE[request_sid].remove(signal_name)

	logger.info(f"{signal_name} removed from {request_sid}")
	return jsonify({"msg": f"Unsubscribed {request_sid} from {signal_name}"}), 200
