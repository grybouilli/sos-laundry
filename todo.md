# Bluetooth
## Common

<!-- Check for connection -> connected ? start communication : keep testing connection -->

 [x] Initiate connection between parties
    -> [x] Implement connection establishment test

 [x] Establish com protocol
 [x] Ring buffer for bt message handling
 [x] Communication test

## Server (Slave, door unit)

 [x] Setup role with AT commands
 [x] UART / Bluetooth setup
 [x] Handle received commands
 [] Control lights (ready & not ready states)

## Client (Master, laudromat unit)

 [x] Setup role with AT commands
 [x] UART / Bluetooth setup
 [x] Send commands on light detection

# Physical device
## CAO

[] Design door front system (LED Display, "Maison du Hobbit", etc)
[] Design door back system (case for the arduino and the circuitry, back of the design etc)
[] Design a case for the master side?

## Electronics

[] Solder wires on laudromat
[] Assemble master side
[] Assemble slave side

# Repo

[] Make clean Makefiles
[] Make circuit schematics
[] Write clear README.md
[] Document code *(in process)*
