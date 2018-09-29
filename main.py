# import RPi.GPIO as GPIO
import time
import reader
import encode
import math

def setup():
    # assigning the pin mode to the way that they are
    # displayed on a raspberry pi board, regardless of
    # the specific broadcom chip that is used in a raspi model
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    # assign 6 different GPIO pins to output
    # pins = [14, 23, 25, 12, 16, 17]
    pwm = []
    for i in range(0, len(pins)):
        GPIO.setup(pin[i], GPIO.OUT)
        # setup pwm object for each pin, and set duty cycle to 50% for square wave
        pwm[i] = GPIO.PWM(pin[i], 400)
        pwm[i].dc(50)

def run_epoch(pins, data):
    return 1

pins = [14]
# string, beginning index
s = reader.Stream("aaaaaaaaaa", 0)
# speed in seconds, length (number of devices), step iterator -- currently broken just use 1
r = reader.Reader(2, 6, 1)

samples = (r.rate)*2500

while 1:
    status = r.read(s)
    chunk = s.string[s.index:s.index+r.length]
    d = encode.gen_data(chunk, samples)
    for state in range(0, samples*4):
        for i in range(0, len(pins)):
            GPIO.PWM(pins[i], d[state+samples*4*i])
            time.sleep(10**6)
    if status == 0:
        break
