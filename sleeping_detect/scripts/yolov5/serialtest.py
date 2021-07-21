import serial
import time

# 'COM3' 부분에 환경에 맞는 포트 입력
ser = serial.Serial('/dev/ttyUSB0', 9600)

while True:
    if ser.readable():
        val = '1'
        val = val.encode('utf-8')
        ser.write(val)
        time.sleep(0.5)


       
