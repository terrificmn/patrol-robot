#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <iostream>

bool IsSleepingSignal = false;


class RollOut {
private:
    ros::Publisher Pub;
    ros::Subscriber Motion_sub;
    ros::Subscriber Sleep_sub;
    ros::NodeHandle Nh;
    
    uint totalCounter = 0;
    double counter = 0.0;
    int switchCnt = 1;
    const double ROTATION = 2.08; // fixed
    double rotation = 0.0;
    const double PI= 3.14159265359;
    const double DISTANCE = 1.5;
    const double LINEAR_SPEED = 0.3;
    // double RotationForOnePointFive = 0.0; // not work as expected
    bool isRotation = false;
    int rotationCount = 1;
    int stopCount = 1;

public:
    //constructor
    RollOut() {
        std::cout << "initialize..." << std::endl;
        Sleep_sub = Nh.subscribe<std_msgs::String>("/test_sleep_signal", 5, &RollOut::sleepingCallback, this);
        Motion_sub = Nh.subscribe<geometry_msgs::Twist>("/cmd_vel", 5, &RollOut::msgCallback, this);

        //Pub = Nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10); //publisher 거북이 테스트
        Pub = Nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10); //차 전용
    }

    void sleepingCallback(const std_msgs::String::ConstPtr &str_msg) {
        
        std::cout << "str_msg.data = " << str_msg->data << std::endl;
        
        if (str_msg->data == "1") {
            IsSleepingSignal = true;
             std::cout << "signal true !"  << std::endl;
        } else {
            IsSleepingSignal = false;
            std::cout << "signal false !"  << std::endl;
        }
    }

    void msgCallback(const geometry_msgs::Twist::ConstPtr &msg) {
        //
        ros::Rate Loop_rate(10);
        
        // double counter = 0.0;
        // uint totalCounter = 0;
        // int switchCnt = 1;
        // const double ROTATION = 2.08; // fixed
        // double rotation = 0.0;
        // const double PI= 3.14159265359;
        // const double DISTANCE = 1.3;
        // const double LINEAR_SPEED = 0.2;
        // // double RotationForOnePointFive = 0.0; // not work as expected
        // bool isRotation = false;
        // int rotationCount = 1;
        // int stopCount = 1;

        while(ros::ok()) {
            geometry_msgs::Twist rect;
            
            rect.linear.x = LINEAR_SPEED; //속도 셋팅
            
            // 시간 구하기 = 거리 / 속도
            double movingTime = DISTANCE / rect.linear.x;
            std::cout << "linear.x = " << rect.linear.x << std::endl;
            ROS_INFO("counter = %lf" , counter);
            std::cout << "moving time = " << movingTime << std::endl;

            std::cout << "signal true/false =" << IsSleepingSignal << std::endl;

            if (IsSleepingSignal == false) {
                // 최초 시작시에는 0으로 초기화  : 카운터는 시간 만큼 올라가고 초기화 
                
                if (counter == 0.0) {
                    rect.linear.x = 0.0;
                    isRotation = false;
                // 가야할 시간보다 카운터가 작으면  전진
                } else if ((double)counter < movingTime) {
                    rect.linear.x = LINEAR_SPEED;
                    std::cout << "MOVING OUT" << std::endl;
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
                        if (totalCounter % 20 == 0) {  // because of 0.1 sec
                            rotation = ROTATION + 0.01;
                            std::cout << "++ MODIFIED" << std::endl;
                        } else {
                            rotation = ROTATION;
                            std::cout << "None MODIFIED " << std::endl;
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

            // IsSleepingSignal true이면
            } else {
                // 살짝 커프를 돈 다음에 멈추는 코드로 진행 중.. 해결해야할 것은 신호가 계속 "1"로 되어 있어서 
                // 신호가 안 들어올 때 
                // 이쪽 else 코드로만 들어오는 것을 해결해야함 
                if(stopCount < 22) {
                    rect.linear.x = 0.2;
                    //rect.angular.z = - 0.52;
                    std::cout << "Signal True: rect.linear.x = " << rect.linear.x << std::endl;
                    
                } else if (stopCount > 22 && stopCount < 40) {
                    rect.linear.x = 0.2;
                    rect.angular.z = - 0.5233;
                     std::cout << "Signal True: rect.linear.x = " << rect.linear.x << std::endl;
                
                } else if (stopCount > 40 && stopCount < 75) {
                    rect.linear.x = 0.1;
                    rect.angular.z = 0.0;
                } else if (stopCount > 75 && stopCount < 95) {
                    rect.linear.x = 0.1;
                    rect.angular.z = 0.4933;
                } else if (stopCount > 95 && stopCount < 130) {
                    rect.linear.x = 0.1;
                    rect.angular.z = 0.0;
                } else {
                    rect.linear.x = 0.0;
                    rect.angular.z = 0.0;
                    std::cout << "Signal True: rect.linear.x = " << rect.linear.x << std::endl;
                    //rotationCount = 1; //reset
                }

                // if (stopCount == 100) {
                //     stopCount = 1;
                // }
                stopCount++;
            }
            
            
            //거리 = 속력 x 시간
            //속도 = 거리 / 시간 예: km/h  m/s
            
            Pub.publish(rect);
            //counter++;
            counter += 0.1;
            ROS_ERROR("counter %lf", counter);
            totalCounter++;
            Loop_rate.sleep();

            // spinOnce를 해줘야지 한번 멈췄다가 subscribe를 잘 하는 듯 하다
            ros::spinOnce();    
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