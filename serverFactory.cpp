#include "serverFactory.h"

void serverFactory::deletelocalserver(){
  if(!Islocalserver()) // localserver is not running
    return;
  delete _localserver; // completely delete and remove local webserver from memory 
  _localserver = NULL;
}
void serverFactory::deletefauxmo(){
  if(!Isfauxmo()) // fauxmo is not running
      return;
  _fauxmo->Stop();
  delete _fauxmo; 
  _fauxmo = NULL;
}
