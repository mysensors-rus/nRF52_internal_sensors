/**
Пример испльзования ШИМ (диммера) с возможностью изменения частоты

*/

#define MY_DEBUG
#define MY_RADIO_NRF5_ESB
#define MY_PASSIVE_NODE
#define MY_NODE_ID 33

#define TEMP_CHILD_ID 1   // Id of the sensor child
#define VOLTAGE_CHILD_ID 2   // Id of the sensor child

#define heartbeat_timeout 60000

#include <MySensors.h>
MyMessage msg_temp(TEMP_CHILD_ID, V_TEMP);
MyMessage msg_voltage(VOLTAGE_CHILD_ID, V_VOLTAGE);



void preHwInit() {

}

void before()
{

}

void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Internal Sensors", "1.0");
  wait(50);
  // Register all sensors to gw (they will be created as child devices)
  present(TEMP_CHILD_ID, S_TEMP);
  wait(50);
  present(VOLTAGE_CHILD_ID, S_MULTIMETER);  
}

void setup() {

  //NRF_CLOCK->INTENSET=B11;  //enable interrupts for EVENTS_HFCLKSTARTED and  EVENTS_LFCLKSTARTED
  NRF_TEMP->TASKS_STOP;
  NRF_TEMP->EVENTS_DATARDY=0;

  NRF_TEMP->INTENSET=1;  //enable EVENTS_DATARDY temperature interrupt
  while (!(NRF_TEMP->INTENSET==1)) {} //wait until completed

}

uint32_t blinkCounter=0;
uint32_t rawTemperature=0;
float celsius=0.0;  //rawTemperature converted to Celsius

uint32_t lasttime=0;
void loop() {
if (millis() > (lasttime + heartbeat_timeout)){
  sendHeartbeat(true);
  lasttime = millis();
}

  NRF_TEMP->TASKS_START=1;
  while (!(NRF_TEMP->EVENTS_DATARDY)) {} //wait until temperature measurement is complete
 
  rawTemperature=NRF_TEMP->TEMP;
  celsius=((float)rawTemperature)/4.0;

  
send(msg_temp.set(celsius, 1));
wait(100);
send(msg_voltage.set(((float)hwCPUVoltage()/1000),2));
wait(900);



}
