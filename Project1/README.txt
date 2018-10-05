Project 1 should be done in groups of two or individually.

This assignment was originally developed by Prof. Peter Dinda, but I have modified it in several ways.

Overview
In this part of the project, your group will build a simple web client and a web server to which it can connect.  The goal is to slowly introduce you to Unix socket programming, and the HTTP protocol, which is the most important network application protocol on the Internet.  You may develop the client and server in either C, C++, or Python.

Notice that I am not asking you to build a web page, but a web server.  Website developers normally deploy their pages on an existing web server framework such as Apache, Node.js, Django, Tomcat, Microsoft IIS, etc.  You'll be building such a web server from scratch, and in the process you will learn about HTTP and low-level network programming.

Coding
I recommend using Python because it allows you to handle strings much more easily than C or even C++.  If you use Python then you must use the low-level socket API for networking and you must use the version of Python that is installed on the test machine (version 2.6).  (The Python version 2.7 that comes bundled with Macs is probably close enough to 2.6 that you can use it for development.)

You must include a makefile that builds the code on murphy.wot.eecs.northwestern.edu and you must test your code on that particular machine before submitting.

HTTP and HTML
The combination of HTTP, the Hypertext Transport Protocol, and HTML, the Hypertext Markup Language, forms the basis for the World Wide Web. HTTP provides a standard way for a client to request typed content from a server, and for a server to return such data to the client. "Typed content" simply means a bunch of bytes annotated with information that tells us how we should interpret them (a MIME type).  For example, the MIME type "text/html" tells us that the bytes are HTML text and the MIME type "image/jpeg" tells us that the bytes represent an image in JPEG format.  You will implement a greatly simplified version of HTTP 1.0.

HTML content provides a standard way to encode structured text that can contain pointers to other typed content. A web browser parses an HTML page, fetches all the content it refers to, and then renders the page and the additional embedded content appropriately.

HTTP Example
In this project, you will only implement HTTP, and only a tiny subset of HTTP 1.0 at that. HTTP was originally a very simple, but very inefficient protocol.  As a result of fixing its efficiency problems, modern HTTP is considerably more complicated. It’s current specification, RFC 2616 (Links to an external site.)Links to an external site., is over a hundred pages long!  Fortunately, for the purposes of this project, we can ignore most of the specification and implement a tiny subset.  In fact, it may be easier to refer to the older, simpler version of the HTTP spec for this assignment, RFC 1945 (Links to an external site.)Links to an external site..

The HTTP protocol works on top of TCP, a reliable, stream-oriented transport protocol, and it uses human-readable messages. Because of these two facts, we can use the telnet program to investigate how HTTP works. We’ll use telnet in the role of the client and www.eecs.northwestern.edu in the role of the server. The typed content we’ll transfer is the EECS department’s home page. This is essentially the same as fetching the home page using your favorite web browser.

[spt175@murphy ~]$ telnet eecs.northwestern.edu 80
Trying 129.105.8.237...
Connected to eecs.northwestern.edu.
Escape character is '^]'.
GET /index.html HTTP/1.0

HTTP/1.1 200 OK
Date: Mon, 09 Apr 2018 17:45:47 GMT
Server: Apache
X-Powered-By: PHP/5.4.16
Set-Cookie: da041beaac9edce9f7dbdeb77c=cf5rdrblmcv89iamtekgodklt2; path=/; HttpOnly
P3P: CP="NOI ADM DEV PSAi COM NAV OUR OTRo STP IND DEM"
Expires: Mon, 1 Jan 2001 00:00:00 GMT
Last-Modified: Mon, 09 Apr 2018 17:45:50 GMT
Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0
Pragma: no-cache
Connection: close
Content-Type: text/html; charset=utf-8
...
</html>
Connection closed by foreign host.
[spt175@murphy ~]$
Above, I just ran the command "telnet eecs.northwestern.edu 80" then I typed "GET /index.html HTTP/1.0" and pressed "return" twice to send an HTTP command to the server.  The telnet command just creates a TCP connection to the specified server and port (port 80 is the standard HTTP port), and then it lets the client and server send text back and forth to each other.  I specified the address with a DNS name, "eecs.northwestern.edu", so the telnet program actually has to first do a DNS lookup to determine that this corresponds to IP address 129.105.8.237.

"GET /index.html HTTP/1.0" is the most basic type of HTTP 1.0 request, and this is the type that you will implement. It says “please give me the file that you think of as /index.html using the 1.0 version of the HTTP protocol.”  HTTP requests can be several lines long, so the blank line and the end indicates that the request is finished

