#include <ros/ros.h>

//PCL header파일들
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
// 필터 헤더파일
#include <pcl/filters/voxel_grid.h>

// pcl::PointCloud<pcl::PointXYZ> 를 사용
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/segmentation/sac_segmentation.h>

#include <iostream>
using std::string;

ros::Publisher pub; //전역으로 선언


void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud_msg) {
    // Declare the format of the point cloud that stores the raw data and the filtered data
    // Container for original & filtered data
    pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2; //Original point cloud data format
    pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
    pcl::PCLPointCloud2 cloud_filtered;//Storage filtered data format

    // Convert to PCL data type. Data format converted to point cloud in PCL
    pcl_conversions::toPCL(*cloud_msg, *cloud);

    // Perform the actual filtering to perform a filtering process
    pcl::VoxelGrid<pcl::PCLPointCloud2> Sor; //필터 오브젝트 만들기 Sor
    Sor.setInputCloud (cloudPtr); //Set the input filter to filter the point cloud to be filtered
    Sor.setLeafSize (0.01, 0.01, 0.01); //Set the voxel size created when filtering to 1cm cube //0.1 기준
    Sor.filter (cloud_filtered);//Execute filter processing, store output cloud_filtered

    // Convert to ROS data type. // Then convert the filtered point cloud data format to the data format under ROS and publish it.
    sensor_msgs::PointCloud2 pc_output;//Declared output point cloud format
    pcl_conversions::moveFromPCL(cloud_filtered, pc_output);//The first parameter is the input, followed by the output

    // pc_output 이란 토픽으로 퍼블리시하기
    pub.publish (pc_output);
}



int main (int argc, char** argv) {
    // Initialize ROS
    ros::init (argc, argv, "mypcl_test");//Declare the name of the node
    ros::NodeHandle nh;

    // Create a ROS subscriber for the input point cloud
    // Create a subscription node for accepting point cloud data
    ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud2>("/camera/depth/color/points", 1, cloud_cb);

    // Create a ROS publisher for the output point cloud
    //Create a publishing node for ROS
    pub = nh.advertise<sensor_msgs::PointCloud2>("/pc_output", 1);
    
    ros::spin ();
}






