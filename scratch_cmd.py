import json
import sys

def main():
    try:
        with open(r'f:\Projects\API_ESPNOW\build\compile_commands.json', 'r') as f:
            data = json.load(f)
            
        for cmd in data:
            if 'espnow_control.c' in cmd['file']:
                print(cmd['command'])
                return
        print("Not found")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == '__main__':
    main()
