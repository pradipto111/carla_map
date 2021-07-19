#include <iostream>
#include <pcl/point_types.h>
#include <pcl/common/io.h>
#include <pcl/io/pcd_io.h>
#include <ros/ros.h>
#include <pcl_ros/transforms.h>
#include <pcl/common/transforms.h>
#include <pcl_ros/point_cloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf2_msgs/TFMessage.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>

//#include <pcl_ros/impl/transfoms.hpp>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_conversions/pcl_conversions.h>

pcl::PointCloud<pcl::PointXYZ> final_cloud;
geometry_msgs::TransformStamped global_tf_data;
ros::Publisher tf_publisher;
int i=0;


void tf_callback(const tf2_msgs::TFMessage::ConstPtr& tf){
	tf2_msgs::TFMessage tf_ = *tf;
	tf_publisher.publish(tf_.transforms[0]);
}


void pcl_callback(const sensor_msgs::PointCloud2::ConstPtr& pcl_cloud){
	tf2_ros::Buffer tfBuffer;
  	tf2_ros::TransformListener tfListener(tfBuffer);
  	
  	geometry_msgs::TransformStamped transformStamped;
	try{
		transformStamped = tfBuffer.lookupTransform("86", (pcl_cloud->header).frame_id,ros::Time(0), ros::Duration(3.0));
		sensor_msgs::PointCloud2 transform_output;	    
		pcl_ros::transformPointCloud("86",transformStamped.transform,*pcl_cloud,transform_output);
		pcl::PointCloud<pcl::PointXYZ> curr_cloud;
		pcl::fromROSMsg(transform_output,curr_cloud);
		if(i==0) final_cloud = curr_cloud;
		else final_cloud += curr_cloud;
		i++;
		ROS_INFO_STREAM("Transform added to final cloud.");
	}catch (tf2::TransformException &ex) {
    		ROS_INFO_STREAM("Could not transform!");
  	}

	
}





int main(int argc, char** argv)
{
	ros::init(argc, argv, "stitch_pcd");
	ros::NodeHandle nh;
	
	tf_publisher = nh.advertise<geometry_msgs::TransformStamped>("/tf_data",10);
	ros::Subscriber tf_subscriber = nh.subscribe("/tf", 1, tf_callback);
	
	//curr_cloud_publisher = nh.advertise<sensor_msgs::PointCloud2>("/transformed_pcl", 10);
	//original_cloud_publisher = nh.advertise<sensor_msgs::PointCloud2>("/untransformed_pcl", 10);
	
	
	ros::Subscriber pcl_subscriber = nh.subscribe("/carla/vehicle/086/lidar/front/point_cloud", 1, pcl_callback);
	ros::spin();
	return 0;
}
