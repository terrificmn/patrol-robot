#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>

class TestSleep {
public:
    ros::Publisher Pub;
    // ros::Subscriber Clock_sub; // timer 만들어보기
    ros::NodeHandle Nh;

    TestSleep() {
        std::cout << "test started" << std::endl;
        Pub = Nh.advertise<std_msgs::String>("/test_sleep_signal", 5, 10);
        // Clock_sub = Nh.subscribe("/clock", 1,  &TestSleep::sub_clock, this); //timer용

    }

};


int main(int argc, char **argv) {
    ros::init(argc, argv, "test_signal");

    TestSleep SleepOjb;

    ros::Rate Loop_rate(10);
    
    int count = 0;

    while(ros::ok()) {
        std_msgs::String sleepStatus;
        
        sleepStatus.data = "1";    
        // if (count < 100) {
        //     sleepStatus.data = "1";
        // } else if (count > 100 && count < 200) {
        //     sleepStatus.data = "0";  //publish를 끄게 되면 계속 1이 남아 있는 듯하다
        // }

        // if (count == 200) {
        //     count = 0;
        // }

        SleepOjb.Pub.publish(sleepStatus);
        Loop_rate.sleep();
        count++;
    }

    return 0;
}


//////////////// 이하는 //////////////////////////
/// 아두이노 코드 in sketchbook IDE

// #include <ros/ros.h>
// #include <std_msgs/String.h>
// #include <std_msgs/UInt16.h>

// #define BUTTON 8
// #define LED 13

// ros::NodeHandle node_handle;

// std_msgs::String std_msg;
// ros::Publisher signal_pub("sleepy_signal", &std_msg);

// void setup() {
//     node_handle.initNode();
//     node_handle.advertise(signal_pub);
// }

// void loop() { 

//     std_msg.data = "1";
//     signal_pub.publish( &std_msg );
//     node_handle.spinOnce();

//     delay(100);
// }