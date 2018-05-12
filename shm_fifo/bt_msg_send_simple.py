#!/usr/bin/python
import ctypes
from ctypes import *

if __name__ == "__main__":

	func = ctypes.cdll.LoadLibrary("./bluetooth_proxy.so")
	func.bluetooth_proxy_init()
	data = bytes('hello, world')
	func.bluetooth_print(data, len(data))
