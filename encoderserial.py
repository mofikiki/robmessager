import serial

# Define the serial port and baud rate (make sure to match the Arduino settings)
serial_port = '/dev/cu.usbserial-14430'  # Replace with the appropriate serial port
baud_rate = 9600

# Open the serial port
ser = serial.Serial(serial_port, baud_rate)

try:
    while True:
        # Read data from the Arduino
        line = ser.readline().decode().strip()

        # Check if the received line contains encoder data
        if line.startswith("RIGHT"):
            # Split the line to extract the counter values for RIGHT and LEFT
            parts = line.split("\t")
            if len(parts) == 6:
                counter1 = int(parts[1].split(":")[1])
                counter2 = int(parts[4].split(":")[1])

                # Display the counter values
                print(f"Counter 1: {counter1}")
                print(f"Counter 2: {counter2}")

except KeyboardInterrupt:
    print("Serial reading stopped.")

finally:
    # Close the serial port when done
    ser.close()
