import sys
import serial
from openpyxl import Workbook
import numpy as np

# Open serial port
device = serial.Serial('/dev/ttyACM1', 115200)

data = [[] for _ in range(int(sys.argv[1]))]

# Read data
for i in range(int(sys.argv[1])):
    data[i] = list(device.read(32))

device.close()

dump_h = [[''] * 16 for _ in range(int(sys.argv[1]))]
dump = [[0] * 16 for _ in range(int(sys.argv[1]))]

# Process data
for r in range(int(sys.argv[1])):
    for c in range(0, 32, 2):
        b1 = format(data[r][c], '02X')
        b2 = format(data[r][c + 1], '02X')
        temp_h = b1 + b2
        dump_h[r][c // 2] = temp_h

# Write dump_h data to Excel file
wb = Workbook()
ws = wb.active

for row in dump_h:
    A = row[int(sys.argv[2])]
    B = row[int(sys.argv[3])]
    if (sys.argv[4] == "b"):
        B = B[2:4]
    elif (sys.argv[4] == "w"):
        temp_B = B[2:4]
        B = temp_B + B[0:2]
    ws.append([A,B])

# Save the Excel file
wb.save("../dumps/exfiltrated_firmware.xlsx")

