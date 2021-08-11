// the setup function runs once when you press reset or power the board

/*#include<ros.h>
#include<std_msgs/String.h>

ros::NodeHandle nh;  //노드핸들설정
std_msgs::String str_msg;   //'str_msg 로 메시지설정
ros::Publisher chatter("chatter", &str_msg);
*/

char input_data;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  //nh.initNode();
  //nh.advertise(chatter);
  
}

// the loop function runs over and over again forever
void loop() {
  while(Serial.available())
  {
    input_data = Serial.read();
  }

  if(input_data == '1')
  {
    digitalWrite(LED_BUILTIN, HIGH); // led_on
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW); // led_off
    delay(1000);
    input_data = '2';
    /*str_msg.data = "1";
    chatter.publish(&str_msg);
    nh.spinOnce();
    delay(1000);
    */
    
  }
}