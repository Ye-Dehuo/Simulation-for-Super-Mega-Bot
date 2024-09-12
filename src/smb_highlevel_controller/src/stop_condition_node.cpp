#include <ros/ros.h>
#include "smb_highlevel_controller/SmbHighlevelController.hpp"
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Imu.h>

  std::string scanTopic_;

  std::string imuTopic_;

  std::string emergency_stop_service_;

  ros::ServiceClient emergency_stop_service_client;

  float emergencyStopRadius_;

  float collision_threshold_;

  bool prior_collision_;

  bool post_collision_;

  bool emergencyStopRadiusUpdate_ = false;

bool readParameters(ros::NodeHandle& nodeHandle)
{
  bool success = true;

  success &= nodeHandle.getParam(
        "/stop_condition/scan_topic", scanTopic_);

  success &= nodeHandle.getParam(
        "/stop_condition/imu_topic", imuTopic_);

  success &= nodeHandle.getParam(
        "/stop_condition/emergency_stop_radius", emergencyStopRadius_);

  success &= nodeHandle.getParam(
        "/stop_condition/collision_threshold", collision_threshold_);

  success &= nodeHandle.getParam(
         "/stop_condition/service_name", emergency_stop_service_);

  success &= nodeHandle.getParam(
         "/stop_condition/prior_collision", prior_collision_);

  success &= nodeHandle.getParam(
         "/stop_condition/post_collision", post_collision_);

      return success;
}

void scanCallback(const sensor_msgs::LaserScan::ConstPtr& message)
{

  auto minDistanceIterator = std::min_element(message->ranges.begin(), message->ranges.end());
  auto distance = *minDistanceIterator;

// Stop radius limit
if(emergencyStopRadius_ < message->range_min && !emergencyStopRadiusUpdate_ ){
  emergencyStopRadius_ = message->range_min;
  emergencyStopRadiusUpdate_ = true;
}

// Is emergency stop needed
  if(distance <= emergencyStopRadius_)
  {
  std_srvs::SetBool service;
  service.request.data = false;

// Call emergency stop service through service client
  if(!emergency_stop_service_client.call(service)){
  ROS_ERROR("Failed to call emergency stop service");
  ros::requestShutdown();
  }

  }


}

void imuCallback(const sensor_msgs::Imu::ConstPtr& message)
{

  // if((message->header.stamp.sec >1) && (message->linear_acceleration.x < collision_threshold_))// Ignore the initial moment

// Is emergency stop needed
  if(message->linear_acceleration.x < collision_threshold_)
  {
  std_srvs::SetBool service;
  service.request.data = false;

// Call emergency stop service through service client
  if(!emergency_stop_service_client.call(service)){
  ROS_ERROR("Failed to call emergency stop service");
  ros::requestShutdown();
  }

  }


}

int main(int argc, char** argv) {
  ros::init(argc, argv, "stop_condition");
  ros::NodeHandle nodeHandle;

  if (!readParameters(nodeHandle)) {
        ROS_ERROR("Could not read parameters.");
        ros::requestShutdown();
      }
  else {ROS_INFO("Successfully launched node.");
  }

  ros::Duration(1).sleep();

  ros::Subscriber scanSubscriber_;

  ros::Subscriber imuSubscriber_;

  emergency_stop_service_client = nodeHandle.serviceClient<std_srvs::SetBool>(emergency_stop_service_);

// Set to prior collision mode
  if(prior_collision_){
  scanSubscriber_ = nodeHandle.subscribe(scanTopic_, 10, scanCallback);
  }

// Set to post collision mode
  if(post_collision_){
  imuSubscriber_ = nodeHandle.subscribe(imuTopic_, 10, imuCallback);
  }

  ros::spin();

  return 0;
}
