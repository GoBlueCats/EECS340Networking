from urllib2 import urlopen
import socket

#return type is str
def public_ip():
	public_str = urlopen('http://ip.42.pl/raw').read()
	print "public ip is: ", public_str
	return public_str

def private_ip():
	result = socket.getfqdn().split('.')[0].split('-')
	private_ip = ''
	for i in result[1:-1]:
		private_ip += i + '.'
	private_ip += result[-1] 
	print "private ip is: ", private_ip
	return private_ip

def main():
	public_ip()
	private_ip()


if __name__ = '__main__':
	main()