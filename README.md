  This README file is a tl;dr of the full paper, which can be read [here](http://noahtrenaman.com/media/Digit_Paper.pdf). This file is more focused on the specific software used.
  
  # The Digit Glove
  The Digit Glove is a wearable tactile language encoder based on the Braille language. The purpose of Digit is to allow a dedicated computer-to-human communication channel through the tactile sense. It could be very helpful for the blind population, but it is not limited in this scope as sighted people could gain from it as well (see paper for more details). Braille, being the first system that uses binary encoding (6 bits per character), is capable of efficiently communicating language through the sense of touch. Digit attempts to accomplish much of the same. The chief difference is that Digit is "read" through a set of vibrating devices instead of through elevated dots on a page. 
  
The code found in the `python` folder can control a Raspberry Pi for a pitch-modulation approach, and the code found in the `cpp` and `arduino` folders are used for an Arduino nano to operate a series of vibration motors to represent Braille through a direct binary approach. It is still up to investigation as to which perceptual methods are most effective.
 
  ## Unit of Meaning
A unit of meaning is simply any one character, such as `a`, `z`, `:`, or `1`. For Braille there are 64 possible units of meaning. Braille accomplishes this through 6 bits (raised or flat), which allows for 2^6 permutations. In order to emulate Braille, a tactile system must also be capable of communicating 6 bits per unit of meaning. The pitch-modulation approach accomplishes this through 3 epochs of 4 variants, which allows for 4^3 permutations. 2^6 = 3^4 = 64. The direct binary approach accomplishes this through allowing 6 bits to be communicated by on-off patterns of vibration motors.
  
  # Using Digit with a Raspberry Pi
 This repository contains Python code for a raspberry pi to control a series of vibration speakers to communicate a given stream of information. The vibration speaker is controlled directly from any of the Raspberry pi's GPIO pins, meaning that the vibration speaker chosen should have an operating voltage in the area of 3V. The specific pins are assigned in the `pins` array.
 Determine which pins you would like to use and attach each one to the positive contact of a vibration speaker, and connect the negative contact to the ground of the raspberry pi. Calling the `setup()` function from `main.py` should generate a low tone (100Hz) in the assigned pins.
 
   ## Epochs
 Digit communicates a unit of meaning through 3 sequential pitch modulation epochs, which have 4 variants. The four variants are `LOW`, `HIGH`, `LOW to HIGH`, and `HIGH to LOW`. A `LOW` epoch is 40 Hz, and a `HIGH` epoch is 150 Hz. The shifting epochs bend from pitch to pitch at a constant rate over the duration of the epoch. An epoch length is defined as the length of the unit of meaning divided by 4. This allows for 75% of the unit of meaning to be vibrations, and 25% to be at rest to communicate that the next unit of meaning is coming.
 
![Digit Animation](media/braille.gif)
This gif demonstrates how each epoch represents a row of a Braille cell.
 
 ## Required Libraries
 This software uses a few libraries that must be installed in the Raspberry pi. These libraries are `RPi.GPIO`, `matplotlib`, `numpy`, and `math`. These are all on PyPI and can be installed with pip.
  
## Configuration
The system is highly configurable, allowing for multiple streams of information to be read, and for customization of the reading experience. This is especially helpful for learning the encoding system.

## Stream
A stream object represents a string of information, and the index of the string that the reader is currently at. For example, if one is reading the sentence, `the quick brown fox jumped over the lazy dog`, and they are currently at the letter `b` in `brown`, then `Stream.index = 10`. Individual streams can be read in different ways based on how they interface with a Reader object.

## Reader
The reader can control how many devices they are using, how fast the characters are presented, and which direction to read through text (to allow re-reading). This is done through the `Reader` class, which takes three arguments:

`rate` - the time, in seconds, that each unit of meaning should take. It is recommended for learning that `rate` be set no lower than 1, and then reduced as the reader becomes more familiar.

`length` - this represents the number of vibration speakers, or rings, that the reader is using. This allows for the reader to perceive as many units of meaning simultaneously as they have digit rings. For example, if the reader has 8 digit rings and are at index `10` of the previously mentioned sentence, they would read `brown fo`. After a time equal to `rate` has passed, they would read `rown fox`.

`direction` - This value should be set to 1 so that the reader can read one new unit of meaning at the speed of `rate`. However, to allow for re-reading or pausing, `direction` can be set to -1 and 0, to scroll backwards in the stream or stop.

## Encoding
The encoding system is guided by two arrays: `code_epochs` and `code_map`. At a given index, `code_epochs` represents the 3 epoch types associated with the unit of meaning at `code_map`. For example, `code_map[0] = "a"`, and `code_epochs[0] = ["HL", "L", "L"]`, detailing that the letter a consists of a HIGH to LOW epoch followed by two LOW epochs. This encoding system was inspired directly by Braille, but can be adjusted to introduce different modulation patterns for particular letters.

# Using Digit with an Arduino Nano
The code found in the Arduino folder follows a different approach at language encoding. Instead of using the pitch-modulation system with epochs, it control a series of three motors per finger which communicate one column of a Braille cell. The system switches from one column to the next in rapid succession, creating a unique sensation for each character.

This code is much more compressed in order to run efficiently on a microcontroller with limited memory. The macros `data_len` and `parallel_cells` can be adjusted for different hardware setups. `data_len` describes how long the array of characters should be, with the future implementation of being able to read a Bluetooth signal. `parallel_cells` represents how many units of meaning can be communicated at one time, and this is dependent on the number of vibration motors connected. For a glove with three vibration motors per finger (excludung the thumb), the number should be 12.

