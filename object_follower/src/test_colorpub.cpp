#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>

int main(int argc, char **argv) {
    ros::init(argc, argv, "test_color_pub");
    // instance
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::Int32MultiArray>("/color_tracker_dummy", 10); // 거북이 테스트용;
    
    ros::Rate Loop_rate(1);
    std_msgs::Int32MultiArray localTest;
    
    localTest.data.clear();
    
    while(ros::ok()) {
        
        localTest.data.push_back(50);
        localTest.data.push_back(20);
        // localTest.data[1] = 20;
        // for (int i=0; i< 100; i++) {
        //     // 그냥 랜덤하게 넣어주기
        //     //localTest.data.push_back(rand() % 255);
        //     localTest.data.push_back(rand() % 255);
        // }
        std::cout << "test published :" << localTest.data[0] << "  ";
        std::cout << "test published :" << localTest.data[1] << std::endl;
        
        pub.publish(localTest);
    }
    
    ros::spinOnce();
    return 0;
}