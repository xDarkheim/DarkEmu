import socket
import time
import binascii

# Settings
HOST = '127.0.0.1'
PORT = 44405

def main():
    print(f"Connecting to {HOST}:{PORT}...")
    
    try:
        # 1. Connect
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        print("Connected!")

        # 2. Create a package
        # C1 = Packet Head
        # 04 = Size (4 bits)
        # F4 = Packet Type (Connect Server Protocol)
        # 06 = Sub Type (Server List Request)
        packet = bytes.fromhex("C1 04 F4 06")
        
        print(f"Sending: {binascii.hexlify(packet)}")
        sock.sendall(packet)

        # 3. Wait answer
        print("Waiting for response...")
        data = sock.recv(1024)
        
        if data:
            print(f"Received: {binascii.hexlify(data)}")
            # If C2 00 F4 06 ... - IT'S WORK!
        else:
            print("Server closed connection without data.")

        sock.close()

    except ConnectionRefusedError:
        print("Error: Server is offline!")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
