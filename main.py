# import RPi.GPIO as GPIO
import time
import reader
import encode
import math

def setup():
    # assigning the pin mode to the way that they are
    # displayed on a raspberry pi board, regardless of
    # the specific broadcom chip that is used in a raspi model
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    # assign 6 different GPIO pins to output
    # pins = [14, 23, 25, 12, 16, 17]
    pins = [14]
    pwms = []
    for i in range(0, len(pins)):
        GPIO.setup(pins[i], GPIO.OUT)
        # setup pwm object for each pin, and set duty cycle to 50% for square wave
        # test hz
        pwms[i] = GPIO.PWM(pins[i], 400)
        pwms[i].start(50)

def run_epoch(pins, data):
    return 1

pins = [14]
# string, beginning index
s = reader.Stream("aaaaaaaaaa", 0)
# speed in seconds, length (number of devices), step iterator -- currently broken just use 1
r = reader.Reader(2, 1, 1)

samples = (r.rate)*2500

while 1:
    status = r.read(s)
    chunk = s.string[s.index:s.index+r.length]
    d = encode.gen_data(chunk, samples)
    pwms = []
    for state in range(0, samples*4):
        for i in range(0, len(pins)):
            pwms[i] = GPIO.PWM(pins[i], d[state+samples*4*i])
            pwms[i].start(50)
            time.sleep(10**-6)
    if status == 0:
        break
