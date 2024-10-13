#pragma once

// ROS
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <iterator>
#include <time.h>
#include <algorithm>
#include <visualization_msgs/Marker.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <std_srvs/SetBool.h>
#include <math.h>

namespace smb_highlevel_controller {

/*!
 * Class containing the Husky Highlevel Controller
 */
class SmbHighlevelController {
public:
	/*!
	 * Constructor.
	 */
	SmbHighlevelController(ros::NodeHandle& nodeHandle);

	/*!
	 * Destructor.
	 */
	virtual ~SmbHighlevelController();

private:

	float xVel_;

	float kpAng_;

	float kpVel_;

	float kdAng_;

	float reduced_kpAng_;

	float kiVel_;

	float angular_prev_error;

  float angular_error;

  float linear_prev_error;

  float linear_error;

  float derivative;

  float intergral;

  float kpAngChangeRadius_;

	bool isStart_;

	bool readParameters();

	void scanCallback(const sensor_msgs::LaserScan::ConstPtr& message);

	void pclCallback(const sensor_msgs::PointCloud2::ConstPtr& message);

	void getGoalPose(geometry_msgs::PoseStamped& goalPose, const sensor_msgs::LaserScan::ConstPtr& message);

	void moveToGoal(const geometry_msgs::PoseStamped& goalPose);

	void showVisMarker(geometry_msgs::PoseStamped& goalPose);

	void transformOdom(geometry_msgs::PoseStamped& pose, geometry_msgs::PoseStamped& targetPose);

	bool startStopCallback(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res);

	ros::NodeHandle nodeHandle_;

	ros::Subscriber scanSubscriber_;

  ros::Subscriber pclSubscriber_;

  ros::Publisher twistPublisher_;

  ros::Publisher visMarkerPublisher_;

	std::string scanTopic_;

	tf2_ros::Buffer tfBuffer_;

	tf2_ros::TransformListener tfListener_;

	std::string start_stop_service_;

	ros::ServiceServer start_stop_service_server_;

};

} /* namespace */