The response always begins with a line that states the version of the protocol that the server speaks (“HTTP/1.1” in this case), an error code (“404”), and a textual description of what that error code means (“Not Found”). Next, the server provides a bunch of information about the content it is about the send as well as information about itself and what kinds of services it can provide. The most critical lines here are “Content-Length: 2070”, which tells us that the content will consist of 2070 bytes, and “Content-Type: text/html”, which tells us how to interpret the content we shall receive. A blank line marks the end of the response header and the beginning of the actual content (the body). After the content has been sent, the server closes the connection.

Part 1: a simple curl clone
This part is worth 35% of the total grade, if completed correctly.

You will implement a simple command-line HTTP client, implemented using the BSD socket interface.  If using Python, you must use the "socket" package and no other (higher-level) network library.  If using C/C++, you must "#include <sys/socket.h>" and not use any higher-level network library.

Like a simplified version of the unix "curl" command, your program should take just one parameter, which is an http web address to fetch.  If successful, if should print the body of the response (the html code) to stdout.

For example, you might invoke it as follows:

$ ./http_client http://somewebsite.com/path/page.htm (Links to an external site.)Links to an external site.
And it would print out the HTML for that page.

Additionally, you must follow these rules:

All requests are assumed to use the HTTP "GET" method (you do not have to support POST or anything else).
Your client must include a "Host: ..." header.  This is necessary because some web servers handle multiple domains.
Your client should understand and follow 301 and 302 redirects.  When you get a redirect, your client should make another request to fetch the corrected url.  Your client should also print a message to stderr explaining what happened, like "Redirected to: http://other.com/blah"
An example of a url with a 301 permanent redirect is:
http://airbedandbreakfast.com/ (Links to an external site.)Links to an external site. (which redirects to https://www.airbnb.com/belong-anywhere (Links to an external site.)Links to an external site.)
An example of a url with a 302 temporary redirect is:
http://maps.google.com/ (Links to an external site.)Links to an external site. (which redirects to http://maps.google.com/maps,  (Links to an external site.)Links to an external site.which redirects to https://www.google.com:443/maps (Links to an external site.)Links to an external site.)
Another example is:
http://stevetarzia.com/index.php  (Links to an external site.)Links to an external site.which redirects to http://stevetarzia.com/index (Links to an external site.)Links to an external site..  Unlike the above, the final destination is not encrypted with HTTPS, so your program should be able to print the HTML.
You must handle a chain of multiple redirects (like the above), but give up after 10 redirects and return an error.  For example, fetching this page should not loop infinitely, but return an error:
http://stevetarzia.com/redirect-hell (Links to an external site.)Links to an external site. 
Your program should return a unix exit code of 0 on success (meaning that you eventually get a "200 OK" response) and non-zero on failure.
If you try to visit a https page (requiring encryption) or if you are redirected to such a page, just return an exit with an error status.
If you get an HTTP response code >= 400, then you should return a non-zero exit code, but also print the response body, if any.
For example, the following page returns a 400 response:
http://thefacebook.com/ (Links to an external site.)Links to an external site.
Check the response's content-type header.  Print the body to stdout only if the content-type begins with "text/html".  Otherwise, exit with a non-zero exit code.
Return an error code if the input url does not start with "http://"
Allow request urls to include a port number.
For example: http://portquiz.net:8080/ (Links to an external site.)Links to an external site.
You should not require a slash at the end of top-level urls.  For example, both of the following urls should work: http://stevetarzia.com (Links to an external site.)Links to an external site. and http://stevetarzia.com/ (Links to an external site.)Links to an external site.
You should be able to handle large pages, such as: http://stevetarzia.com/libc.html (Links to an external site.)Links to an external site. 
Part 2: a simple web server
This part is worth 35% of the total grade, if completed correctly.

As above, you must implement this using low-level BSD sockets, but you may use C, C++, or Python.

Write an HTTP server that handles one connection at a time and that serves any files in the current directory if their name ends with  .html or .htm.  To prepare for this assignment, download the sample html file rfc2616.html and save it to your working directory.  The command-line interface for your server will be

$ ./http_server1 [port]
Where "port" is a port number >= 1024.  You will then be able to use http_client, telnet, or any web browser, to fetch files from your server. For example, if you run your server on murphy.wot.eecs.northwestern.edu on port 10002, then you can access your server by visiting the following url on the web browser on a machine that is on the wired Northwestern network: "http://murphy.wot.eecs.northwestern.edu:10002/rfc2616.html"

It is important to note that you will not be able to use port 80.  Ports less than 1024 are reserved, and you need special permissions to bind to them.  The firewall will block access to murphy's high ports if you are not on the Northwestern wired campus network.

Your server should have the following structure:

