"""Copyright (c) DarkEmu.
Manual client for requesting the server list from ConnectServer.
"""
# This script is intentionally simple for quick manual verification.
import socket
import time
import binascii

# Settings for the local ConnectServer instance.
HOST = '127.0.0.1'
PORT = 44405

def main():
    print(f"Connecting to {HOST}:{PORT}...")
    
    try:
        # 1. Create a TCP socket and connect to the ConnectServer.
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        print("Connected!")

        # 2. Build the server list request packet.
        # C1 = Packet Head
        # 04 = Size (4 bytes)
        # F4 = Packet Type (Connect Server Protocol)
        # 06 = Sub Type (Server List Request)
        packet = bytes.fromhex("C1 04 F4 06")
        
        print(f"Sending: {binascii.hexlify(packet)}")
        sock.sendall(packet)

        # 3. Read the response from the server.
        print("Waiting for response...")
        data = sock.recv(1024)
        
        if data:
            print(f"Received: {binascii.hexlify(data)}")
            # If the response begins with C2 .. F4 06, the server list arrived.
        else:
            print("Server closed connection without data.")

        sock.close()

    except ConnectionRefusedError:
        print("Error: Server is offline!")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
