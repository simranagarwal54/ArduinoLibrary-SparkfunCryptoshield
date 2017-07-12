#include <cryptoauth.h>

// Change to AtSha204() to use the 204
//AtEcc108 sha = AtEcc108();
AtSha204 sha=AtSha204();
AtEcc108 ecc=AtEcc108();

void setup() {
    Serial.begin(9600);
    sha.enableDebug(&Serial);
    ecc.enableDebug(&Serial);
}

void loop() {
    /* If you haven't personalized your device yet, you will recieve
     * this on your serial terminal:
       ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000ffff0000
       Success

       Otherwise, you'll get actual random bytes.
    */
    if (0 == sha.getRandom()){
        Serial.println(" Sha Random Success");
        sha.rsp.dumpHex(&Serial);
    }
    else{
        Serial.println("Failure");
    }

    delay(1000);

    if (0 == ecc.getRandom(0)){
        Serial.println("Ecc Random Success");
        ecc.rsp.dumpHex(&Serial);
    }
    else{
        Serial.println("Failure");
    }
      delay(1000);
  Serial.println();
}
