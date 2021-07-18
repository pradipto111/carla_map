#include <iostream>
#include <pcl/point_types.h>
#include <pcl/common/io.h>
#include <pcl/io/pcd_io.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf2_msgs/TFMessage.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>

#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_conversions/pcl_conversions.h>

pcl::PointCloud<pcl::PointXYZ> final_cloud;

ros::Publisher pcd_publisher;
int i=0;

void stitch_callback(const sensor_msgs::PointCloud2::ConstPtr& msg){
/*
	pcl::PointCloud<pcl::PointXYZ> curr_cloud;
	pcl::fromROSMsg(*msg,curr_cloud);
	if(i==0) final_cloud = curr_cloud;
	else final_cloud += curr_cloud;
	i++;
*/
	ROS_INFO_STREAM(msg->header);
	
}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "stich_pcd");
	ros::NodeHandle nh;
	
	pcd_publisher = nh.advertise<sensor_msgs::PointCloud2>("/final_cloud",100);
	ros::Subscriber pcd_subscriber = nh.subscribe("/carla/vehicle/086/lidar/front/point_cloud",10, stitch_callback);
	ros::spin();
	
	pcl::io::savePCDFileASCII ("final_pcd.pcd", final_cloud);
	return 0;
}
