# https://www.makerguides.com/a4988-stepper-motor-driver-arduino-tutorial/

import time
import RPi.GPIO as GPIO # pip install rpimotorlib
from RpiMotorLib import RpiMotorLib

GPIO_pins = (0, 0, 0) # use 1/16 of step
dir_pin = 15
step_pin = 14

test_motor = RpiMotorLib(direction, step, GPIO_pins, "A4988")

def main():
    test_motor.motor_go(False, "1/16", 3200, .05, False, .05)
    time.sleep(1)
    test_motor.motor_go(True, "1/16", 3200, .05, False, .05)

if __name__ == "__main__":
    main()
    GPIO.cleanup()
