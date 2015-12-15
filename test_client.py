import socket
import time

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect( ("localhost", 8765) )

# client.send("STORE abc.txt 15\n")
# client.send("This is abc.txt")

f = open("img.png", "r")
contents = f.read()
client.send("STORE img.png %d\n"%(len(contents)))
client.send(contents)

print client.recv( 1024 )

