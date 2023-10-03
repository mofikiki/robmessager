import serial
# from flask import Flask, request, jsonify, render_template

# app = Flask(__name__)

# Configure the serial connection to your Arduino for movement commands
serial_port_arduino1 = '/dev/cu.usbserial-1430'  # Serial port for Arduino 1
serial_port_arduino2 = '/dev/cu.usbserial-14430'  # Serial port for Arduino 2
baud_rate = 9600

ser_arduino1 = serial.Serial(serial_port_arduino1, baud_rate)
ser_arduino2 = serial.Serial(serial_port_arduino2, baud_rate)

# Function to process received encoder data
def process_encoder_data(encoder_data_arduino1,data_arduino2):
    # Split the received encoder data into individual values
    values1 = encoder_data_arduino1.split(";")
    values2= data_arduino2.split(";")
    if len(values1) == 6:
        counter1 = int(values1[0])
        speed1 = float(values1[1])
        distance1 = float(values1[2])
        counter2 = int(values1[3])
        speed2 = float(values1[4])
        distance2 = float(values1[5])
        error =int (values2)
        # Process and use encoder data from Arduino 1 and Arduino 2 as needed
        print("ENCODER (Arduino 1):")
        print("Counter 1:", counter1)
        print("Speed 1 (rotations/s):", speed1)
        print("Distance 1 (m):", distance1)
        print("Counter 2:", counter2)
        print("Speed 2 (rotations/s):", speed2)
        print("Distance 2 (m):", distance2)
        
        print("Encoder (Arduino2):")
        print("error: ", error)

    else:
        print("Invalid encoder data format")

# @app.route('/move', methods=['POST'])
# def move_robot():
#     data = request.json

#     # Extract X, Y, and theta values from the JSON data
#     x = data.get('x', 0.0)
#     y = data.get('y', 0.0)
#     theta = data.get('theta', 0.0)

#     # Format the data as a string for movement and send it to the Arduino
#     data_to_send = f"{x} {y} {theta}\n"
#     ser_arduino2.write(data_to_send.encode('utf-8'))

#     return jsonify({'message': 'Movement command sent'})

# @app.route('/set_desired', methods=['POST'])
# def set_desired_position():
#     data = request.json

#     # Extract desired position values (x, y, theta) from the JSON data
#     x = data.get('x', 0.0)
#     y = data.get('y', 0.0)
#     theta = data.get('theta', 0.0)

#     # Format the data as a string for movement and send it to the Arduino
#     data_to_send = (f"{x} {y} {theta}\n")
#     ser_arduino1.write(data_to_send.encode('utf-8'))

#     return jsonify({'message': 'Desired position set'})

# Add a root route to display a welcome message
# @app.route('/')
# def home():
#     return render_template("control.html") 

# if __name__ == '__main__':
#     app.run(host='0.0.0.0', port=80)


try:
    while True:
        # Read incoming encoder data from the first Arduino (Arduino 1)
        encoder_data_arduino1 = ser_arduino1.readline().decode('utf-8').strip()

        # Read incoming data from the second Arduino (Arduino 2)
        data_arduino2 = ser_arduino2.readline().decode('utf-8').strip()

        # Process encoder data from Arduino 1
        if encoder_data_arduino1:
            process_encoder_data(encoder_data_arduino1)
        
        elif data_arduino2:
            process_encoder_data(data_arduino2)

        # Process data from Arduino 2 as needed
        # ...

except serial.SerialException as e:
    print("Serial port error:", e)

except Exception as e:
    print("An error occurred:", e)

finally:
    if ser_arduino1 and ser_arduino1.is_open:
        ser_arduino1.close()
    if ser_arduino2 and ser_arduino2.is_open:
        ser_arduino2.close()



