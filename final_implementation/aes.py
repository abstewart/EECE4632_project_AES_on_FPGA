import hashlib
from base64 import b64encode, b64decode
from Crypto.Cipher import AES


#The below class is based on python implementation from Pablo T. Campos
#Link: https://medium.com/quick-code/aes-implementation-in-python-a82f582f51c2
class aes_algo(object):
    def __init__(self, binary_key):
        self.block_size = AES.block_size
        self.key = binary_key
        return


    def pad_str(self, ptext):
        num_bytes_to_pad = self.block_size - (len(ptext) % self.block_size)
        ascii_int = chr(num_bytes_to_pad)
        full_pad_str = num_bytes_to_pad * ascii_int
        text_with_pad = ptext + full_pad_str
        return text_with_pad

    def unpad_str(self, ptext):
        last_char = ptext[len(ptext)-1:]
        num_pad_bytes = ord(last_char)
        return ptext[:-num_pad_bytes]

    def encrypt_txt(self, ptext):
        plain_text = self.pad_str(ptext)
        cipher = AES.new(self.key, AES.MODE_ECB)
        encrypted_text = cipher.encrypt(ptext.encode())
        return b64encode(encrypted_text).decode("utf-8")

    def decrypt_txt(self, encrypted_text):
        encrypted_text = b64decode(encrypted_text)
        cipher = AES.new(self.key, AES.MODE_ECB)
        ptext = cipher.decrypt(encrypted_text).decode("utf-8")
        return self.unpad_str(ptext)

    def encrypt_bin(self, binary):
        cipher = AES.new(self.key, AES.MODE_ECB)
        return cipher.encrypt(binary)

    def decrypt_bin(self, binary):
        cipher = AES.new(self.key, AES.MODE_ECB)
        return cipher.decrypt(binary).decode("latin_1")
    
        
#end of class

#utility functions
def convert_string_hex_to_bin(inp):
    return int(inp, 16).to_bytes(16, "big")

def convert_string_hex_to_str(inp):
    return b64encode(convert_string_hex_to_bin(inp)).decode("utf-8")

def convert_bin_to_str(binary):
    return b64encode(binary).decode("utf-8")

"""
#main testing stuff
if __name__ == '__main__':
    text = "hi there"
    print("str" + text)
    print(len(text))
    
    print("chr" + chr(3))
    print(len(chr(3)))
    
    print(text.encode())
    print(type("hi there".encode()))
    print(len(text.encode()))
    
    print("aes block size in bytes:" + str(AES.block_size))

    print("testing cipher implementation")

    key = "10a58869d74be5a374cf867cfb473859"
    ptext = "00000000000000000000000000000000"
    etext = "6d251e6944b051e04eaa6fb4dbf78465"

    key_bin = convert_string_hex_to_bin(key)
    ptext_bin = convert_string_hex_to_bin(ptext)
    etext_str = convert_string_hex_to_str(etext)
    
    en = aes_algo(key_bin)
    e = en.encrypt_bin(ptext_bin)
    print("encrypted stuff")
    print(convert_bin_to_str(e))
    print(etext_str)

    print("decrypting stuff")

    print("actual")
    print(convert_string_hex_to_str(ptext))
    d = en.decrypt_bin(e)
    dt = convert_bin_to_str(d)
    print(dt)
    print(d.hex())


    #reference stuff
    #bytes to hex: b'01'.hex()
    #hex to bytes: bytes.fromhex('deadbeef')
"""