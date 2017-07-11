#include <cryptoauth.h>    
//creating object of class AtEcc108 in the library cryptoauth
AtEcc108 ecc = AtEcc108();

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
char msg1[numChars] = {0};
char msg2[numChars] = {0};
int ch = 0;
int slot1 = 0;
int slot2 = 0;
boolean newData = false;

//this function prints the output in the required hexadecimal format
void hexify(const char *str, const uint8_t *hex, unsigned int len)
{

  int i;
  Serial.write(str);

  Serial.println();

  for (i = 0; i < len; i++)
    {
      static char tmp[4] = {};
      if (i > 0)
        Serial.write(" ");

      sprintf(tmp, "0x%02X", hex[i]);
      Serial.write(tmp);
    }

  Serial.write("\n");

}

/*This function checks for the user input and its start and end markers.*/
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '(';
    char endMarker = ')';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0';
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}




void setup() {
  Serial.begin(9600);
  while(!Serial);
  ecc.enableDebug(&Serial);
  Serial.println("User Menu");
  Serial.println("Enter your choice. Make sure to add '(' and ')' at the start and end of your input as defined below.");
  Serial.println("1. User Input (Choice,YOUR MSG) ");
  Serial.println("2. Create Hash Digest and print it (Choice,Your MSG)");
  Serial.println("3. Create Signature of message and print it (Choice,Your Msg,Private Key Slot");
  Serial.println("4. Check authentication (Choice,Your Message,Public Key Slot)");
  Serial.println("5. Debug mode (Choice,Your Message,Private Key Slot,Public Key Slot,Debug Message)");
  pinMode(2, OUTPUT); //blue
  pinMode(3, OUTPUT); //red
   
}


void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        digitalWrite(2,LOW);   //You need to calibrate the leds according to verification success or failure.
        digitalWrite(3,LOW);
        strcpy(tempChars, receivedChars);
        parseData();
        showParsedData();
        newData = false;  //to ensure that the loop doesn't run in an endless loop
        delay(100);
        
        switch(ch){
          case 5:   
            /*Debug Mode:
            You can enter your message to be sent and a message to verify, if both the messages are same, Verification is successful
            And You can enter the private-public key pair slots individually as well, if both slot ids are same, verification is successful
            else failed*/
            sha256_hash_t hash5;    
            char sig5[64];
            char pub5[64];
            Serial.print("Private Key Slot: ");
            Serial.println(slot1);
            Serial.print("Public Key Slot: ");
            Serial.println(slot2);
            Serial.print("Your message for verifying: ");
            Serial.println(msg2);
            
            sha256(&hash5, msg1, sizeof(msg1));   //calling the sha256 directly from library
            hexify("\nMessage Digest for message Sent: ", &hash5[0], sizeof(hash5));

            if (0 != ecc.sign(&hash5[0], sizeof(hash5),slot1))   //creating signature and verifying if signature was made or not
              Serial.println("Fail sign");
            else{
              memcpy (sig5, ecc.rsp.getPointer(), sizeof(sig5));
              hexify("Signature1 Created:", (const uint8_t *) sig5, sizeof(sig5));  //NOt deterministic Signature created
             
            }

            //Serial.println(msg2);
            
            if(0!=ecc.hash_verify(msg2, sizeof(msg2),slot2,(const uint8_t *) sig5)){   //Calling the verification function from the library
              delay(100);
             memcpy (pub5, ecc.rsp.getPointer(), sizeof(pub5));  //while verifying, the public key is generated and this stores the buffered value into the variable pub5
             Serial.println();
             hexify("Public Key:", (const uint8_t *) pub5, sizeof(pub5)); 
             Serial.println("\n Verification failed");
             digitalWrite(3, HIGH);
            }
            else{
             memcpy (pub5, ecc.rsp.getPointer(), sizeof(pub5));
             Serial.println();
             hexify("Public Key:", (const uint8_t *) pub5, sizeof(pub5));
             Serial.println("\n Verification Success\n");
             digitalWrite(2,HIGH);  
            }
          break;
          
          case 4:
            sha256_hash_t hash1;
            char sig[64];
            char pub[64];
            char *msg4;
            
             Serial.print("Private-Public KeyPair Slot: ");
            Serial.println(slot1);
            
            strcpy(msg4,msg1);   //taking backup of msg
  
            sha256(&hash1, msg1, sizeof(msg1));
            hexify("\nMessage Digest for message Sent: ", &hash1[0], sizeof(hash1));
            
            if (0 != ecc.sign(&hash1[0], sizeof(hash1),slot1))
              Serial.println("Fail sign");
            else{
              memcpy (sig, ecc.rsp.getPointer(), sizeof(sig));
              hexify("Signature1 Created:", (const uint8_t *) sig, sizeof(sig));
            }

            delay(10);
            //Serial.println(msg4);
            
            if(0!=ecc.hash_verify(msg1, sizeof(msg1),slot1,(const uint8_t *) sig)){
               memcpy (pub, ecc.rsp.getPointer(), sizeof(pub));
               Serial.println();
               hexify("Public Key:", (const uint8_t *) pub, sizeof(pub));
              Serial.println("\n Verification failed");
              digitalWrite(3, HIGH);
  
            }
            else{
              memcpy (pub, ecc.rsp.getPointer(), sizeof(pub));
              Serial.println();
              hexify("Public Key:", (const uint8_t *) pub, sizeof(pub));
              Serial.println("\n Verification Success\n");
              digitalWrite(2,HIGH);  
             }
           break;

          case 3:
           sha256_hash_t hash3;
            char sig3[64];
            Serial.print("Private Key Slot:");
            Serial.println(slot1);

             sha256(&hash3, msg1, sizeof(msg1));
             if (0 != ecc.sign(&hash3[0], sizeof(hash3),slot1))
              Serial.println("Fail sign");
            else{
              memcpy (sig3, ecc.rsp.getPointer(), sizeof(sig3));
              hexify("Signature1 Created:", (const uint8_t *) sig3, sizeof(sig3));
            }
           break;

          case 2:
            sha256_hash_t hash2;
            sha256(&hash3, msg1, sizeof(msg1));
            hexify("\nMessage Digest for message Sent: ", &hash1[0], sizeof(hash1));
          break;
          
         case 1:
            Serial.print("Private Key Slot: ");
            Serial.println(slot1);
            Serial.print("Public Key Slot: ");
            Serial.println(slot2);
            Serial.print("Your second message ");
            Serial.println(msg2);
            break;

        default:
          Serial.println("please enter a valid input");
        }       
    }
}

/*This Function is used to split the input into datas required for the implementation*/

void parseData() {      

    char * strtokIndx;  //index variable

    strtokIndx = strtok(tempChars,",");  //gets the first part 
    ch = atoi(strtokIndx); 
    
    strtokIndx = strtok(NULL, ","); //continues where the previous call left off
    strcpy(msg1, strtokIndx);

    strtokIndx = strtok(NULL, ",");
    slot1 = atoi(strtokIndx);     

    strtokIndx = strtok(NULL, ",");
    slot2 = atoi(strtokIndx);     

    strtokIndx = strtok(NULL, ",");
    strcpy(msg2, strtokIndx);

}

/*This function is used to print the input*/
void showParsedData() {
    Serial.print("Your Choice: ");
    Serial.println(ch);
    Serial.print("Your message: ");
    Serial.println(msg1);
}



