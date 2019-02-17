#!/usr/bin/python

import sys
import ctypes
import errno
from ctypes import *
from ctypes.util import find_library
import random
import struct

#print(hid.hid_init())

class hid_device(Structure):
    pass

class hid:
    def __init__(self,vid = 0, pid = 0, sn = None):
        self.vid = vid
        self.pid = pid
        self.sn = sn
        self.handle = None
        self.hid = cdll.LoadLibrary(find_library('hidapi'))
        self.hid.hid_init()
        self.hid.hid_open.restype = POINTER(hid_device)
    def page_write(self,page,data):
        self.ss = struct.pack('4B32B',0x1,0,page,0,*data)
        for i in range(3):
            if not self.handle:
                self.handle = self.hid.hid_open(self.vid, self.pid, self.sn)
            if self.handle and (self.hid.hid_write(self.handle, self.ss, 36)  != 36):
                self.handle = None
                continue
            break
    def write(self,data):
        pages = len(data)/32
        for page in range(pages):
            self.page_write(page, data[page*32:page*32+32])

