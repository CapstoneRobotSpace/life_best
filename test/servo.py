import RPi.GPIO as GPIO
import time
pin = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(pin,GPIO.OUT)
p = GPIO.PWM(pin,50)
p.start(0)

try:
  print('DutyCycle : 2')  
  p.ChangeDutyCycle(2)
  time.sleep(7)
  print('DutyCycle : 7.5')
  p.ChangeDutyCycle(7)
  time.sleep(7)
  while True:
    print('DutyCycle : 10')    
    p.ChangeDutyCycle(10)
    time.sleep(2)
    print('DutyCycle : 7.5')
    p.ChangeDutyCycle(7.5)
    time.sleep(2)
    print('DutyCycle : 5')
    p.ChangeDutyCycle(5)
    time.sleep(2)

except KeyboardInterrupt:
  p.stop()
GPIO.cleanup()
