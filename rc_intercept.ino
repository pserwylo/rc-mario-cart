// Number of RX functions we are intercepting, currently just "Right"
#define ACTION_COUNT      1

// Arduino pins that the input functions from the RX chip are connected to.
#define RC_INPUT_RIGHT    5
#define RC_OUTPUT_RIGHT   6

// Duration between presses (in ms) to count as 
// an "action" - in my case, "fire infrared" 
#define ACTION_DURATION 500

#define ACTION_DISABLE_STEERING_TIME  5000

// I want to be able to iterate over each of the pins that
// are being intercepted on the Arduino, so they will all 
// be put into these arrays in the future.
int pins[ ACTION_COUNT ]                        = { RC_INPUT_RIGHT };
int outputPins[ ACTION_COUNT ]                  = { RC_OUTPUT_RIGHT };
boolean isActive[ ACTION_COUNT ]                = { false };
const char* labels[ ACTION_COUNT ]              = { "right" };
long unsigned int lastActivated[ ACTION_COUNT ] = { 0 };

long unsigned int disabledSteeringTime = 0;

void setup() {
  Serial.begin( 9600 );
  
  for ( int i = 0; i < ACTION_COUNT; i ++ ) {
    pinMode( pins[ i ], INPUT );
    pinMode( outputPins[ i ], OUTPUT );
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
  Serial.print( "Disabling steering for " );
  Serial.print( ACTION_DISABLE_STEERING_TIME / 1000 );
  Serial.println( " secs" );
  
  disabledSteeringTime = millis();
  digitalWrite( RC_OUTPUT_RIGHT, LOW );
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
    int*    outputPin = &outputPins[ i ];
    boolean* isActive = &isActive[ i ]; 
    const char* label = labels[ i ];
    long unsigned int* lastActive = &lastActivated[ i ];
    
    boolean isHigh = ( digitalRead( *pin ) == HIGH );
 
    if ( disabledSteeringTime > 0 && millis() - disabledSteeringTime > ACTION_DISABLE_STEERING_TIME ) {
      disabledSteeringTime = 0;
      Serial.println( "Reacrivating steering" );
    }
 
    // Previously pressed, now released...   
    if ( *isActive && !isHigh ) {
    
      Serial.print( label );
      Serial.println( " OFF" );
    
      *isActive = false;
      digitalWrite( *outputPin, LOW );
    
    // Previously released, now pressed...
    } else if ( !*isActive && isHigh && disabledSteeringTime == 0 ) {
    
      Serial.print( label );
      Serial.println( " ON" );

      *isActive = true;
      digitalWrite( *outputPin, HIGH );
    
      if ( millis() - *lastActive < ACTION_DURATION ) {
        actionPerformed( i );
      }
    
      *lastActive = millis();
    }
  }
  
}
