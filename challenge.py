import time

start_time = time.time()


def encryption(input_file, integer_key):
    with open(input_file, "r") as input:
        message = input.read()

    encrypted_message = []

    for index, char in enumerate(message):
        ascii_value = ord(char)
        encrypted_message.append(format((ascii_value + index + integer_key), "08b"))

    return encrypted_message


def decryption(encrypted_message, integer_key):
    decrypted_message = []

    for index, char in enumerate(encrypted_message):
        value = int(char, 2)
        ascii_value = value - index - integer_key
        decrypted_message.append(chr(ascii_value))

    return "".join(decrypted_message)


encrypted_message = encryption("input.txt", 3)
print(encrypted_message)
message = decryption(encrypted_message, 3)
print(message)

end_time = time.time()
print(f"{end_time - start_time} seconds")
