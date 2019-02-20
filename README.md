An query encryption/decryption in C, wrapped for python.

$./make.sh

import QCrypt
enc = QCrypt.encode("query_to_encode")\n
dec = QCrypt.decode(enc)
