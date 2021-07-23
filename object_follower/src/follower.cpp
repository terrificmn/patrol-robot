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
    const int CENTER = 255;
    int x = 300;
    int approach = CENTER;
    int xDistance;
    double counter = 0.0;

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
        std::cout << "ok received - data 1 :" << msg->data[0] << std::endl;
        std::cout << "ok received - data 2 :" << msg->data[1] << std::endl;
        
        //int x = msg->data[0];
        //int y = msg->data[1];

        ros::Rate Loop_rate(10);
        geometry_msgs::Twist goTest;
        
        while(ros::ok()) {
            
            // center가 255라고 가정
            if (counter < 20.0) {
                if (x < CENTER) {
                xDistance = CENTER - x;
                goTest.linear.x = 0.1;
                goTest.angular.z = 0.2;
                if(xDistance == CENTER || xDistance < 5 ) {
                    goTest.angular.z = 0.0;
                }

                } else {
                    xDistance = x - CENTER;
                    goTest.linear.x = 0.1;
                    goTest.angular.z = -0.2;
                    if(xDistance == CENTER || xDistance < 5) {
                        goTest.angular.z = 0.0;
                    }
            
                }

            } else {
                goTest.linear.x = 0.0;
            }
            
            TurtlePub.publish(goTest);
            Loop_rate.sleep();
            counter+= 0.1;
            ros::spinOnce();

        }
    }


};

int main(int argc, char **argv) {
    ros::init(argc, argv, "follow_node");
    // instance
    FollowedColor FollowObj;

    //메인에서 직접 퍼블리시 할 경우
   // geometry_msgs::Twist goTest;
   // FollowObj.getPub().publish(goTest);

    ros::spin();
    return 0;
}