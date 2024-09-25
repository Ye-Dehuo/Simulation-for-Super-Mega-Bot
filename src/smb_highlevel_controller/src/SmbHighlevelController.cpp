#include <smb_highlevel_controller/SmbHighlevelController.hpp>

// STD
#include <string>

namespace smb_highlevel_controller {

SmbHighlevelController::SmbHighlevelController(ros::NodeHandle& nodeHandle) :
  nodeHandle_(nodeHandle), tfListener_(tfBuffer_)
{
  if (!readParameters()) {
      ROS_ERROR("Could not read parameters.");
      ros::requestShutdown();
    }

  scanSubscriber_ = nodeHandle_.subscribe(scanTopic_, 10, &SmbHighlevelController::scanCallback, this);// subscribe /scan topic

  pclSubscriber_ = nodeHandle_.subscribe("/rslidar_points", 10, &SmbHighlevelController::pclCallback, this);

  twistPublisher_ = nodeHandle_.advertise<geometry_msgs::Twist>("/cmd_vel",10);

  visMarkerPublisher_ = nodeHandle_.advertise<visualization_msgs::Marker>("/visualization_marker", 10);

  start_stop_service_server_ = nodeHandle_.advertiseService(start_stop_service_, &SmbHighlevelController::startStopCallback, this);// Service server to start and stop SMB robot

  ROS_INFO("Successfully launched node.");

}
SmbHighlevelController::~SmbHighlevelController()
{
}

bool SmbHighlevelController::readParameters()
{
  bool success = true;

  success &= nodeHandle_.getParam(
        "/smb_highlevel_controller/scan_topic", scanTopic_);

  success &= nodeHandle_.getParam(
        "/smb_highlevel_controller/x_vel", xVel_);

  success &= nodeHandle_.getParam(
        "/smb_highlevel_controller/p_gain_ang", kpAng_);

  success &= nodeHandle_.getParam(
        "/smb_highlevel_controller/p_gain_vel", kpVel_);

  success &= nodeHandle_.getParam(
                "/smb_highlevel_controller/start_robot", isStart_);

  success &= nodeHandle_.getParam(
                  "/smb_highlevel_controller/service_name", start_stop_service_);// Load service name

      return success;

}

void SmbHighlevelController::scanCallback(const sensor_msgs::LaserScan::ConstPtr& message)// the callback for /scan subscriber
{
  geometry_msgs::PoseStamped goalPose,goalPoseOdom;
  getGoalPose(goalPose, message);
  moveToGoal(goalPose);
  transformOdom(goalPose, goalPoseOdom);
  showVisMarker(goalPoseOdom);
}

void SmbHighlevelController::pclCallback(const sensor_msgs::PointCloud2::ConstPtr& message)// output the number of points in 3D point cloud
{
  // ROS_INFO_STREAM("Number of points in 3D cloud: [" << message->width * message->height <<"]");
}

void SmbHighlevelController::getGoalPose(geometry_msgs::PoseStamped& goalPose, const sensor_msgs::LaserScan::ConstPtr& message)// output the smallest distance of laser scan; output the position of the pillar
{
  auto minDistanceIterator = std::min_element(message->ranges.begin(), message->ranges.end());
  auto distance = *minDistanceIterator;
  auto minDistanceIndex = std::distance(message->ranges.begin(), minDistanceIterator);
  auto ang = message->angle_min + message->angle_increment * minDistanceIndex;

  goalPose.header.frame_id = "rslidar";// goalPose in rslidar frame
  goalPose.header.stamp = ros::Time(0);
  goalPose.pose.position.x = distance * cos(ang);
  goalPose.pose.position.y = distance * sin(ang);
  goalPose.pose.position.z = 0;
  goalPose.pose.orientation.x = 0;
  goalPose.pose.orientation.y = 0;
  goalPose.pose.orientation.z = 0;
  goalPose.pose.orientation.w = 1;

  ROS_INFO_STREAM("Minimum range [m]: [" << distance << "]");
  // ROS_INFO_STREAM("Angle from the robot [rad]: [" << ang << "]");
}

void SmbHighlevelController::moveToGoal(const geometry_msgs::PoseStamped& goalPose)
{
  geometry_msgs::Twist velMsg;

  if(isStart_){

  velMsg.linear.x = xVel_;// constant linear velocity

  // velMsg.linear.x = kpVel_ * sqrt(pow(goalPose.pose.position.x, 2) + pow(goalPose.pose.position.y, 2));// P controller for linear velocity

  velMsg.angular.z = kpAng_ * atan2(goalPose.pose.position.y, goalPose.pose.position.x);// P controller for angular velocity

  }

  twistPublisher_.publish(velMsg);// publish twist command to SMB

  ROS_INFO_STREAM("velMsg.linear.x [m/s]: [" << velMsg.linear.x << "]");
  ROS_INFO_STREAM("velMsg.angular.z [rad/s]: [" << velMsg.angular.z << "]");
}

void SmbHighlevelController::transformOdom(geometry_msgs::PoseStamped& pose, geometry_msgs::PoseStamped& targetPose) // ensure the showVisMarker() function correctly displays the position of the target point 
{
  try{
    targetPose = tfBuffer_.transform(pose, "odom", ros::Duration(0));// transform the coordinate frame of goalPose from rslidar to odom
  } catch(tf2::TransformException &exception){
    ROS_WARN("%s", exception.what());
  }
}

void SmbHighlevelController::showVisMarker(geometry_msgs::PoseStamped& goalPose) // show a target point
{
  visualization_msgs::Marker pillarMarker;

  pillarMarker.header.frame_id = "odom";// publish the point in odom frame
  pillarMarker.header.stamp = ros::Time(0);
  pillarMarker.ns = "smb_highlevel_controller";
  pillarMarker.id = 0;
  pillarMarker.type = visualization_msgs::Marker::SPHERE;
  pillarMarker.action = visualization_msgs::Marker::ADD;
  pillarMarker.pose = goalPose.pose;
  pillarMarker.scale.x = 0.1;
  pillarMarker.scale.y = 0.1;
  pillarMarker.scale.z = 0.1;
  pillarMarker.color.a = 1.0;
  pillarMarker.color.r = 0.0;
  pillarMarker.color.g = 1.0;
  pillarMarker.color.b = 0.0;

  visMarkerPublisher_.publish(pillarMarker);// show the estimated position of the pillar

}

bool SmbHighlevelController::startStopCallback(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res)
{
  res.success = true;
  if(isStart_ != req.data){
  isStart_ = req.data;
  res.message = std::string("Set the robot to ") + (isStart_ ? "start" : "stop");
  }
  return res.success;
}

} /* namespace */
