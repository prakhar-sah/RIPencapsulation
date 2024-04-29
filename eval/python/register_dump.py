import sys
import serial
from openpyxl import Workbook

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
        dump[r][c // 2] = int(temp_h, 16)

# Write dump_h data to Excel file
wb = Workbook()
ws1 = wb.active
ws1.title = "hex"
ws2 = wb.create_sheet("int")

for row in dump_h:
    ws1.append(row)
    
for row in dump:
    ws2.append(row)

# Save the Excel file
wb.save("../dumps/"+sys.argv[2])

