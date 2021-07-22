#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>

int main(int argc, char **argv) {
    ros::init(argc, argv, "test_color_pub");
    // instance
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::Int32MultiArray>("/color_tracker_test", 10); // 거북이 테스트용;
    
    ros::Rate Loop_rate(1);
    std_msgs::Int32MultiArray localTest;
    
    /*
    뭔가 잘못됨 수정해야함
    erminate called after throwing an instance of 'ros::InvalidNameException'
    what():  Character [-] at element [4] is not valid in Graph Resource Name [test-color-pub].  Valid characters are a-z, A-Z, 0-9, / and _.
    Aborted (core dumped)
    이부분
    */

    while(ros::ok()) {
        localTest.data[0] = 10;
        localTest.data[1] = 20;
        std::cout << "published :" << localTest.data[0] << std::endl;
        std::cout << "published :" << localTest.data[1] << std::endl;
        pub.publish(localTest);
    }
    
    ros::spin();
    return 0;
}