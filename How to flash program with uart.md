# How to flash program with uart
---

### Step 1: Connect UART to mainboard
1. make sure the mainboard is **NOT** in contact with any conductive surface (e.g. metal case of your notebook, carbon fiber)
2. locate the port, UART1, on the mainboard. "VTRG" can be found beside "UART1".<br>
  V - Vcc<br>
  T - Tx<br>
  R - Rx<br>
  G - Gnd
3. connect uart to port UART1 with:<br>
  5V  to V<br>
  RXD to T<br>
  TXD to R<br>
  GND to G
4. plug the uart to any COM port of your computer. **UNPLUG** it at once if there is a **SHORT CIRCUIT** or when **NO LED IS ON**

### Step 2: Set up for Flash loader and Keil

#### Flash loader
1. locate Flash loader on your computer:<br>
   ```C:\Program Files (x86)\STMicroelectronics\Software\Flash Loader Demo\STMFlashLoader Demo.exe```
2. create a shortcut on desktop because it is hard to find...
3. ensure Flash loader is not operating before plugging the uart in
4. you can now plug the uart in

#### Keil
1. plug the uart into the COM port you are going to use
2. check which COM port is it in ```Windows Setting\Devices```
3. go back to Keil and select ```Flash > Configure Flash Tools...```
4. select ```Utilities``` at the top and then select ```Use External Tool for Flash Programming```
5. change the setting ```./Objects/ustrobo17_internal.hex [your COM port id] 115200 STM32F1_Med-density_128K```. e.g. 4 if you are using COM4

### Step 3: Set the mainboard to BOOT mode
1. set the mainboard to BOOT mode by toggling the BOOT mode switch
2. check the BOOT mode led to see if it is on or not. If **NOT**, **UNPLUG** the uart at once and check the wire connection.
3. press the SW1 button, which is the reset button for mainboard.

### Step 4: Flash your program to mainboard

#### Flashing via Flash loader
1. switch on Flash loader
2. select the corresponding COM port ID that the uart is connected to in the drop dowm menu ```Port Name```.
3. press ```Next```
   * if it is ```Not responding```, Here are some possible cause and their solution:
     - the mainboard is not in BOOT mode -> set it to BOOT mode, press reset button and go to 1.
     - if you have not press reset button -> press reset button and go to 1.
     - incorrect COM port is being selected -> go to 1. and select the correct COM port
     - wire connection is loose -> fix the wire connection and go to 1.
     - the Tx and Rx cables are flipped -> flip them back and go to 1.
     - you may try to switch to another COM port
   * if it moves on to the next page, go to 6.
4. press ```Next```
5. press ```Next```
6. select ```Download``` and press ```...``` at the right to select the address
7. select ```.hex``` file at the bottom-right corner
8. select ```ustrobo17_internal\Objects\ustrobo17_internal.hex``` and press ```Next```
9. wait for it to finish flashing and press ```Close```

#### Flashing via Keil
1. ensure Flash loader is not running
2. press F7 to compile the program
3. press F8 to flash the program
   * if it stops flashing somewhere, check the error message to debug. Here are some possible cause and their solution:
     - the mainboard is not in BOOT mode -> set it to BOOT mode, press reset button and press F8
     - if you have not press reset button -> press reset button and press F8
     - the uart is not connected to the COM port set in the configuration -> connect it to the correct one / set it to current one and press F8
     - there might be an error in your program -> fix the error, press F7 and then F8
     - the Tx and Rx cables are flipped -> flip them back, press F7 and then F8
   * if there are a lot of ```[OK]```, it is flashed successfully

### Step 5: Run the program
1. exit BOOT mode by the toggling the BOOT mode switch
2. press reset button
