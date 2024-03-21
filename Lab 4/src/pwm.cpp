// Authors:     Group 3  
// Date:        04/04/2024   
// Assignment:  Lab 4
//
// Description: 
//
// Requirements:
//      1.You have the option to create a single PWM signal or two PWM
//      signals. The PWM signal or signals together with the L293D enable
//      chip signal must control the speed and the direction of rotation.
//      2. The PWM signal or signals should be implemented on timer 3 and, if
//      using two PWM signals, include timer 4. Note: You will need to demo
//      your PWM signal coming from timer 3 using the digital storage
//      oscilloscope in the lab.
//      3.Has a changeDutyCycle function.
//      -the changeDutyCycle function takes in an argument which is the
//      combination of the analog to digital conversion, ADCH and ADCL
//      registers. The result of the combination of the ADCH and ADCL
//      registers is a 10-bit number. From this you will edit the OCRn
//      Aregisters for the PWM timer or timers used for the lab. Editing the
//      OCRnA register changes the duty cycle of the PWM signal.
//----------------------------------------------------------------------//