Create a TCP socket on which to listen for new connections. (What packet family and type should you use?)
Bind that socket to the port provided on the command line. We’ll call this socket the "accept socket."  Please listen on all IP addresses on the machine by choosing INADDR_ANY (in C) or "" (in Python) as the listening address.
Listen on the accept socket. (What will happen if you use a small backlog versus a larger backlog? What if you set the backlog to zero?)
Do the following repeatedly:
a. Accept a new connection on the accept socket. (When does accept return? Is your process consuming cycles while it is in accept?) Accept will return a new socket for the connection. We’ll call this new socket the "connection socket." (What is the 5-tuple describing the connection?)

b. Read the HTTP request from the connection socket and parse it. (How do you know how many bytes to read?)

c. Check to see if the requested file requested exists (and ends with ".htm" or ".html").

d. If the file exists, construct the appropriate HTTP response (What’s the
right response code?), write the HTTP header to the connection socket, and then open the file and write its contents to the connection socket (thus writing the HTTP body).

e. If the file doesn’t exist, construct a HTTP error response (404 Not Found) and write it to the connection socket.  If the file does exist, but does not end with ".htm" or "html", then write a "403 Forbidden" error response.

f. Close the connection socket.

TIP: If you are running your server code locally (on your laptop), then you can use the special hostname "localhost" to refer to your own machine.  For example, direct your browser to "http://localhost:8000/index.html".

To test your server, you should try to load a page with a standard browser, like Chrome.  If you have done everything correctly, Chrome will render your version of the page as shown at https://tools.ietf.org/html/rfc2616 (Links to an external site.)Links to an external site..

Part 3: multi-connection web server
This part is worth 30% of the total grade, if completed correctly

The server you wrote for part 2 can handle only one connection at a time. To illustrate the problem, try the following. Open a telnet connection to your http_server1 and type nothing. Now make a request to your server using a browser. What happens? If the connection request is refused, try increasing the backlog you specified for listen in http_server1 and then try again. After http_server1 accepts a connection, it blocks (stalls) while reading the request and thus is unable to accept another connection. Connection requests that arrive during this time are either queued, if the listen queue (whose size you specified using listen) is not full, or refused, if it is.

Consider what happens if the current connection is very slow, for example if it is running over a weak cellular signal. Your server is spending most of its time idle waiting for this slow connection while other connection requests are being queued or refused. Reading the request is only one place where http_server1 can block. It can also block on waiting for a new connection, on reading data from a file, and on writing that data to the socket.

Write an HTTP server, http_server2, that avoids two of these situations: (1) waiting for a connection to be established, and (2) waiting on the read after a connection has been established. You can make the following assumptions:

If you can read one byte from the socket without blocking, you can read the whole request without blocking.

Reads on the file will never block

Writes will never block

It is important to note that if you have no connections with new data available and no new connections just established, then you should block.

To support multiple connections at a time in http_server2, you will need to do two things:

Explicitly maintain the state of each open connection

Block on multiple sockets, file descriptors, events, etc.

It is up to you to decide what the contents of the state of a connection are and how you will maintain them. However, Unix, as well as most other operating systems, provides a mechanism for waiting on multiple events. The Unix mechanism is the select system call.  If you have not taken an Operating Systems course, then the term "system call" may be unfamiliar.  System calls are special functions that allows a process to ask the OS to do something on its behalf.  For the purposes of this assignment, just think of "select" as a library function, like "printf".

"Select" allows us to wait for one or more file descriptors (a socket is a kind of file descriptor) to become available for reading (so that at least one byte can be read without blocking), writing (so that at least one byte can be written without blocking), or to have an exceptional condition happen (so that the error can be handled). In addition, select can also wait for a certain amount of time to pass. If you're using python, you can read about select here: https://pymotw.com/2/select/ (Links to an external site.)Links to an external site..

Your server should have the following structure:

Create a TCP socket on which to listen for new connections

Bind that socket to the port provided on the command line.

Listen on that socket, which we shall call the "accept socket."

Initialize the list of open connections to empty

Do the following repeatedly:

a. Make a list of the sockets we are waiting to read from the list of open connections. We shall call this the "read list."

b. Add the accept socket to the read list. Having a new connection arrive on this socket makes it available for reading, it’s just that we use a strange kind of read, the accept call, to do the read.

c. Call select with the read list. Your program will now block until one of the sockets on the read list is ready to be read.

d. For each socket on the read list that select has marked readable, do the following:

i. If it is the accept socket, accept the new connection and add it to the list of open connections with the appropriate state

ii. If it some other socket, performs steps 4.b through 4.f from the description of http_server1 in Part 2.  After closing the socket, delete it from the list of open connections.

Test your server using telnet and a web browser as described above, to see whether it really handles two simultaneous connections.

Extra Credit: complex multi-connection web server
This part is worth 10% of the total grade, if completed correctly

