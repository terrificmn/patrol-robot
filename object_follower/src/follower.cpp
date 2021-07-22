#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>
#include <geometry_msgs/Twist.h>


class FollowedColor {
private:
    ros::Subscriber LocSub;
    ros::Publisher TurtlePub;
    ros::Publisher ScoutPub;
    ros::NodeHandle Nh;

public:
    //contructor
    FollowedColor() {
        std::cout << "node started.." << std::endl;
        LocSub = Nh.subscribe<std_msgs::Int32MultiArray>("/color_tracker_test", 10, &FollowedColor::localCallback, this);
        TurtlePub = Nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10); // 거북이 테스트용
        //ScoutPub = Nh.advertise<geometry_msgs/Twist>("/cmd_vel", 5); //real to scout-mini
    }

    //getter
    ros::Publisher getPub() {
        return TurtlePub;
    }

    //칼라 좌표 받기
    void localCallback(const std_msgs::Int32MultiArray::ConstPtr &msg) {
        // to do
        std::cout << "ok received - data 1 :" << msg->data[0];
        std::cout << "ok received - data 2 :" << msg->data[1];
    }


};

int main(int argc, char **argv) {
    ros::init(argc, argv, "follow_node");
    // instance
    FollowedColor FollowObj;
    ros::Rate Loop_rate(1);
    geometry_msgs::Twist goTest;
    
    //pub = FollowObj.getPub();

    while(ros::ok()) {
        goTest.linear.x = 0.5;
        FollowObj.getPub().publish(goTest);
    }

    ros::spin();
    return 0;
}