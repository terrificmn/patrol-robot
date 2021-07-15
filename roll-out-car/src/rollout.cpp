#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>

class RollOut {
private:
    ros::Publisher Pub;
    ros::Subscriber Motion_sub;
    ros::NodeHandle Nh;
public:
    //constructor
    RollOut() {
        std::cout << "initialize..." << std::endl;
        Motion_sub = Nh.subscribe<geometry_msgs::Twist>("/cmd_vel", 5, &RollOut::msgCallback, this);

        //pub = nh.advertise<>("/", 5, 0)
        Pub = Nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10); //publisher
    }

    void msgCallback(const geometry_msgs::Twist::ConstPtr &msg) {
        //
        ros::Rate Loop_rate(1);
        int counter = 0;
        int switchCnt = 1;
        const double PI= 3.14159265359;
        const double INPUT_LENGTH = 5.0;

        while(ros::ok()) {
            geometry_msgs::Twist tri;
            
            tri.linear.x = 0.5; //속도 셋팅
            
            // 시간 구하기 = 거리 / 속도
            double movingTime = INPUT_LENGTH / tri.linear.x;

            // 최초 시작시에는 0으로 초기화  : 카운터는 시간 만큼 올라가고 초기화 
            if (counter == 0) {
                tri.linear.x = 0.0;
            // 가야할 시간보다 카운터가 작으면  전진
            } else if ((double)counter < movingTime) {
                tri.linear.x = 0.5;
            // 카운터가 시간보다 커지면 다시 초기화 및 거북이 각도 바꾸기
            } else {
                tri.linear.x = 0.0;
                tri.angular.z += (PI / 2.0);
                counter = 0; //카운터 초기화
            } 
            
            //거리 = 속력 x 시간
            
            //속도 = 거리 / 시간 예: km/h  m/s

            Pub.publish(tri);
            counter++;
            Loop_rate.sleep();
        }
    }
};



int main(int argc, char **argv) {
    ros::init(argc, argv, "roll_out");
    // 생성
    RollOut RolloutObj;

    ros::spin();


    return 0;
}