# import RPi.GPIO as GPIO
import time
import reader
import encode

def setup():
    # assigning the pin mode to the way that they are
    # displayed on a raspberry pi board, regardless of
    # the specific broadcom chip that is used in a raspi model
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    # assign 6 different GPIO pins to output
    pins = [14, 23, 25, 12, 16, 17]
    pwm = []
    for i in range(0, len(pins)-1):
        GPIO.setup(pin[i], GPIO.OUT)
        # setup pwm object for each pin, and set duty cycle to 50% for square wave
        pwm[i] = GPIO.PWM(pin[i], 400)
        pwm[i].dc(50)

def run_epoch(pins, data):
    return 1

# string, beginning index
s = reader.Stream("eat on it", 0)
# speed in seconds, length (number of devices), step iterator -- currently broken just use 1
r = reader.Reader(2, 6, 1)
d = encode.gen_data(s.string)
encode.disp_data(d)
 
#setup()
print(range(1,4))  
while 1:
    status = r.read(s)
    for i in range(1, 4)
    time.sleep(r.rate)
    if status == 0:
        break
