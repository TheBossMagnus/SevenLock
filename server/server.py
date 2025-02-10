import csv

passwordsFIle = "passwords.csv"
import serial
import time

def main():
    ser = serial.Serial('/dev/ttyS0', 9600, timeout=1)
    while True:
        with open(passwordsFIle, 'r', newline='') as f:
            reader = csv.reader(f)
            for row in reader:
                if row:
                    ser.write(row[0].encode('utf-8'))
        time.sleep(5)

if __name__ == '__main__':
    main()