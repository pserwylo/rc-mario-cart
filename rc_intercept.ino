// Arduino pin that the "Right" input function from the
// RX chip is connected to.
#define RC_INPUT_RIGHT    5

// Duration between presses (in ms) to count as 
// an "action" - in my case, "fire infrared" 
#define ACTION_DURATION 500

boolean isActive = false;

// Last time the "Right" function was activated
long unsigned int lastActivated = 0;

void setup() {
  Serial.begin( 9600 );
  pinMode( RC_INPUT_RIGHT, INPUT );
}

/**
 * An action was pressed twice in quick succession.
 * We can use queues like this to mean, e.g. "Fire IR LED" in the future.
 */
void actionPerformed() {
  
  // For now, just log this to the serial port for debugging...
  Serial.println( "Received action from Right pin" );
  
  // TODO: Hook up IR LED and "fire" it when an action is performed.

}

/**
 * Continuously read input from the RX chip. 
 * If the "Right" function is activated or deactivated, log it and remember it.
 * Also, if it is identified for the second time in ACTION_DURATION
 * milliseconds, then invoke actionPerformed().
 */
void loop() {
 
  boolean isHigh = ( digitalRead( RC_INPUT_RIGHT ) == HIGH );
 
  // Previously pressed, now released...   
  if ( isActive && !isHigh ) {
    
    Serial.println( "Right OFF" );
    isActive = false;
    
  // Previously released, now pressed...
  } else if ( !isActive && isHigh ) {
    
    Serial.println( "Right ON" );
    isActive = true;
    
    if ( millis() - lastActivated < ACTION_DURATION ) {
      actionPerformed();
    }
    
    lastActivated = millis();
  }
  
}
