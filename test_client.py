import socket
import time

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect( ("localhost", 8765) )

# for i in range(64):
# 	client.send("STORE abc%d.txt 4100\n%s"%(i,"DATA"*1025))
# 	# client.send()
# 	print client.recv( 1024 )
# 	# client.send("DIR\n")
# 	# print client.recv( 1024 )
# 	client.send("READ abc%d.txt 0 16\n"%i)
# 	print client.recv( 1024 )

# for i in range(64):
# 	client.send("DELETE abc%d.txt\n"%i)
# 	print client.recv( 1024 )

# client.send("STORE small.txt 98304\n")
# client.send("DATA"*24576)
# print client.recv( 1024 )

# client.send("STORE medium.txt 196608\n")
# client.send("DATA"*49152)
# print client.recv( 1024 )

# client.send("DELETE small.txt\n")
# print client.recv( 1024 )

# client.send("STORE medium2.txt 196608\n")
# client.send("DATA"*49152)
# print client.recv( 1024 )

# # client.send("READ medium2.txt 4096 192512\n")
# # print client.recv( 1024 )

# client.send("DELETE medium.txt\n")
# print client.recv( 1024 )

# client.send("DELETE medium2.txt\n")
# print client.recv( 1024 )

# raw_input("<enter>")

# for i in range(64):
# 	client.send("STORE abc%d.txt 4100\n%s"%(i,"DATA"*1025))
# 	# client.send()
# 	print client.recv( 1024 )
# 	# client.send("DIR\n")
# 	# print client.recv( 1024 )
# 	# client.send("READ abc%d.txt 0 16\n"%i)
# 	# print client.recv( 1024 )

# for i in range(64):
# 	if i % 2 == 0:
# 		client.send("DELETE abc%d.txt\n"%i)
# 		print client.recv( 1024 )


# client.send("STORE large.txt 262144\n")
# client.send("DATA"*65536)
# print client.recv( 1024 )

# raw_input("<enter>")

# for i in range(64):
# 	if i % 2 == 1:
# 		client.send("DELETE abc%d.txt\n"%i)
# 		print client.recv( 1024 )

# # client.send("DELETE large.txt\n")

# # client.send("STORE bad.txt 8\nlol\0bad")
# # print client.recv( 1024 )


f = open("img.png", "r")
contents = f.read()
client.send("STORE img.png %d\n"%(len(contents)))
client.send(contents)
print client.recv( 1024 )

client.send("READ img.png 0 %d\n"%(len(contents)))
img_back_contents = client.recv( len(contents)*2 )
fout = open("img_back.png", "w")
fout.write(img_back_contents)





