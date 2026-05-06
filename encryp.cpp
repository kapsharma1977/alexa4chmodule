#include "encryp.h"

encryp::encryp(){
  key = (char *)malloc((LEN+1)*sizeof(char)); // one for null char
  ////key = (char *)calloc((LEN+1)*sizeof(char),sizeof(char)); // one for null char
  if(key == NULL)
    return;
  String mac = WiFi.macAddress();
  char tmp[7];
  itoa(ESP.getChipId(),tmp,16);
  key[0] = tmp[0];
  key[1] = tmp[1];
  uint8_t ik = 2;
  for(uint8_t i = 0; i < mac.length() && ik < (2 + 12); i++){
    if(mac.charAt(i) != ':'){
      key[ik] = mac.charAt(i);
      ik++;
    }
  }
  key[14] = tmp[2];
  key[15] = tmp[3];
  key[LEN] = '\0';
}
bool encryp::xor_encrypt(char *s, uint8_t len){
  if(!IsKey())
    return false;
  if(s != NULL && len <= LEN && len > 0){
    ////str = (char *)malloc(len*sizeof(char));
    //Serial.printf("Length %d\n",len);
    for(uint8_t i = 0; i < len; i++){
      s[i] = xor_encrypt(s[i],i);
    }
    //Serial.printf("len %d\n",len);
    return true;
  }
  else
    return false;
}
bool encryp::xor_decrypt(char *crypted, uint8_t len){// member *crypted will be decrypted by this fn, so do not call clear()
  if(!IsKey())
    return false;
  if(crypted != NULL && len <= LEN && len > 0){
    //free(dstr);ESP.getFreeHeap();
    //dstr = (char *)malloc((len + 1)*sizeof(char));
    for(uint8_t i = 0; i < len; i++){
      crypted[i] = xor_decrypt(crypted[i],i);
    }
    //dstr[len] = '\0';
    //Serial.printf("string =%s;len =%d\n",dstr,len);
    //Serial.println(String(dstr));
    return true;
  }
  else
    return false;
}