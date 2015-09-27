import pyb


ESP8266_LED = 5

pin = pyb.Pin(ESP8266_LED, pyb.Pin.OUT_PP)

while True:
    pin.high()
    pyb.delay(1500)
    pin.low()
    pyb.delay(1500)
