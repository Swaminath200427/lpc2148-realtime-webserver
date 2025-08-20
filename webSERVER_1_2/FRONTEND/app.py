import os
import sys
import platform
import struct
import ctypes
from ctypes import c_void_p, WINFUNCTYPE, CFUNCTYPE
from flask import Flask, render_template, jsonify
import mysql.connector
import requests
import json
import threading    
import time

app = Flask(__name__)

# Print initial information for debugging
print(f"Python version: {sys.version}")
print(f"Architecture: {platform.architecture()}")
print(f"Pointer size: {struct.calcsize('P') * 8} bits")

def load_library():
    lib_name = 'libmylibrary.dll' if sys.platform.startswith('win') else 'libmylibrary.so'
    lib_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), lib_name)
    #lib_path = ctypes.util.find_library(lib_name)

    print(f"Attempting to load library from: {lib_path}")
    print(f"Python Architecture: {platform.architecture()[0]}")

    if not os.path.exists(lib_path):
        print(f"Error: The library file {lib_path} does not exist.")
        return None

    try:
        # Force using WinDLL for Windows
        lib = ctypes.WinDLL(lib_path) if sys.platform.startswith('win') else ctypes.CDLL(lib_path)
        
        # Define function signatures with appropriate calling conventions
        lib.ENC28J60_init.argtypes = []
        lib.ENC28J60_init.restype = None

        lib.generate_random.argtypes = [ctypes.c_double, ctypes.c_double]
        lib.generate_random.restype = ctypes.c_double

        lib.sensor_data_to_json.argtypes = [ctypes.c_void_p]
        lib.sensor_data_to_json.restype = ctypes.c_char_p
        
        lib.Getdata.argtypes = [ctypes.c_void_p]
        lib.Getdata.restype = None

        lib.set_ip_address.argtypes = [ctypes.c_char_p]
        lib.set_ip_address.restype = None

        # Initialize the ENC28J60 Ethernet module
        print("Initializing ENC28J60...")
        try:
            lib.ENC28J60_init()
        except Exception as e:
            print(f"Error calling ENC28J60_init: {e}")

        return lib
    except Exception as e:
        print(f"Error loading shared library: {e}")
        print("System details:")
        print(f"  Operating System: {sys.platform}")
        print(f"  Python version: {sys.version}")
        print(f"  Python architecture: {8 * ctypes.sizeof(ctypes.c_voidp)} bit")
        return None

lib = load_library()
if lib is None:
    print("Warning: Failed to load the sensor data library. Some functionality may be limited.")
else:
    print("Successfully loaded the sensor data library.")
    
    # Set IP address
    lib.set_ip_address(b"192.168.1.100")

# Configuration
MONGOOSE_SERVER = "http://192.168.1.100"  # LPC2148 IP address
POLL_INTERVAL = 2  # seconds between data fetches

# Database configuration
DB_CONFIG = {
    "host": "localhost",
    "port": "3306",
    "user": "root",
    "password": "Rishi@2003",
    "database": "sensor_data_db"
}

def connect_to_database():
    try:
        conn = mysql.connector.connect(**DB_CONFIG)
        if conn.is_connected():
            return conn
        return None
    except mysql.connector.Error as e:
        print(f"Database connection error: {e}")
        return None

def get_sensor_data_from_c():
    if lib is None:
        return None
    
    class SensorData(ctypes.Structure):
        fields = [("temperature", ctypes.c_double),
                    ("humidity", ctypes.c_double),
                    ("pressure", ctypes.c_double),
                    ("light", ctypes.c_double)]
    
    sensor_data = SensorData()
    lib.Getdata(ctypes.byref(sensor_data))
    
    json_data = lib.sensor_data_to_json(ctypes.byref(sensor_data))
    if json_data:
        return json.loads(json_data.decode('utf-8'))
    return None

def store_sensor_data(sensor_data):
    """Store sensor data received from Mongoose server"""
    conn = connect_to_database()
    if conn is None:
        return False

    try:
        cursor = conn.cursor()
        query = """INSERT INTO SensorData 
                  (temperature, humidity, pressure, light, timestamp) 
                  VALUES (%s, %s, %s, %s, %s)"""
        
        values = (
            sensor_data.get('temperature', 0.0),
            sensor_data.get('humidity', 0.0),
            sensor_data.get('pressure', 0.0),
            sensor_data.get('light', 0.0),
            time.strftime('%Y-%m-%d %H:%M:%S')
        )
        
        cursor.execute(query, values)
        conn.commit()
        return True

    except mysql.connector.Error as e:
        print(f"Error storing data: {e}")
        return False

    finally:
        if cursor:
            cursor.close()
        if conn and conn.is_connected():
            conn.close()

def fetch_mongoose_data():
    """Fetch data from Mongoose server"""
    try:
        response = requests.get(f"{MONGOOSE_SERVER}/api/sensor")
        if response.status_code == 200:
            return response.json()
        else:
            print(f"Error fetching data: {response.status_code}")
            return None
    except requests.RequestException as e:
        print(f"Request error: {e}")
        return None

def background_data_collection():
    """Background thread to continuously fetch and store data"""
    while True:
        try:
            # Fetch data from Mongoose
            sensor_data = get_sensor_data_from_c()
            if sensor_data:
                # Store in database
                store_sensor_data(sensor_data)
            
        except Exception as e:
            print(f"Background task error: {e}")
        
        time.sleep(POLL_INTERVAL)

# Start background data collection
data_thread = threading.Thread(target=background_data_collection, daemon=True)
data_thread.start()

@app.route('/')
def home():
    conn = connect_to_database()
    if conn is None:
        return "Failed to connect to the database."

    try:
        cursor = conn.cursor()
        cursor.execute('''SELECT id, temperature, humidity, pressure, light, timestamp 
                          FROM SensorData 
                          ORDER BY timestamp DESC 
                          LIMIT 10''')
        rows = cursor.fetchall()

        data = [{
            'id': row[0],
            'temperature': row[1],
            'humidity': row[2],
            'pressure': row[3],
            'light': row[4],
            'timestamp': row[5]
        } for row in rows]

        # Get real-time data from C library
        current_data = get_sensor_data_from_c()

        return render_template('index.html', 
                               historical_data=data,
                               current_data=current_data)

    except mysql.connector.Error as e:
        return f"Error fetching data: {e}"

    finally:
        if cursor:
            cursor.close()
        if conn and conn.is_connected():
            conn.close()

@app.route('/api/current')
def get_current_data():
    """API endpoint to get current sensor data"""
    data = get_sensor_data_from_c()
    if data:
        return jsonify(data)
    return jsonify({'error': 'Failed to fetch data'}), 500

@app.route('/api/historical')
def get_historical_data():
    """API endpoint to get historical sensor data"""
    conn = connect_to_database()
    if conn is None:
        return jsonify({'error': 'Database connection failed'}), 500

    try:
        cursor = conn.cursor()
        cursor.execute('''SELECT id, temperature, humidity, pressure, light, timestamp 
                          FROM SensorData 
                          ORDER BY timestamp DESC 
                          LIMIT 100''')
        rows = cursor.fetchall()

        data = [{
            'id': row[0],
            'temperature': row[1],
            'humidity': row[2],
            'pressure': row[3],
            'light': row[4],
            'timestamp': row[5].isoformat()
        } for row in rows]

        return jsonify(data)

    except mysql.connector.Error as e:
        return jsonify({'error': str(e)}), 500

    finally:
        if cursor:
            cursor.close()
        if conn and conn.is_connected():
            conn.close()

if __name__ == "_main_":
    app.run(debug=True)