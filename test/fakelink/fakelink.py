import socket
import json
import time
import sys

HOST = '0.0.0.0'

DATA = {
  "latitude": 49.269606,
  "longitude": -123.166990,
  "altitude_agl_meters": 30.0,
  "altitude_msl_meters": 90.0,
  "heading_degrees": 128.0,
  "timestamp_telem": 1567750839,
  "timestamp_msg": 1567750839,
}

def increaseMessage():
  for key, value in DATA.items():
    DATA[key] += 1

def main():
    PORT = int(sys.argv[1])

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)
            while True:
                conn.sendall(json.dumps(DATA).encode('UTF-8'))
                conn.sendall("\n".encode('UTF-8'))
                increaseMessage()
                time.sleep(1)

if __name__ == "__main__":
    main()