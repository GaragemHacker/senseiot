
#include "espCrypto.h"

espCrypto esp;
String payload = "test321";

void setup()
{
  String enc, dec;

  Serial.begin(115200);//Inicia a comunicação serial.


  esp.setKey("1234567890123456");//Adiciona a chave para encriptação, mantenha em segredo!
  

  enc = esp.encrypt(payload);//A função encrypt retorna uma string encriptada.
  Serial.println(enc);//Printa o criptograma (mensagem encriptada).

  dec = esp.decrypt(enc);//A função decrypt retorna uma string desencriptada.
  Serial.println(dec);//Printa a mensagem desencriptada.
  
  
}


void loop()
{

}
