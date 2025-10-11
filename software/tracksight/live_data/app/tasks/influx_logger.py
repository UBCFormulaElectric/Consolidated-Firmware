"""
Influx database handler class.

File for handling influxdb queries.
This requires the influx dbrc mapping to have db name == bucket name
TODO: Implement proper error handling for things like no data available.
TODO: explore configuration options for the database
link here: https://docs.influxdata.com/influxdb/v2/reference/config-options/
"""

from queue import Empty, Queue
from threading import Thread

# influx
from flask_socketio import SocketIO
import influxdb_client
from influxdb_client.client.write_api import WriteOptions, WriteType
# ours
from CanMsg import CanSignal
from logger import logger
from settings import CAR_NAME, INFLUX_BUCKET, INFLUX_ORG, INFLUX_TOKEN, INFLUX_URL
from urllib3.exceptions import NewConnectionError

from tasks.stop_signal import should_run

if INFLUX_ORG is None:
    raise Exception("No Influx Organization Provided")
if INFLUX_TOKEN is None:
    raise Exception("No Token Provided for Influx")
if CAR_NAME is None:
    raise Exception("No Car Name is Provided")


def setup():
    # Checks if the vehicle bucket exists, and if not, creates it
    logger.debug(
        f"Connecting to InfluxDB database at '{INFLUX_URL}' with token '{INFLUX_TOKEN}' and org '{INFLUX_ORG}'."
    )
    if not INFLUX_ORG:
        raise ValueError("INFLUX_ORG must be set in the environment variables.")
    with influxdb_client.InfluxDBClient(
        url=INFLUX_URL,
        token=INFLUX_TOKEN,
        org=INFLUX_ORG,
        debug=False,
    ) as _client:
        logger.debug("Connected to InfluxDB successfully.")
        try:
            # creates the can_data bucket if it doesn't exist yet
            logger.info(f"Checking if bucket '{INFLUX_BUCKET}' exists...")
            if (
                _client.buckets_api().find_bucket_by_name(bucket_name=INFLUX_BUCKET)
                is None
            ):
                _client.buckets_api().create_bucket(bucket_name=INFLUX_BUCKET)
                logger.info(f"Created bucket '{INFLUX_BUCKET}' successfully.")
            else:
                logger.info(f"Bucket '{INFLUX_BUCKET}' already exists.")
        except NewConnectionError:
            raise Exception(
                "InfluxDB is not responding. Have you started the influx database docker container?"
            )

influx_queue: Queue[CanSignal] = Queue()

def _log_influx() -> None:
    logger.debug("Starting InfluxDB logger thread")
    with influxdb_client.InfluxDBClient(
        url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG, debug=False
    ) as _client:
        with _client.write_api(
            write_options=WriteOptions(
                write_type=WriteType.synchronous, batch_size=10)
        ) as write_api:
            while should_run():
                try:
                    signal = influx_queue.get(timeout=1)
                except Empty:
                    continue

                logger.debug(f"Writing {signal.name} with value {signal.value} to InfluxDB at {INFLUX_URL}")
                # write the signal to influxdb
                write_api.write(
                    bucket=INFLUX_BUCKET,
                    org=INFLUX_ORG,
                    record={
                        "measurement": f"{CAR_NAME}_live",
                        "fields": {signal.name: signal.value},
                        "time": signal.timestamp,
                    },
                    write_precision=influxdb_client.WritePrecision.MS,
                )
    logger.debug("InfluxDB logger thread stopped.")


def get_influx_logger_task(sio: SocketIO) -> Thread:
    return sio.start_background_task(_log_influx)


# def get_measurements(cls) -> list[str]:
# 	"""
# 	Get all measurements from the database.
# 	:param bucket: Name of bucket to fetch data from.
# 	:returns List of all measurements.
# 	"""
# 	if not cls.is_setup:
# 		raise RuntimeError("InfluxHandler not initialized.")

# 	query = f"""
# 	import "influxdata/influxdb/schema"
# 	schema.measurements(bucket: \"{_INFLUX_BUCKET}\")"""

# 	with influxdb_client.InfluxDBClient(
# 		url=_INFLUX_URL, token=_INFLUX_TOKEN, org=_INFLUX_ORG, timeout=100_000_000, debug=False
# 	) as _client:
# 		return [
# 			str(i[0])
# 			for i in _client.query_api().query(query).to_values(columns=["_value"])
# 		]

# this gets all the signals which are present
# this does NOT represent the list of valid signals for a given commit
# def get_signals(measurement: str) -> list[str]:
# 	"""
# 	Get all signals from the database.
# 	:param bucket: Name of bucket to fetch data from.
# 	:returns List of all measurements.
# 	"""
# 	query = f"""
# 	import "influxdata/influxdb/schema"
# 	schema.tagValues(
# 		bucket: "{_INFLUX_BUCKET}",
# 		predicate: (r) => r._measurement == "{measurement}",
# 		tag: "signal"
# 	)"""

# 	return [
# 		str(i[0])
# 		for i in _client.query_api()
# 		.query(query=query)
# 		.to_values(columns=["_value"])
# 	]

# def query(
# 	measurement: str,
# 	signals: List[str],
# 	time_range: Tuple[str, str],
# 	max_points: int,
# 	ms_resolution: int = 100,  # TODO implement
# ) -> dict[str, dict]:
# 	"""
# 	Make a general query to the database.
# 	:param measurement: Measurement to pull data from.
# 	:param fields: Fields to fetch.
# 	:param time_range: Tuple like (time start, time end) to specify the time interval.
# 	:param bucket: Name of bucket to fetch data from.
# 	:param max_points: Maximum number of datapoints to fetch.
# 	:param ms_resolution: Minimum time delta required before grabbing a new datapoint.
# 	:return: A dictionary where the keys are the fields and the values are TimeValue objects.
# 	"""

# 	query = f"""
# 	from(bucket:"{_INFLUX_BUCKET}")
# 		|> range(start: {time_range[0]}, stop: {time_range[1]})
# 		|> filter(fn: (r) =>
# 			r._measurement == "{measurement}" and
# 			r._field == "value" and
# 			contains(value: r.signal, set: {str(signals).replace("'", '"')}))
# 		|> tail(n: {max_points})
# 	"""

# 	query_result = {signal: {"times": [], "values": []} for signal in signals}
# 	for signal, value, time in (
# 		_client.query_api()
# 		.query(query=query)
# 		.to_values(columns=["signal", "_value", "_time"])
# 	):
# 		query_result[signal]["times"].append(str(time))
# 		query_result[signal]["values"].append(value)

# 	return query_result