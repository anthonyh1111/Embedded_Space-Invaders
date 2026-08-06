# ECE 16 Grand Challenge

Table of Contents
- [ECE 16 Grand Challenge](#ece-16-grand-challenge)
  - [Grand Challenge 1: Space Invaders Controller](#grand-challenge-1-space-invaders-controller)
    - [Implementations of 3 improvements and 3 features](#implementations-of-3-improvements-and-3-features)
    - [Using the Controller](#using-the-controller)
  - [Grand Challenge 2: Guitar Tuner](#grand-challenge-2-guitar-tuner)
  - [Collaborators](#collaborators)


A socket-based controller for Space Invaders to controller the game wirelessly with the ESP32.

The game is a modified version of Space Invaders based off of this project: https://github.com/leerob/Space_Invaders

You must have Pygame installed before being able to run the game: https://www.pygame.org/


## Grand Challenge 1: Space Invaders Controller



### Implementations of 3 improvements and 3 features

For the 3 features I chose 

- Updating the OLED with the current score and the number of remaining lives
- Top Scores: Updating the OLED with the top 3 scores after the game ends
- Sensitivity Settings: By pushing the button I could cycle through faster/ slower movements with 1 being the slowest and 3 being the fastest.

For the 3 Improvements

- Smoothing of the accelerometer values for better movements
- Button firing
- Rapid firing

First, to implement the features I created a new socket to send information on which made it easier to manipulate the information sent on that socket. The controller received that infomation on the new socket and I used it to update the display values on the OLED through arduino. To set up the new socket I just had to initialize it in the game code and the controller so that it could send and receive information. On arduino I would write a command such as if(command.startsWith("Score:", etc.)) it would call a function in Display.ino and update the score with its appropriately receieved value and wasn't too hard to implement. The challenge was understanding how to create the new socket properly and where to send information from the game code. 

Second, to implement the improvements I used a moving average filter to smooth the values coming from the acceelerometer and applied that to the getOrientation() function to get more accurate data from the accelerometer. In arduino, I had to make sure that the sensitivity was accurate with the movement so I used an if statement along with a switch statement to make sure the movement and sensitivity worked hand-in-hand. To implement button firing I used the photodetector. In the Photodetector I applied a low-pass-filter (moving average) for data and applied a threshold that I could print out the values for and calculate the threshold in which the button was pressed and not pressed (isCovered) function. The difficulty in these implementations were in figuring the appropriate values for accelerometer movement and correct values for the photodetector "button" press. 

### Using the Controller 
Although the controller isn't perfect, to use the controller you would hold it horizontally and apply a slight tilt upwards for more accurate movement. You would need to implement a button, OLED, photodetector, accelerometer, to your adafeather. For firing you would tap on the photosensor, for movement you would tilt the board left and right, for sensitivity you would cycle through the button with 1 being the lowest and 3 being the highest, and for rapid firing just tap as fast as you can/ hold your finger on the accelerometer . 

[video of controller](https://youtu.be/R6F89TPE4mo) 

## Grand Challenge 2: Guitar Tuner
In this challenge, we had the creative freedom to do whatever we saw fit for our final project. This project needed to address a problem that we had in our lives, and of course needed to utilize the MCU in some kind of way. The problem I wanted to address was tuning in noisy enviroments. My roommates are often causing a ruckus, and tuning in these enviroments is not ideal. Because tuning with my phone uses the mic, it's prone to picking up other noises making my readings less accurate. In addition if i'm tuning my electric guitar acoustically, alot of the times the sound is so weak, the phone mic won't even be able to pick it up. This is why I decided to create my own tuner, that would check the signal of the guitar to determine if it was in tune or not. Nothing grinds everybodys ears more than when notes are out of tune, it just sounds terrible, and unprofessional. The intended users of my device are electric guitar and bass players.

I decided that I wanted to transmit the signal of the guitar into the MCU, and send the data over to Python where I would then calculate the frequency thats being played. We needed to create a preamplifier circuit that would magnify the approx 40mV signal of the guitar, and bias it to 1.6V because the arudino can only read from 0-3.3V. We also added a buffer circuit to impedance match the high impedance output of the guitar to the low impedance of the MCU.
Initially My plan was to send the data over to Python, where A circular list would be waiting to proccess it live. The problem with this approach was that the maximum freqeuncy that we were able to achieve was sad. We maxed out at around 700 Hz. According to the Nyquist rule, we'd only be able to replicate frequencies around 350Hz, but even then I found it didnt' work too well, and the data was oftentimes terrible. To work around this, we sent the data in chunks of 500 Values one at a time to the MCU, which would then proccess the data after 500 Values were sent, which allowed us to sample at up to 5000Hz. Once in the MCU, we used the scipy library to calculate the forier transform of the signal, and using this, we chose the frequency with the largest magnitude to represent the note. We then calculated what note it would most accurately represent, and the difference between the ideal frequency and the frequency we observed.

[Grand Challenge 2 Video Demo Part 1](https://youtube.com/shorts/5qEHsS09Kv4?feature=share)


## Collaborators 

Anthony Huynh: A18489020

Aaron Chakma: A18088368



