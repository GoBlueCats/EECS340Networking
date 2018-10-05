import random
from socket import*
import sys

Client_port = 53
Server_port = 53
server_port_tcp = 53
client_port_tcp = 53


def Receive_client():
	ServerSocekt_UDP = socket(AF_INET, SOCK_DGRAM)
	Server_address = ('',Server_port)
	ServerSocekt_UDP.bind(Server_address)

	print >>sys.stderr, 'proxy as server, ready to receive'

	while True:
		
		message, address = ServerSocekt_UDP.recvfrom(2048)
		message_new = client(message)

		if message_new:
			ServerSocekt_UDP.sendto(message_new, address)
		else:
			print >>sys.stderr, 'send the wrong message'

		if trunc(message_new):
			print >>sys.stderr, 'start to build tcp'
			tcp_process(message_new)

## to know if the message from upper server is trunc or not
def trunc(message):
	transfer(message)
	message = message[2][0]
	#convert message from str to bits
	message= ' '.join(format(ord(x), 'b') for x in message)
	if message[6]:
		sys.stderr>>sys.stderr, 'trunc message'
		return True
	else:
		return False

# once server find the response message is truncted, it needs to 
# build up tcp connection and wait for 
def tcp_process(message):
	sys.stderr>>sys.stderr, 'start to build connection between proxy and client'
	server_socket_tcp = socket(AF_INET, SOCK_STREM)
	server_tcp_address = ('', 53)
	server_socket_tcp.bind(server_tcp_address)
	server_socket_tcp.listen(1)
	sys.stderr>>sys.stderr, 'already build tcp connection between proxy and client, listening'

	while  True:
		print >>sys.stderr, 'waiting for a connection'
		connection, client_address = server_socket_tcp.accept()
		try:
        	print >>sys.stderr, 'connection from', client_address
        	while True:
        		data = connection.recv(2048)
        		if data:
        			print >>sys.stderr, 'send request to upper server'
        			response = upper_commute(data)
        			connection.sendall(response)
        		else:
        			print >>sys.stderr, 'no tcp request from client'
        			break
        finally:
        	connection.close()
    server_socket_tcp.close()

def upper_commute(data):
	print >>sys.stderr, 'now connect with upper server'
	ClientSocket_TCP = socket(AF_INET, SOCK_STREM)
	ClientSocket.connect(('8.8.8.8',client_port_tcp))
	response = ''
	try:
		#send data to upper server
		ClientSocket.sendall(data)
		while True:
			data = ClientSocket.recv(2048)
			if not data:
				break
			response += data
	finally:
		print >>sys.stderr, 'now close proxy as client'
		ClientSocket.close()
		return response


def client(message):
	ClientSocket = socket(AF_INET, SOCK_DGRAM)
	UpperServer_address = ('8.8.8.8',Client_port)
	try:
		print >>sys.stderr, 'sending "%s"' % message
		sent = ClientSocket.sendto(message, UpperServer_address)

		print >>sys.stderr, 'waiting to receive'
		data, server = ClientSocket.recvfrom(2048)
		print >>sys.stderr, 'received "%s"' % data

	finally:
		ClientSocket.close()
		return data

def main():
	Receive_Client()


if __name__ = '__main__':
	main()