import RPi.GPIO as GPIO
import time

def setup():
    # assigning the pin mode to the way that they are
    # displayed on a raspberry pi board, regardless of
    # the specific broadcom chip that is used in a raspi model
    # THINK ABOUT DUTY CYCLE WHATEVER THAT IS
    GPIO.setmode(GPIO.BOARD)
    GPIO.setwarnings(False)
    # assign 6 different GPIO pins to output
    pins = [14, 23, 25, 12, 16, 17]
    pwm = []
    for i in range(0, len(pins)-1):
        GPIO.setup(pin[i], GPIO.OUT)
        # start each motor at 10 hz
        pwm[i] = GPIO.PWM(pin[i], 100)

setup()
