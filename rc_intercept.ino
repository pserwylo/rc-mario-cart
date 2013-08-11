// Number of RX functions we are intercepting, currently just "Right"
#define ACTION_COUNT      2

// Arduino pins that the input functions from the RX chip are connected to.
#define RC_INPUT_RIGHT    5
#define RC_INPUT_LEFT     7
#define RC_OUTPUT_RIGHT   6
#define RC_OUTPUT_LEFT    8

// Duration between presses (in ms) to count as 
// an "action" - in my case, "fire infrared" 
#define ACTION_DURATION 500

#define ACTION_DISABLE_STEERING_TIME  3000

// #define DEBUG

// I want to be able to iterate over each of the pins that
// are being intercepted on the Arduino, so they will all 
// be put into these arrays in the future.
int pins[ ACTION_COUNT ]                        = { RC_INPUT_RIGHT,  RC_INPUT_LEFT  };
int outputPins[ ACTION_COUNT ]                  = { RC_OUTPUT_RIGHT, RC_OUTPUT_LEFT };
boolean isActive[ ACTION_COUNT ]                = { false, false };
const char* labels[ ACTION_COUNT ]              = { "right", "left" };
long unsigned int lastActivated[ ACTION_COUNT ] = { 0, 0 };

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
  
  const char* label   = labels[ pinIndex ];
  const int outputPin = pins[ pinIndex ];
  
  // For now, just log this to the serial port for debugging...
  #ifdef DEBUG
    Serial.print( "Received action from " );
    Serial.println( label );
  #endif
  
  // TODO: Hook up IR LED and "fire" it when an action is performed.
  #ifdef DEBUG
    Serial.print( "Disabling steering for " );
    Serial.print( ACTION_DISABLE_STEERING_TIME / 1000 );
    Serial.println( " secs" );
  #endif
  
  disabledSteeringTime = millis();
  isActive[ pinIndex ] = false;
  digitalWrite( outputPin, LOW );
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
      #ifdef DEBUG
        Serial.println( "Reactivating steering" );
      #endif
    }
 
    // Previously pressed, now released...   
    if ( *isActive && !isHigh ) {
    
      #ifdef DEBUG
        Serial.print( label );
        Serial.println( " OFF" );
      #endif
    
      *isActive = false;
      digitalWrite( *outputPin, LOW );
    
    // Previously released, now pressed...
    } else if ( !*isActive && isHigh && disabledSteeringTime == 0 ) {
    
      #ifdef DEBUG
        Serial.print( label );
        Serial.println( " ON" );
      #endif

      *isActive = true;
      digitalWrite( *outputPin, HIGH );
    
      if ( millis() - *lastActive < ACTION_DURATION ) {
        actionPerformed( i );
      }
    
      *lastActive = millis();
    }
  }
  
}
