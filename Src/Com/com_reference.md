# EmbedLink Communication Reference 

## COM Packets

### Packet Max 32 Byte

    1     1     1        28        1  
 +-----+-----+-----+============+-----+
 | STA | TYP | LEN |  Payload   | CRC |
 +-----+-----+-----+============+-----+

### Command Packet 4 Byte

    1     1     1     1  
 +-----+-----+-----+-----+
 | STA | CMD |  0  | CRC |
 +-----+-----+-----+-----+

## COM Reference  

STA: Start byte of the packet / Frame Synchronization
* COM_STA	0x2B 

TYP: Type byte of the packet / Payload identifier
* COM_SYC	0x21
* COM_ACK	0x22
* COM_NAK	0x23
* COM_RET	0x24
* COM_RQE	0x25

CRC: Cyclic redundancy check CRC-8