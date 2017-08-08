#!/usr/bin/python -u
# -*- coding: utf-8 -*-

import smbus
import time

address = 0x68
channel = 1
bus     = smbus.SMBus(channel)

# PWR_MGMT_1をクリア
bus.write_i2c_block_data(address, 0x6B, [0x00])
time.sleep(0.1)

# I2Cで磁気センサ機能(AK8963)へアクセスできるようにする(BYPASS_EN=1)
bus.write_i2c_block_data(address, 0x37, [0x02])
time.sleep(0.1)
