from typing import Any
from flask import Blueprint, jsonify, request
from logger import logger
from middleware.candb import live_can_db
from middleware.subtable import SubscriptionError, get_sid_signals, subscribe_signal, unsubscribe_signal, paused_sids

sub_handler = Blueprint("subtable_handler", __name__)

class RequestError(Exception):
	...

@sub_handler.errorhandler(SubscriptionError)
def subscription_error_handler(err):
	logger.error(f"Subscription error: {err}")
	return jsonify({"error": str(err)}), 400

@sub_handler.errorhandler(RequestError)
def request_error_handler(err):
	logger.error(f"Request error: {err}")
	return jsonify({"error": str(err)}), 400

def get_signal_name(data: Any):
	signal_name = data.get("signal", None)
	if not signal_name:
		raise RequestError("No signal provided")
	if type(signal_name) is not str:
		raise RequestError("Signal must be a string")
	return signal_name

def get_sid(data: Any):
	sid = data.get("sid", None)
	if not sid:
		raise RequestError("No SID provided")
	if type(sid) is not str:
		raise RequestError("SID must be a string")
	return sid

@sub_handler.route("/subscribe", methods=["POST"])
def subscribe():
	data = request.get_json()
	signal_name = get_signal_name(data)
	request_sid = get_sid(data)

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
	signal_name = get_signal_name(data)
	request_sid = get_sid(data)

	if request_sid is None:
		return jsonify({"error": "Failed to subscribe: no SID provided"}), 400
	if type(signal_name) is not str:
		return jsonify({"error": "Failed to subscribe: signal must be a string"}), 400

	unsubscribe_signal(request_sid, signal_name)

	logger.info(f"{signal_name} removed from {request_sid}")
	return jsonify({"msg": f"Unsubscribed {request_sid} from {signal_name}"}), 200

@sub_handler.route("/<sid>/signals", methods=["GET"])
def get_subscribed_signals(sid: str):
	return jsonify(get_sid_signals(sid)), 200

@sub_handler.route("/<sid>/play", methods=["POST"])
def play_subscribed_signals(sid: str):
	if sid not in paused_sids:
		return jsonify({"msg": f"SID {sid} is not paused"}), 209
	paused_sids.remove(sid)
	return jsonify({"msg": f"Resumed signals for SID {sid}"}), 200

@sub_handler.route("/<sid>/pause", methods=["POST"])
def pause_subscribed_signals(sid: str):
	if sid in paused_sids:
		return jsonify({"msg": f"SID {sid} is already paused"}), 209
	paused_sids.add(sid)
	return jsonify({"msg": f"Paused signals for SID {sid}"}), 200
