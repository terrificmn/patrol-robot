#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

// global
ros::Publisher pub;

void msgCallback(const nav_msgs::Odometry::ConstPtr& msg) {
    nav_msgs::Odometry odom;
    ros::Rate loop_rate(20);
    
    while (ros::ok()) {
        odom = *(msg);
        odom.pose.pose.position.z = -0.4;
        
        pub.publish(odom);
        loop_rate.sleep(); 
    }
    
}

int main (int argc, char** argv) {
    ros::init(argc, argv, "filtering_odom");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/t265/odom/sample", 1, msgCallback); 
    pub = nh.advertise<nav_msgs::Odometry>("t265/odom/sample1", 1); // move_base에서 사용할 예정
    ros::spin();

}