import serial
from flask import Flask, request, jsonify, render_template

app = Flask(__name__)

# Configure the serial connection to your Arduino
serial_port_arduino = '/dev/cu.usbserial-14430'  # Replace with your Arduino's serial port
baud_rate = 9600

ser_arduino = serial.Serial(serial_port_arduino, baud_rate)

@app.route('/')
def home():
    return render_template("control.html")

@app.route('/move', methods=['POST'])
def move_robot():
    data = request.form

    # Extract X, Y, and theta values from the form data
    x = float(data['x'])
    y = float(data['y'])
    theta = float(data['theta'])

    # Format the data as a string for movement and send it to the Arduino
    data_to_send = f"{x} {y} {theta}\n"
    ser_arduino.write(data_to_send.encode('utf-8'))

    return jsonify({'message': 'Movement command sent'})

@app.route('/set_desired', methods=['POST'])
def set_desired_position():
    data = request.form

    # Extract desired position values (x, y, theta) from the form data
    x = float(data['x'])
    y = float(data['y'])
    theta = float(data['theta'])

    # Format the data as a string for setting the desired position and send it to the Arduino
    data_to_send = f"DESIRED: {x} {y} {theta}\n"
    ser_arduino.write(data_to_send.encode('utf-8'))

    return jsonify({'message': 'Desired position set'})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
