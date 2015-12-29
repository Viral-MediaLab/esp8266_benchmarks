import time
from serial import Serial


def millis():
    return int(round(time.time() * 1000))


class ESP8266():
    def __init__(self, port, baud_rate):
        print "opening connection"
        self.ser = Serial(port, baud_rate, timeout=1)
        self.ser.close()
        self.ser.open()
        print "connection opened"

    def send_command(self, cmd, timeout=1000, until="-a-string-thats-never-there-"):
        self.ser.write(cmd + "\r\n")
        last_activity = millis()
        buf = ""
        while True and (millis() - last_activity) < timeout and until not in buf:
            bytes_to_read = self.ser.inWaiting()
            buf += self.ser.read(bytes_to_read)
            if bytes_to_read > 0:
                last_activity = millis()
        return buf

    def make_http_request(self, host, port, route):
        start = millis()
        self.send_command("AT+CIPSTART=0,\"TCP\",\"" + host + "\"," + str(port), 500, "OK")
        self.send_command("AT+CIPSTATUS", 500, "OK")
        msg = "GET " + route + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n"
        self.send_command("AT+CIPSEND=0," + str(len(msg)), 500, "OK")
        result = self.send_command(msg, 10000, "+IPD,0,5:0")
        print result
        self.send_command("AT+CIPCLOSE=0", 500, ",CLOSED")
        print str(1 if "CC3000" in result else 0) + "," + str(millis() - start)


esp = ESP8266("/dev/tty.usbserial-FTH9KZ63", 115200)
esp.send_command("AT+RST", 5000, "WIFI GOT IP")
esp.send_command("AT+CIPMUX=1", 100)
esp.send_command("AT+CIFSR", 1000)
esp.make_http_request("www.adafruit.com", 80, "/testwifi/index.html")

# for i in range(300):
#     esp.make_http_request("www.adafruit.com", 80, "/testwifi/index.html")
#     time.sleep(1)
