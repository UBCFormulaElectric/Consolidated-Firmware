from ntplib import NTPPacket
from ntplib import ntp_to_system_time
import socket

_TIMEOUT = 5
_NTP_VERSION = 3
_NTP_HOST = "0.us.pool.ntp.org"

def ntp_request(t_0: float) -> tuple[float, float]:
	"""
	Given t_0, returns t_1, t_2
	"""
	addrinfo = socket.getaddrinfo(_NTP_HOST, "ntp")[0]
	family, sockaddr = addrinfo[0], addrinfo[4]
	with socket.socket(family, socket.SOCK_DGRAM) as s:
		s.settimeout(_TIMEOUT)

		# create the request packet - mode 3 is client
		query_packet = NTPPacket(mode=3, version=_NTP_VERSION, tx_timestamp=t_0) # type: ignore

		# send the request
		s.sendto(query_packet.to_data(), sockaddr)

		# wait for the response - check the source address
		response_packet, src_addr = None, (None, )
		while src_addr[0] != sockaddr[0]:
			response_packet, src_addr = s.recvfrom(256)

		# build the destination timestamp
		# dest_timestamp = system_to_ntp_time(time.time())

	# construct corresponding statistics
	stats = NTPPacket()
	stats.from_data(response_packet)
	t_1 = stats.recv_timestamp # t_1
	t_2 = stats.tx_timestamp   # t_2

	return (t_1, t_2)

if __name__ == "__main__":
	from datetime import datetime
	t_1, t_2 = ntp_request(0)
	print(datetime.fromtimestamp(ntp_to_system_time(t_1)), datetime.fromtimestamp(ntp_to_system_time(t_2)))