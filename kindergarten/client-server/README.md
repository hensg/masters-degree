# Problem definition:

## Server
- Each server store data into memory
- Multiple servers may be used to expand the memory to be able to store more data
- The total memory will be the sum of each server memory
- It may look as a single memory to the client, transparent
- Multiple clients cannot modify the same memory region concurrently, it must be avoided

## Client
- Clients may send the following commands to the server throught the network: Write(position, data, sizeofdata) and Read(position, size)
