from flask import Blueprint, jsonify, request
from logger import logger
from middleware.candb import live_can_db
from middleware.subtable import SubscriptionError, subscribe_signal, unsubscribe_signal

sub_handler = Blueprint("subtable_handler", __name__)

@sub_handler.errorhandler(SubscriptionError)
def subscription_error_handler(err):
	logger.error(f"Subscription error: {err}")
	return jsonify({"error": str(err)}), 400

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

	subscribe_signal(request_sid, signal_name)

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

	unsubscribe_signal(request_sid, signal_name)

	logger.info(f"{signal_name} removed from {request_sid}")
	return jsonify({"msg": f"Unsubscribed {request_sid} from {signal_name}"}), 200
