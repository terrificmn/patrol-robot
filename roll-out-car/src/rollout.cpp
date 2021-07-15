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
        //Pub = Nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10); //publisher
        Pub = Nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10); //publisher
    }

    void msgCallback(const geometry_msgs::Twist::ConstPtr &msg) {
        //
        ros::Rate Loop_rate(10);
        //int counter = 0;
        double counter = 0.0;
        uint totalCounter = 0;
        int switchCnt = 1;
        const double ROTATION = 2.08; // fixed
        double rotation = 0.0;
        const double PI= 3.14159265359;
        const double DISTANCE = 1.0;
        const double LINEAR_SPEED = 0.2;
        // double RotationForOnePointFive = 0.0; // not work as expected
        bool isRotation = false;
        int rotationCount = 1;

        while(ros::ok()) {
            geometry_msgs::Twist rect;
            
            rect.linear.x = LINEAR_SPEED; //속도 셋팅
            
            // 시간 구하기 = 거리 / 속도
            double movingTime = DISTANCE / rect.linear.x;
            std::cout << "linear.x = " << rect.linear.x << std::endl;
            ROS_INFO("counter = %lf" , counter);
            std::cout << "moving time = " << movingTime << std::endl;

            // 최초 시작시에는 0으로 초기화  : 카운터는 시간 만큼 올라가고 초기화 
            if (counter == 0.0) {
                rect.linear.x = 0.0;
                isRotation = false;
            // 가야할 시간보다 카운터가 작으면  전진
            } else if ((double)counter < movingTime) {
                rect.linear.x = LINEAR_SPEED;
            // 카운터가 시간보다 커지면 다시 초기화 및 각도 바꾸기
            
            } else {
                rect.linear.x = 0.0;
                isRotation = true;
                
                //RotationForOnePointFive = (PI / rotation) / 15;  // 1.5sec
            } 

            // only works when rotation is needed
            if (isRotation == true) {
                std::cout << "ROTATION TRUE/FALSE = " << isRotation << std::endl;
                if(rotationCount % 11 != 0) {
                    ROS_INFO("totalcount %d", totalCounter);
                    // rotation slightly modified per 3 counts 
                    if (totalCounter % 30 == 0) {  // because of 0.1 sec
                        rotation = ROTATION + 0.01;
                    } else {
                        rotation = ROTATION;
                    }
                    
                    rect.angular.z = (PI / rotation);
                    //RotationForOnePointFive += RotationForOnePointFive;
                    //rect.angular.z += RotationForOnePointFive; //divided by 15 (1.5 sec)
                    ROS_INFO("angular z = %lf", rect.angular.z);
                    
                    rotationCount++;
                    std::cout << "rotationCount = " << rotationCount << std::endl;
                } else {
                    counter = 0.0; //카운터 초기화    
                    rotationCount = 1; //reset
                    isRotation = false; //reset
                }
            }
            
            //거리 = 속력 x 시간
            //속도 = 거리 / 시간 예: km/h  m/s
            
            Pub.publish(rect);
            //counter++;
            counter += 0.1;
            totalCounter++;
            
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