I do not think it is possible to do the extra credit (as described below) on a Windows machine, since Windows has a different mechanism for asynchronous I/O on files.  Windows users should use a Wilkinson lab machine or murphy.

http_server2 can handle multiple connections at a time, but there remain a number of places where it can block. These are implicit in the assumptions we have made. In general, almost any system call can block. In particular, if select tells us that a file descriptor is readable, it only means that at least one byte can be read. Reading any subsequent byte may block. The same holds true for writes.

To avoid unnecessary blocking, the program must check each system call that may block, and certainly read and write, before it executes the system call. Does this mean that we have to call select before we read or write each byte? Not necessarily. We can instead use non-blocking I/O. If we set a file descriptor to operate in non-blocking mode, then system calls on that file descriptor will fail with an "EAGAIN" error instead of blocking. EAGAIN means “I can’t do that right now because doing so would block you and you asked me never to let that happen.” To read more about non-blocking I/O, see the man page for "fcntl" (and https://docs.python.org/2/library/fcntl.html (Links to an external site.)Links to an external site. if you are using python).  fcntl(fd,F_SETFL,O_NONBLOCK)" is a way to set a file descriptor to non-blocking I/O using a classic aystem call, "fcntl.fcntl(fd,fcntl.F_SETFL, os.O_NONBLOCK)" in the case of Python. To learn how to retrieve error codes from system calls, check out the man page for "errno", or "try/except" code blocks for Python.

For extra credit, you can build an HTTP server, http_server3, which uses select and non-blocking I/O to provide availability even in the face of blocking on any of the reads, writes, and accepts, as well as dealing with partial reads and writes. The overall structure of your code should be as follows:

Create a TCP socket on which to listen for new connections

Bind that socket to the port provided on the command line.

Listen on that socket, which we shall call the "accept socket."

Initialize the list of open connections to empty

Do the following repeatedly:

a. Make a list of file descriptors we are waiting to read from the list of open connections. This will include both sockets and file descriptors for files you are in the process of reading. We shall call this the "read list".

b. Add the accept socket to the read list.

c. Make a list of sockets we are waiting to write from the list of open connections. We shall call this the write list.

d. Call select with the read list and the write list. Your program will now block until one of the sockets on the read list is ready to be read or written.

e. For each socket on the read list that select has marked readable, do the following:

i. If it is the accept socket, accept the new connection and add it to the list of open connections with the appropriate state.

ii. If it’s some other socket, look up its connection in the list of open connections, figure out how much you have left to read, and then read until you get an EAGAIN or you’ve read the whole request.

1. If you get the EAGAIN, update the connection’s state accordingly.

2. If you’ve read the whole request, open the file, set its file descriptor to non-blocking, add it to the connection state, and update the state to note that you’re in the process of reading the file.

iii. If it’s some other file descriptor, look up its connection in the list of open connections, figure out how much you have left to read, and then read until you get an EAGAIN or you’ve read the whole file.

1. If you get the EAGAIN, update the connection state toreflect how much you have read.

2. If you’ve read the whole file, close the file, update the connection state to reflect that you are ready to start writing the contents to the socket.

f. For each socket on the write list that select has marked writeable do the following.

i. Look up its connection in the list of open connections, figure out how much you have left to write, and then write until you get an EAGAIN or you’ve read the whole request.

1. If you get the EAGAIN, update the connection state to reflect how much you’ve written.

2. If you’ve written the whole file, close the socket and remove the connection from the list of open connections. 


Submission
You should work in pairs.  List the participants (names and netids) in a README.txt file.
Your code must compile and run on murphy.wot.eecs.northwestern.edu.
Include a makefile to compile the code.  We expect that running "make" will produce outputs "http_client", "http_server1", "http_server2".
If you're using python, then you can skip the makefile and just name your source files "http_client.py", "http_server1.py", etc.
You may not use any outside libraries to in your code.
Your submission should be a tgz file, including a README.txt, any source code files (not object files or executables), and a makefile (if using C or C++).  Use a command like the following
tar -czvf project1_NETID.tgz README.txt *.py
Remember that you should leave a comment explaining any bits of code that you copied from the Internet (just in case two groups copy the same code from the Internet, we won't think that you copied each other directly).
After you make your tarball, copy it to a temporary folder, extract it, compile it, and test it again to make sure you included everything.
Sharing files with your partner using git
I recommend that you create a git repository for your code and use Bitbucket (Links to an external site.)Links to an external site. to share the repository with your partner.  Each of you will be able to work on your own copy of the code, and periodically push updates to the central repository.  I recommend Bitbucket instead of Github because private repositories are free on Bitbucket (and it would be a violation of our cheating policy to post your code on a public repository).