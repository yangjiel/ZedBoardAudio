restart:
    ; Wait a while, for the FPGA clock to settle.
        DELAY 32768 ; a while

    ; Set up the PLL, by first turing it off
        ;   reg[0] <= 0x0E
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x00
        WRITE   0x0E
        STOP

    ; then write the configuration values
        ; reg[2] <= 0x02 0x71 0x01 0x00 0x1b 0x01  (it requires 6 bytes written to it)
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x02
        WRITE   0x00
        WRITE   0x7D
        WRITE   0x00
        WRITE   0x0C
        WRITE   0x23
        WRITE   0x01
        STOP

    ; Wait until bit 1 on the last byte of register 2  is '1',
    ; indicating that the CODEC's
    ;
    ; I cheat and wait 32,768 I2C clock cycles (about 64ms)
        DELAY   32768 ; a while

    ; Enable the core by setting reg[0] to 0x0F
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x0
        WRITE   0xF
        STOP

    ; Wait a short while for the core to power up.
        DELAY   32768 ; a while

; At this point the core is powered up and all the other registers can be set.

    ; Become the I2S master.
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x15 ; R15 Serial Port 0
        WRITE   0x1
        STOP


   ; Setting all the input mixers
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; Reg[4] - Record mixer left control 0
        WRITE   0x0A
        WRITE   0x01 ; Enable the mixer
        STOP

        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; Reg[5] - Record mixer left control 1
        WRITE   0x0B
        WRITE   0x05 ; MX1AUXG = 0db
        STOP

        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; Reg[6] - Record mixer right control 0
        WRITE   0x0C
        WRITE   0x01 ; Just enable the mixer
        STOP

        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; Reg[7] - Record mixer right control 1
        WRITE   0x0D
        WRITE   0x05 ; Record mixer right Control 1 - MX2AUXG = 0db
        STOP

   ; Setting all the output mixers
        ; reg[0x1C] <= 0x2D   Playback mixer left 0
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x1C
        WRITE   0x21
        STOP

        ; reg[0x1E] <= 0x4E   Playback Mixer right 0
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x1E
        WRITE   0x41
        STOP

        ; reg[0x23] <= 0xE7   Playback Headphone volume left
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x23
        WRITE   0xE7
        STOP

        ; reg[0x24] <= 0xE7   Playback Headphone volume right
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x24
        WRITE   0xE7
        STOP

        ; reg[0x25] <= 0xE7   Playback Line out volume left
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x25
        WRITE   0xE7
        STOP

        ; reg[0x26] <= 0xE7   Playback Line out volume right
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x26
        WRITE   0xE7
        STOP


    ; Set up the ADC
        ; reg[0x19] <= 0x???
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x19
        WRITE   0x03
        STOP

    ; Set up the DAC
        ; reg[0x29] <= 0x???
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x29
        WRITE   0x03
        STOP

        ; reg[0x2A] <= 0x03
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; subaddress MSB
        WRITE   0x2A
        WRITE   0x03
        STOP

    ; Set up the signal routing
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; R58 - Serial Input Route Control
        WRITE   0xF2
        WRITE   0x01
        STOP

   ; Power up the various parts of the cores
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; R59 - Serial Output Route Control
        WRITE   0xF3
        WRITE   0x01
        STOP

   ; Power up the various parts of the cores
        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; Reg[65] - Clock enable 0
        WRITE   0xF9
        WRITE   0x7F ; Power up all the modules
        STOP

        WRITE   0x76 ; dev_write_address
        WRITE   0x40 ; Reg[66] - Clock enable 1
        WRITE   0xFA
        WRITE   0x03 ; Enable the two clocks
        STOP

done:
        DELAY 32768
        JUMP done
