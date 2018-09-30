# import RPi.GPIO as GPIO
import time
import reader
import encode
import math

def setup(pins):
    # assigning the pin mode to the way that they are
    # displayed on a raspberry pi board, regardless of
    # the specific broadcom chip that is used in a raspi model
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    # assign 6 different GPIO pins to output
    # pins = [14, 23, 25, 12, 16, 17]
    pwms = []
    for i in range(0, len(pins)):
        GPIO.setup(pins[i], GPIO.OUT)
        # setup pwm object for each pin, and set duty cycle to 50% for square wave
        # test hz
        pwms[i] = GPIO.PWM(pins[i], 100)
        pwms[i].start(50)
    return pwms

pins = [14]

# string, beginning index
s = reader.Stream("hello_world", 0)

# speed in seconds, length (number of devices), step iterator -- currently broken just use 1
r = reader.Reader(2, 1, 1)

samples = (r.rate)*2500

d = encode.gen_data(s.string, samples)
encode.disp_data(d)

pwms = setup(pins)
time.sleep(1)
for pwm in pwms:
    pwm.stop()
# run the code for an individual vibration speaker
while 1:
    status = r.read(s)
    pwms[0].start(50)
    for state in d:
        pwms[0].ChangeFrequency(d[state])
        time.sleep(10**-6)
    if status == 0:
        break
