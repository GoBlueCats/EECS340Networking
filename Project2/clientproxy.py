import random
from socket import*
import sys

#serverSocket_tcp = socket(AF_INET, SOCK_STREM)

#define the port number before hand 
Client_port = 53
Server_port = 53

#proxy as a server, receiver query from client
def resolver():
	ServerSocekt_UDP = socket(AF_INET, SOCK_DGRAM)
	Server_address = ('',Server_port)
	ServerSocekt_UDP.bind(Server_address)

	while True:
		message, address = ServerSocekt_UDP.recvfrom(2048)
		message_new = client(message)
		if message_new:
			ServerSocekt_UDP.sendto(message_new, address)
		else:
			print >>sys.stderr, 'send the wrong message'
		


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
	resolver()



if __name__ = '__main__':
	main()