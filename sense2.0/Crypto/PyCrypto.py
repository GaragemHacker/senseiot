# http://bityard.blogspot.com.br/2010/10/symmetric-encryption-with-pycrypto-part.html
# https://pypi.python.org/pypi/pycrypto

#!/usr/bin/env python

from Crypto.Cipher import AES
import binascii
 
key = 'mysecretpassword'
plaintext = 'Secret Message A'
 
# --------------------------------------
# Encrypt
# --------------------------------------

encobj = AES.new(key, AES.MODE_ECB)
ciphertext = encobj.encrypt(plaintext)
 
# Resulting ciphertext in hex
msg = ciphertext.encode('hex')
print "Encrypt" 
print ciphertext.encode('hex') 

# --------------------------------------
# Decrypt
# --------------------------------------

ciphertext1 = binascii.unhexlify(msg)
 
decobj = AES.new(key, AES.MODE_ECB)
plaintext = decobj.decrypt(ciphertext1)
 
# Resulting plaintext
print "Decrypt" 
print plaintext
