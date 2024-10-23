

# EmbedLink Communication Reference 

All communications 
### COM Packet 20 Byte

    1     1     1        16        1  
 +-----+-----+-----+============+-----+
 | STA | TYP | LEN |  Payload   | CRC |
 +-----+-----+-----+============+-----+
 
 * COM STA : Start Byte : 0xA0 