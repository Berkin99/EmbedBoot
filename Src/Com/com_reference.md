# EmbedLink Communication Reference 

## COM Packets

### Packet Max 20 Bytes

    1     1     1        16        1  
 +-----+-----+-----+============+-----+
 | STA | TYP | LEN |  Payload   | CRC |
 +-----+-----+-----+============+-----+

### Command Packet 4 Bytes

    1     1     1     1  
 +-----+-----+-----+-----+
 | STA | CMD |  0  | CRC |
 +-----+-----+-----+-----+

### User Packet X Bytes

    1     1     1        X        1  
 +-----+-----+-----+===========+-----+
 | STA | USR | LEN |  Payload  | CRC |
 +-----+-----+-----+===========+-----+

* User types need to be unique.
* Payload cannot exceed 16 bytes.
* Users should use `comPacketHandler()`.
* Last Transmitted Packet: All packets including the command packet, excluding retransmits.
* Last Received Packet: All packets are saved unless they are command packets.

## COM Reference  

STA: Start byte of the packet / Frame Synchronization  
* `COM_STA` 0x2B 

TYP: Type byte of the packet / Payload identifier  
* `COM_SYC` 0x21  
* `COM_ACK` 0x22  
* `COM_NAK` 0x23  
* `COM_GET` 0x24  
* `COM_SET` 0x24  
* `COM_REQ` 0x25  
* `COM_MSG` 0x??  

* USER_TYPES

CRC: Cyclic Redundancy Check CRC-8

# Bootloader Update Rules:

* 1 MASTER and 1 SLAVE communication.