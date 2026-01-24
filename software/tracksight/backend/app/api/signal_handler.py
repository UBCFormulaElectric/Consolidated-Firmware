import re
from flask import Blueprint, Response, stream_with_context, jsonify, request
from influxdb import BadTimeFormat, get_influxdb_client, get_timed_query
from influxdb_client.rest import ApiException
from settings import INFLUX_BUCKET
from middleware.candb import live_can_db

signal_handler = Blueprint("influx_handler", __name__)

@signal_handler.route("/signal/nodes", methods=["GET"])
def get_nodes():
    """
    Gets the list of all nodes (str) in the current parser.
    """
    return jsonify(list(live_can_db.nodes.keys())), 200

@signal_handler.route("/signal/metadata", methods=["GET"])
def get_signal_metadata():
    """
    Gets metadata of the signal of the current parser.
    Pass signal names (regex) in `name` parameter.
    E.g. `/signal?name=INVFR_bError,*_bReserve`
    """
    name_arg = request.args.get("name")

    def condition(signal_name):
        return not name_arg or re.search(name_arg, signal_name)

    return jsonify({signal.name:
        {
            "name": signal.name,
            "min_val": signal.min_val,
            "max_val": signal.max_val,
            "unit": signal.unit,
            "enum": signal.enum,
            "tx_node": msg.tx_node_name,
            "cycle_time_ms": msg.cycle_time,
            "id": msg.id,
            "msg_name": msg.name,
        }
        for msg in live_can_db.msgs.values()
        for signal in msg.signals
        if condition(signal.name)
    }), 200

@signal_handler.route("/signal", methods=["GET"])
def get_signal():
    """
    Gets the signals of the current parser. 
    
    By default, 5 minute interval of data is returned unless both `start` and `end` parameters are provided.

    Optional parameters:
    `name` - Pass signal name (regex).
    `start` - Start time for query (RFC3339 format)
    `end` - End time for query (RFC3339 format)

    E.g. `/signal?name=INVFR_bError&start=2025-11-01&end=2025-11-01T01:00:00`
    - Matches exactly `INVFR_bError` between Nov 1, 2025 00:00 to Nov 1, 2025 01:00
    """
    # parsing args
    name_arg = request.args.get("name")
    start = request.args.get("start")
    end = request.args.get("end")

    query: str
    try:
        query = get_timed_query(
            bucket=INFLUX_BUCKET,
            start=start,
            end=end
        )
    except BadTimeFormat as btf:
        return f"bad time: {btf}", 400

    if name_arg:
        # i love arbitrary query execution!!!!
        query += f'|> filter(fn: (r) => r._field =~ /^{name_arg}$/)'

    print(query)

    results = None
    with get_influxdb_client() as client:
        try:
            results = client.query_api().query(query=query)
        except ApiException as ae:
            return f"bad query: {ae}", 400

    measurement_names = []
    for table in results:
        for record in table.records:
            values = record.values
            values['_time'] = str(values['_time'])
            measurement_names.append(values)

    return jsonify(measurement_names), 200

@signal_handler.route("/signal/csv", methods=["GET"])
def get_csv_file():
    """
    Given start and end time, returns csv file of all data in time frame from `INFLUX_BUCKET`.
    Optional parameters:
    `start` - Start time for query (RFC3339 format)
    `end` - End time for query (RFC3339 format)

    Otherwise, return 5 minute interval relative to whatever is provided:
    - `start` to `start` + 5 min
    - `end` - 5 min to `end` 
    """
    start = request.args.get("start")
    end = request.args.get("end")
    separator = ','
    query = get_timed_query(
        bucket=INFLUX_BUCKET,
        start=start,
        end=end,
    )

    csv_results = None
    with get_influxdb_client() as client:
        try:
            csv_results = client.query_api().query_csv(query=query)
        except ApiException:
            return "bad date query", 400
    csv_results = list(map(lambda x: separator.join(x) + '\n', csv_results))

    return Response(
        stream_with_context(iter(csv_results)),
        mimetype="text/csv",
        headers={"Content-Disposition": f"attachment; filename={start}_{end}.csv"}
    )