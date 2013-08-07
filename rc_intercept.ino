// Arduino pin that the "Right" input function from the
// RX chip is connected to.
#define RC_INPUT_RIGHT    5

boolean isActive = false;

void setup() {
  Serial.begin( 9600 );
  pinMode( RC_INPUT_RIGHT, INPUT );
}

/**
 * Continuously read input from the RX chip. 
 * If the "Right" function is activated or deactivated, log it and remember it.
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
    
   }
  
}
