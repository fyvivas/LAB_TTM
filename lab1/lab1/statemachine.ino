/*!
\file   statemachine.ino
\date   2022-08-02
\author Fulvio Vivas <fulvio.vivas@unicauca.edu.co>
\brief  State Machine on temperature control.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of Unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) Unicauca 2022. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/


/*F**************************************************************************
* NAME: setup_State_Machine
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Setup the State Machine
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void setup_State_Machine()
{
  // Add transitions
  stateMachine.AddTransition(LOW_A, NORMAL_B, []() { return currentInput == Sign_T; });
  stateMachine.AddTransition(NORMAL_B, LOW_A, []() { return currentInput == Sign_D; });

  stateMachine.AddTransition(NORMAL_B, HIGH_C, []() { return currentInput == Sign_T; });
  stateMachine.AddTransition(HIGH_C, NORMAL_B, []() { return currentInput == Sign_D; });

  stateMachine.AddTransition(HIGH_C, ALERT_D, []() { return currentInput == Sign_T; });
  stateMachine.AddTransition(ALERT_D, NORMAL_B, []() { return currentInput == Sign_P; });

  // Add actions
  stateMachine.SetOnEntering(LOW_A, input_LowA);
  stateMachine.SetOnEntering(NORMAL_B, input_NormalB);
  stateMachine.SetOnEntering(HIGH_C, input_HighC);
  stateMachine.SetOnEntering(ALERT_D, input_AlertD);

  stateMachine.SetOnLeaving(LOW_A, output_LowA);
  stateMachine.SetOnLeaving(NORMAL_B, output_NormalB);
  stateMachine.SetOnLeaving(HIGH_C, output_HighC);
  stateMachine.SetOnLeaving(ALERT_D, output_AlertD);
}

/*F**************************************************************************
* NAME: read_data
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Read data from serial port
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void read_data(){
  
  bool flag_data = false;
  
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      Serial.print("Value:");
      temp_value = inString.toInt();
      flag_data = true;
      Serial.println(temp_value);
      Serial.print("String: ");
      Serial.println(inString);
      // clear the string for new input:
      inString = "";
    }
  }

  if (flag_data){
    update_Input_State_Machine();
  }

}

/*F**************************************************************************
* NAME: state_Low
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Check conditions for state_LOW_A
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void state_Low(){
  if(temp_value > TH_TEMP_LOW){
    currentInput = Input::Sign_T;
  }
  else{
    currentInput = Input::Unknown;
  }
}

/*F**************************************************************************
* NAME: state_Normal
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Check conditions for state_NORMAL_B
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void state_Normal(){
  if(temp_value < TH_TEMP_LOW){
    currentInput = Input::Sign_D;
  }
  else if(temp_value > TH_TEMP_NORM){
      currentInput = Input::Sign_T;
  }
  else{
    currentInput = Input::Unknown;
  }
}

/*F**************************************************************************
* NAME: state_High
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Check conditions for state_HIGH_C
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void state_High(){
  if(temp_value < TH_TEMP_NORM){
    currentInput = Input::Sign_D;
  }
  else if(temp_value > TH_TEMP_HIGH){
     currentInput = Input::Sign_T;
  }
  else{
    currentInput = Input::Unknown;
  }
}

/*F**************************************************************************
* NAME: state_Alert
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Check conditions for state_ALERT_D
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void state_Alert(){
  Serial.println("state ALERT");
}


/*F**************************************************************************
* NAME: update_Input_State_Machine
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* update State Machine inputs from temperature sensor
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void update_Input_State_Machine()
{
  int currentState = stateMachine.GetState();
  switch (currentState)
  {
    case LOW_A:     state_Low();
                    break;
    case NORMAL_B:  state_Normal(); 
                    break;
    case HIGH_C:    state_High(); 
                    break;
    case ALERT_D:   state_Alert(); 
                    break;
    default:        Serial.println("state Unknown"); 
                    break;
  }
}

/*F**************************************************************************
* NAME: input_LowA
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Execute actions when entering state LOW_A
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void input_LowA()
{
  currentInput = Input::Unknown;
  DEBUG("State_LOW_A");
  Serial.println("A   B   C   D");
  Serial.println("X            ");
  Serial.println();
  digitalWrite(LED_GREEN_PIN,HIGH);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.setRGB(0, 255, 0);
  lcd.print("State:LOW_A");
}

/*F**************************************************************************
* NAME: output_LowA
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Execute actions when leaving state LOW_A
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void output_LowA()
{
  //DEBUG("Leaving_State_LOW_A");
  Serial.println("Leaving State_LOW_A");
  digitalWrite(LED_GREEN_PIN,LOW);
}

/*F**************************************************************************
* NAME: input_NormalB
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Execute actions when entering state NORMAL_B
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void input_NormalB()
{
  currentInput = Input::Unknown;
  DEBUG("State_NORMAL_B");
  Serial.println("A   B   C   D");
  Serial.println("    X        ");
  Serial.println();
  digitalWrite(LED_YELLOW_PIN,LOW);
  digitalWrite(LED_GREEN_PIN,LOW);
  digitalWrite(LED_RED_PIN,LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.setRGB(128, 128, 128);
  lcd.print("State:NORMAL_B");
}

/*F**************************************************************************
* NAME: output_NormalB
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Execute actions when leaving state NORMAL_B
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void output_NormalB()
{
  //DEBUG("Leaving State_NORMAL_B");
  Serial.println("Leaving State_NORMAL_B");
}

/*F**************************************************************************
* NAME: input_HighC
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Execute actions when entering state HIGH_C
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void input_HighC()
{
  currentInput = Input::Unknown;
  DEBUG("State_HIGH_C");
  Serial.println("A   B   C   D");
  Serial.println("        X    ");
  Serial.println();
  digitalWrite(LED_YELLOW_PIN,HIGH);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.setRGB(0, 0, 255);
  lcd.print("State:HIGH_C");
}

/*F**************************************************************************
* NAME: output_HighC
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Execute actions when leaving state HIGH_C
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void output_HighC()
{
  //DEBUG("Leaving State_HIGH_C");
  Serial.println("Leaving State_HIGH_C");
  digitalWrite(LED_YELLOW_PIN,LOW);
}

/*F**************************************************************************
* NAME: input_AlertD
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Execute actions when entering state ALERT_D
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void input_AlertD()
{
  asyncTask1.Start();
  currentInput = Input::Unknown;
  DEBUG("State_ALERT_D");
  Serial.println("A   B   C   D");
  Serial.println("            X");
  Serial.println();
  digitalWrite(LED_RED_PIN,HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.setRGB(255, 0, 0);
  lcd.print("State:ALERT_D");
}

/*F**************************************************************************
* NAME: output_AlertD
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Execute actions when leaving state ALERT_D
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void output_AlertD()
{
  //DEBUG("Leaving State_ALERT_D");
  Serial.println("Leaving State_ALERT_D");
  digitalWrite(LED_RED_PIN,LOW);
}
