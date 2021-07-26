#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>

class FollowedColor {
private:
    ros::Subscriber LocSub;
    ros::Subscriber LaserSub;
    ros::Publisher TurtlePub;
    ros::Publisher ScoutPub;
    ros::NodeHandle Nh;
    const int CENTER = 300;
    int x;
    int tempX = 0;
    double xCount = 0.0;
    int approach = CENTER;
    int xDistance;
    double counter = 0.0;

    //pointcloud2 to laser
    double left_a;
    double back_b;
    double right_c;
    double front_d;

public:
    //contructor
    FollowedColor() {
        std::cout << "node started.." << std::endl;
        LocSub = Nh.subscribe<std_msgs::Int32MultiArray>("/color_tracker", 10, &FollowedColor::localCallback, this);
        LaserSub = Nh.subscribe<sensor_msgs::LaserScan>("/scan", 100, &FollowedColor::laserCallback, this);
        //LocSub = Nh.subscribe<std_msgs::Int32MultiArray>("/color_tracker_dummy", 10, &FollowedColor::localCallback, this); //for test
        TurtlePub = Nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10); // 거북이 테스트용
        ScoutPub = Nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10); //real to scout-mini
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
        
        int x = msg->data[0];
        //int y = msg->data[1];

        ros::Rate Loop_rate(10);
        geometry_msgs::Twist goTest;
        
        while(ros::ok()) {
            
            if (tempX == x) { //이전 값과 비교
                   xCount+= 0.1;
                   //std::cout << "same X" << std::endl;;
                } else {
                    xCount = 0.0;
                    //std::cout << "difference X" << std::endl;;
            }
            //just for test without cam (from real topic)
            //테스트중임 변수값 임의로 셋팅~ 아래 2개 테스트 끝나면 삭제할 것
            xCount = 0.0; //shoud be removed when test is done
            front_d = 0.31; //shoud be removed when test is done

            if (xCount > 1.0) {
                std::cout << "x = " << x << std::endl;
                goTest.linear.x = 0;
                goTest.angular.z = 0;

            } else if (front_d < 0.30) { //lidar laser-scan 
                //stop
                goTest.linear.x = 0;
                std::cout << "rover stoped now!" << std::endl;

            } else {   // roll-out
                if (x < CENTER) {
                        xDistance = CENTER - x;
                        goTest.linear.x = 0.1;
                        goTest.angular.z = 0.2;
                        std::cout << "xDistance :" << xDistance << std::endl;
                    if(xDistance == CENTER || xDistance < 10 ) {
                        goTest.angular.z = 0.0;
                        goTest.linear.z = 0.0;
                    }

                    } else {
                        xDistance = x - CENTER;
                        goTest.linear.x = 0.1;
                        goTest.angular.z = -0.2;
                        std::cout << "xDistance :" << xDistance << std::endl;
                        if(xDistance == CENTER || xDistance < 10) {
                            goTest.angular.z = 0.0;
                            goTest.linear.x = 0.0;
                        }
                }
            }
            
            TurtlePub.publish(goTest);
            ScoutPub.publish(goTest);
            Loop_rate.sleep();
            counter+= 0.1;
            tempX = x; //in order to compare 
            ros::spinOnce();

        }
    }

    void laserCallback(const sensor_msgs::LaserScan::ConstPtr &laserMsg) {
        left_a = laserMsg->ranges[180];  //left
        back_b = laserMsg->ranges[laserMsg->ranges.size() / 2]; //back
        right_c = laserMsg->ranges[540];  //right
        front_d = laserMsg->ranges[1]; //front

        std::cout << "laserMsg front: " << laserMsg->ranges[1] << std::endl;
        
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