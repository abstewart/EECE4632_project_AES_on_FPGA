from Crypto.Cipher import AES
from base64 import b64encode, b64decode

class AESCipher(object):
    def __init__(self, key):
        self.block_size = AES.block_size
        self.key = key

    def encrypt_text(self, plain_text):
        plain_text = self.__pad(plain_text)
        cipher = AES.new(self.key, AES.MODE_ECB)
        encrypted_text = cipher.encrypt(plain_text.encode())
        return b64encode(encrypted_text).decode("utf-8")

    def decrypt_text(self, encrypted_text):
        encrypted_text = b64decode(encrypted_text)
        cipher = AES.new(self.key, AES.MODE_ECB)
        plain_text = cipher.decrypt(encrypted_text).decode("utf-8")
        return self.__unpad(plain_text)

    def encrypt_bin(self, binary):
        cipher = AES.new(self.key, AES.MODE_ECB)
        encrypted_text = cipher.encrypt(binary)
        return encrypted_text

    def __pad(self, plain_text):
        number_of_bytes_to_pad = self.block_size - len(plain_text) % self.block_size
        ascii_string = chr(number_of_bytes_to_pad)
        padding_str = number_of_bytes_to_pad * ascii_string
        padded_plain_text = plain_text + padding_str
        return padded_plain_text

    @staticmethod
    def __unpad(plain_text):
        last_character = plain_text[len(plain_text) - 1:]
        return plain_text[:-ord(last_character)]

def convert_string_hex_to_bin(inp):
    return int(inp, 16).to_bytes(16, "big")

def convert_string_hex_to_str(inp):
    return b64encode(convert_string_hex_to_bin(inp)).decode("utf-8")

if __name__ == '__main__':
    key = "10a58869d74be5a374cf867cfb473859"
    key = int(key, 16).to_bytes(16, "big")
    testing = AESCipher(key)
    e = testing.encrypt_text("hi there")
    print("encrypted: " + e)
    p = testing.decrypt_text(e)
    print("decrypted: " + p)

    blank_msg = "00000000000000000000000000000000"
    blank_bin = int(blank_msg).to_bytes(16, "big")
    ee = testing.encrypt_bin(blank_bin)
    ee = b64encode(ee).decode("utf-8")
    print(ee)
    t_ee = "6d251e6944b051e04eaa6fb4dbf78465"
    t_ee = int(t_ee, 16).to_bytes(16,"big")
    t_ee = b64encode(t_ee).decode("utf-8")
    print(t_ee)
