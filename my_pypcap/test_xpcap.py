
import xpcap
import ctypes

xpcap.init('/Users/tkorays/MyLibs/lib/libpcap.1.7.4.dylib')

# lib version
print("LibVersion:", xpcap.lib_version())

# find all devs
alldev = xpcap.findalldevs()
dev = alldev
while dev:
    print(dev.name)
    dev = dev.get_next()

print("freealldevs: ", xpcap.freealldevs(alldev))


def callback(userdata, hdr, sp):
    print("len:", hdr.contents.caplen)

pcap = None
pcap = xpcap.create(b"en0")
print("pcap:", pcap)
#pcap = xpcap.open_offline(b"/Users/tkorays/Desktop/hehe.pcap")

print("set_snaplen:", pcap.set_snaplen(65535))
print("SetImmediateMode:", pcap.set_immediate_mode(1))
print("SetTimeOut:", pcap.set_timeout(5000))
print("Activate:", pcap.activate())

data = ctypes.c_char()

i = 0
while i<10:
    ret = pcap.dispatch(-1, callback, data)
    print(xpcap.strerror(ret))
    i += 1

print(pcap)
print("major version:", xpcap.major_version(pcap))
print("major version:", xpcap.minor_version(pcap))

print("close:", pcap.close())

