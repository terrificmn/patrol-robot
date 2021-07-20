#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
using namespace std;

class GoCircle {
private:
    ros::Publisher Pub;
    ros::Subscriber Sub;
    ros::NodeHandle Nh;
public:
    GoCircle() {
        cout << "initialize the circle movement!" << endl;
        Sub = Nh.subscribe<geometry_msgs::Twist>("/cmd_vel", 5, &GoCircle::msgCallback, this);
        Pub = Nh.advertise<geometry_msgs::Twist>("/cmd_vel", 5);
        // Pub = Nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 5);
    }

    void msgCallback(const geometry_msgs::Twist::ConstPtr &msg) {
    
        ros::Rate Loop_rate(10);

        int multipliedInputNum = 1.0;  // 사용자 거리 입력
        int counter = 0;
        const double PI= 3.14159265359;
        const double R = PI / 2;
        double spinSpeed = 30;  // 한 바퀴 돌아가는 시간을 정해줌 -- Loop_rate(10)이면 잘 되는데 (1) 오차가 큼
        // 1.0 --> spinSpeed 10 = linear.x = 0.98 === too fast 
        // 1.0 --> spinSpeed 30 = linear.x = 0.3?

        while(ros::ok()) {
            geometry_msgs::Twist cir;
            
            cir.angular.z =  (2.0 * PI) / spinSpeed;
            cir.linear.x = (multipliedInputNum * (2.0 * PI * R)) / spinSpeed;
            
            cout << "linear.x = " << cir.linear.x << endl;
            // 각도는 2 PI == 2 PI가 360도가 된다
            // 거리는 2 PI R  == R 은 원주율의 반절 180도가 R 
            
            //거리 = 속력 x 시간
            //속도 = 거리 / 시간 예: km/h  m/s

            Pub.publish(cir);
            counter++;
            Loop_rate.sleep();
        }
    
    }
};




int main (int argc, char** argv) {
    ros::init(argc, argv, "roll_out_circle");
    
    GoCircle GoCircleObj;

    ros::spin();

    return 0;
}