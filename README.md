You can read the paper discussing this device [here](http://noahtrenaman.com/media/Digit_Paper.pdf).
  
  # The Digit Glove
  The Digit Glove is a wearable tactile language device based on the Braille language. It is an attempt to design a dedicated computer-to-human communication channel through the tactile sense. The chief difference between this device and Braille is that language is "read" through a set of vibrating devices instead of through elevated dots on a page. 
  
The code found in the `python` folder can control a Raspberry Pi for a pitch-modulation approach, and the code found in the `cpp` and `arduino` folders are used for an Arduino nano to operate a series of vibration motors to represent Braille through a binary approach. It is still up to investigation as to which methods are most effective.
 
  ## Unit of Meaning
A unit of meaning is simply any one character, such as `a`, `z`, `:`, or `1`. For Braille there are 64 possible units of meaning. Braille accomplishes this through 6 bits (raised or flat), which allows for 2^6 permutations. In order to emulate Braille, any tactile system must also be capable of communicating 6 bits per unit of meaning. The pitch-modulation approach accomplishes this through 3 epochs of 4 variants, which allows for 4^3 permutations. 2^6 = 3^4 = 64. The direct binary approach accomplishes this through allowing 6 bits to be communicated by on-off patterns of vibration motors.
 
   ## Pitch Modulation
 A unit of meaning is communicated by three sequential pitch modulation epochs, which have four variants. The four variants are `LOW`, `HIGH`, `LOW to HIGH`, and `HIGH to LOW`. A `LOW` epoch is 40 Hz, and a `HIGH` epoch is 150 Hz. The shifting epochs bend from pitch to pitch at a constant rate over the duration of the epoch. An epoch length is defined as the length of the unit of meaning divided by four. This allows for 75% of the unit of meaning to be vibrations, and 25% to be at rest to communicate that the next unit of meaning is coming.
 
![Digit Animation](media/braille.gif)
This gif demonstrates how each epoch represents a row of a Braille cell.
