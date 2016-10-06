from ctypes import *


class c_pcap_pkthdr(Structure):
    _fields_ = [('ts_1', c_uint32),
                ('ts_2', c_uint32),
                ('caplen', c_uint32),
                ('len', c_uint32)]


class pcap_data:
    _fields_ = [('data', c_char_p)]


__pcap_instance__ = None
__err__ = create_string_buffer(b'\0'*128)


class c_pcap(Structure):

    def activate(self):
        global __pcap_instance__
        if not __pcap_instance__:
            return -1
        func_activate = __pcap_instance__.pcap_activate
        func_activate.argtype = [POINTER(c_pcap)]
        func_activate.restype = c_int
        return func_activate(byref(self))

    def set_snaplen(self, len):
        global  __pcap_instance__
        if not __pcap_instance__:
            return -1
        func_set_snaplen = __pcap_instance__.pcap_set_snaplen
        func_set_snaplen.argtype = [POINTER(c_pcap), c_int]
        func_set_snaplen.restype = c_int
        return func_set_snaplen(byref(self), len)

    def set_immediate_mode(self, mode):
        global __pcap_instance__
        if not __pcap_instance__:
            return -1
        if mode != 0:
            mode = 1
        func_set_immediate_mode = __pcap_instance__.pcap_set_immediate_mode
        func_set_immediate_mode.argtype = [POINTER(c_pcap), c_int]
        func_set_immediate_mode.restype = c_int
        return func_set_immediate_mode(byref(self), mode)

    def set_timeout(self, timeout):
        global __pcap_instance__
        if not __pcap_instance__:
            return -1
        func_set_timeout = __pcap_instance__.pcap_set_timeout
        func_set_timeout.argtype = [POINTER(c_pcap), c_int]
        func_set_timeout.restype = c_int
        return func_set_timeout(byref(self), timeout)

    def dispatch(self, cnt, callback, dt):
        global __pcap_instance__
        if not __pcap_instance__:
            return -1
        func_dispatch = __pcap_instance__.pcap_dispatch
        func_dispatch.argtype = [POINTER(c_pcap), c_int, callback, c_char_p]
        func_dispatch.restype = c_int
        FUNC_CB = CFUNCTYPE(c_voidp, c_char_p, POINTER(c_pcap_pkthdr), c_char_p)
        return func_dispatch(byref(self), cnt, FUNC_CB(callback), dt)

    def close(self):
        global __pcap_instance__
        if not __pcap_instance__:
            return False
        func_close = __pcap_instance__.pcap_close
        func_close.argtype = [POINTER(c_pcap)]
        func_close(byref(self))
        return True


class c_pcap_if(Structure):
    """
    pcap_if_t in c
    """
    def get_next(self):
        if self.next:
            return self.next.contents
        return None


class c_pcap_addr(Structure):
    """
    pcap_addr in c
    """
    def get_next(self):
        if self.next:
            return self.next.contents
        return None


class c_sock_addr(Structure):
    """
    sock_addr in c
    """
    pass


c_pcap_if._fields_ = [('next', POINTER(c_pcap_if)),
                  ('name', c_char_p),
                  ('description', c_char_p),
                  ('addresses', POINTER(c_pcap_addr)),
                  ('flags', c_int32)]

c_pcap_addr._fields_ = [('next', POINTER(c_pcap_addr)),
                    ('addr', POINTER(c_sock_addr)),
                    ('netmask', POINTER(c_sock_addr)),
                    ('broadaddr', POINTER(c_sock_addr)),
                    ('dstaddr', POINTER(c_sock_addr))]

c_sock_addr._fields_ = [('sin_family', c_ushort),
               ('sin_port', c_ushort),
               ('sin_addr', c_int64),
               ('sin_zero', c_char*8)]




def init(dllname):
    global  __pcap_instance__
    __pcap_instance__ = cdll.LoadLibrary(dllname)
    if not __pcap_instance__:
        print("Fail to load dll: %s" % dllname)


def lib_version():
    global __pcap_instance__
    if not __pcap_instance__:
        return ""
    func_version = __pcap_instance__.pcap_lib_version
    func_version.restype = c_char_p
    return func_version()

def major_version(pcap):
    global __pcap_instance__
    if not __pcap_instance__ or not isinstance(pcap, c_pcap):
        return -1
    func_major_version = __pcap_instance__.pcap_major_version
    func_major_version.argtype = [POINTER(c_pcap)]
    func_major_version.restype = c_int
    return func_major_version(pointer(pcap))


def minor_version(pcap):
    global __pcap_instance__
    if not __pcap_instance__ or not isinstance(pcap, c_pcap):
        return -1
    func_minor_version = __pcap_instance__.pcap_minor_version
    func_minor_version.argtype = [POINTER(c_pcap)]
    func_minor_version.restype = c_int
    return func_minor_version(pointer(pcap))


def findalldevs():
    global __pcap_instance__
    global __err__
    if not __pcap_instance__:
        return None
    func_findalldevs = __pcap_instance__.pcap_findalldevs
    func_findalldevs.argtype = [POINTER(POINTER(c_pcap_if)), c_char_p]
    func_findalldevs.restype = c_int
    alldevs = POINTER(c_pcap_if)()
    if func_findalldevs(byref(alldevs), __err__) != 0:
        return None
    return alldevs.contents


def freealldevs(dev):
        global __pcap_instance__
        if not __pcap_instance__ or not isinstance(dev, c_pcap_if):
            return False
        func_freealldevs = __pcap_instance__.pcap_freealldevs
        func_freealldevs.argtype = [POINTER(c_pcap_if)]
        func_freealldevs(pointer(dev))
        return True


def create(adp_name):
    global __pcap_instance__
    global __err__
    if not __pcap_instance__:
        return None
    func_create = __pcap_instance__.pcap_create
    func_create.argtype = [c_char_p, c_char_p]
    func_create.restype = POINTER(c_pcap)
    p = func_create(adp_name, __err__)
    return p.contents



def open_offline(filename):
    global __pcap_instance__
    if not __pcap_instance__:
        return None
    func_open_offline = __pcap_instance__.pcap_open_offline
    func_open_offline.argtype = [c_char_p, c_char_p]
    func_open_offline.restype = POINTER(c_pcap)
    ret = func_open_offline(filename, __err__)
    if not ret:
        return None
    return ret.contents


def strerror(n):
    err = ["generic error code",
           "loop terminated by pcap_breakloop",
           "the capture needs to be activated",
           "the operation can't be performed on already activated captures",
           "no such device exists",
           "this device doesn't support rfmon (monitor) mode",
           "operation supported only in monitor mode",
           "no permission to open the device",
           "interface isn't up",
           "this device doesn't support setting the time stamp type",
           "you don't have permission to capture in promiscuous mode",
           "the requested time stamp precision is not supported"
           ]
    suc = ["ok",
           "generic warning code",
           "this device doesn't support promiscuous mode",
           "the requested time stamp type is not supported"]

    if n < 0:
        return err[abs(n)+1]
    else:
        return suc[n]
