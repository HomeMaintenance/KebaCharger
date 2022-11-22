from pymodbus.client.sync import ModbusTcpClient as ModbusClient
from time import sleep


def main():
    client = ModbusClient("192.168.178.88")
    if not client.connect():
        return
    print("connected")

#  5014
    register_number = 1000
    n_registers = 2
    unit_id = 255

    response = client.read_holding_registers(
        register_number, count=n_registers, unit=unit_id)
    print(f"response: {response}")

    while True:
        response = client.read_holding_registers(
            register_number, count=n_registers, unit=unit_id)

        if response.registers[1] == 3:
            response2 = client.write_register(5014, value=0, unit=unit_id)
            seconds = 0
            set_state = False
            while(set_state == False):
                response3 = client.read_holding_registers(
                    register_number, count=n_registers, unit=unit_id)
                set_state = response3.registers[1] == 5
                if(set_state == False):
                    sleep(0.1)
                    seconds = seconds + 0.1
            print(f"turn off seconds: {seconds}")

        if response.registers[1] == 5:
            response4 = client.write_register(5014, value=1, unit=unit_id)
            seconds = 0
            set_state = False
            while(set_state == False):
                response5 = client.read_holding_registers(
                    register_number, count=n_registers, unit=unit_id)
                set_state = response5.registers[1] == 3
                if(set_state == False):
                    sleep(0.1)
                    seconds = seconds + 0.1
            print(f"turn on seconds: {seconds}")

    pass


if __name__ == "__main__":
    main()
