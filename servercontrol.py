from flask import Flask, request, jsonify
import serial


serial_port = '/dev/cu.usbserial-14430'
baud_rate = 9600
ser = None

app = Flask(__name__)


ser = serial.Serial(serial_port, baud_rate)

@app.route('/move', methods=['POST'])
def move_robot():
    data = request.json

    # Extract X, Y, and theta values from the JSON data
    x = data.get('x', 0.0)
    y = data.get('y', 0.0)
    theta = data.get('theta', 0.0)

    
    data_to_send = (f"{x} {y} {theta}\n")

    
    ser.write(data_to_send.encode('utf-8'))

    return jsonify({'message': 'Movement command sent'})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
