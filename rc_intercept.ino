// Number of RX functions we are intercepting, currently just "Right"
#define ACTION_COUNT        2

// Arduino pins that the input functions from the RX chip are connected to.
#define RC_INPUT_RIGHT    5
#define RC_INPUT_LEFT     6

// Duration between presses (in ms) to count as 
// an "action" - in my case, "fire infrared" 
#define ACTION_DURATION 500

// I want to be able to iterate over each of the pins that
// are being intercepted on the Arduino, so they will all 
// be put into these arrays in the future.
int pins[ ACTION_COUNT ]                        = { RC_INPUT_RIGHT, RC_INPUT_LEFT };
boolean isActive[ ACTION_COUNT ]                = { false, false };
const char* labels[ ACTION_COUNT ]              = { "right", "left" };
long unsigned int lastActivated[ ACTION_COUNT ] = { 0, 0 };

void setup() {
  Serial.begin( 9600 );
  
  for ( int i = 0; i < ACTION_COUNT; i ++ ) {
    pinMode( pins[ i ], INPUT );
  }
}

/**
 * An action was pressed twice in quick succession.
 * We can use queues like this to mean, e.g. "Fire IR LED" in the future.
 */
void actionPerformed( const int pinIndex ) {
  
  // For now, just log this to the serial port for debugging...
  Serial.print( "Received action from Right pin" );
  Serial.println( *labels[ pinIndex ] );
  
  // TODO: Hook up IR LED and "fire" it when an action is performed.

}

/**
 * Continuously read input from the RX chip. 
 * If a function is activated or deactivated, log it and remember it.
 * Also, if it is identified for the second time in ACTION_DURATION
 * milliseconds, then invoke actionPerformed().
 */
void loop() {
 
  for ( int i = 0; i < ACTION_COUNT; i ++ ) {
    int*          pin = &pins [ i ];
    boolean* isActive = &isActive[ i ]; 
    const char* label = labels[ i ];
    long unsigned int* lastActive = &lastActivated[ i ];
    
    boolean isHigh = ( digitalRead( *pin ) == HIGH );
 
    // Previously pressed, now released...   
    if ( *isActive && !isHigh ) {
    
      Serial.print( label );
      Serial.println( " OFF" );
    
      *isActive = false;
    
    // Previously released, now pressed...
    } else if ( !*isActive && isHigh ) {
    
      Serial.print( label );
      Serial.println( " ON" );

      *isActive = true;
    
      if ( millis() - *lastActive < ACTION_DURATION ) {
        actionPerformed( i );
      }
    
      *lastActive = millis();
    }
  }
  
}
