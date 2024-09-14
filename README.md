# Simulation for Super Mega Bot
This repository is based on the ETH courses [Programming for Robotics - ROS](https://rsl.ethz.ch/education-students/lectures/ros.html) which can provide great help in learning and applying ROS for begeinners  

The project is one of the solutions of the excercises from  the ETH courses. It aims to enable the SMB robot to perform target tracking, motion control, and emergency stop based on sensor information (such as odometry, laser scanning, IMU, etc.). Additionally, it focuses on mastering the use of fundamental ROS tools such as Gazebo, Rviz, Rqt, and bag files  

The development of this project was greatly inspired and assisted by [NelsenEW/eth-zurich-solution](https://github.com/NelsenEW/eth-zurich-solution.git) which has more details and guidance for the ETH excercises

## How to initiate?
`git clone` this repo to the workspace directory  

`cd ~/path/to/Simulation-for-Super-Mega-Bot/src`, initiate the **workspace** by `catkin_init_workspace`

`cd ..` to  `/Simulation-for-Super-Mega-Bot`, compile the workspace by `catkin build`(recommended)

Set workspace environment by `source devel/setup.bash`, or revise `.bashrc` file with adding `source ~/path/to/Simulation-for-Super-Mega-Bot/devel/setup.bash` directly  

If above steps all successed, now can try to run the simulation

## How to run the Simulation?
### Regular
Run the simulation through `roslaunch smb_highlevel_controller smb_highlevel_controller.launch start_robot:="true"`. In this way, the SMB robot will operate normally and head towards the target, but eventually, it will collide with obstacles and will not stop 

![image](https://github.com/Ye-Dehuo/Simulation-for-Super-Mega-Bot/blob/main/img/Gazebo%20display.png) 

### Emergency stop
In oder to realize the emergency stop function, can run the simulation through one of the following commands  

`roslaunch smb_highlevel_controller smb_highlevel_controller.launch start_robot:=true auto_emergency_stop:=true prior_collision:=true`  

`roslaunch smb_highlevel_controller smb_highlevel_controller.launch start_robot:=true auto_emergency_stop:=true post_collision:=true`  

The fomer will stop the SMB robot when the robot is close enough to the obstacle(**prior_collision stop**), while the latter will stop the robot after crashing with the obstacle(**post_collision stop**)
 ## How to set Rviz?
Visualize the 3D point cloud and the laser scan by choosing topics /rslidar_scan and /scan <br><br>
Visualize SMB robot by adding Robot Model plugin <br><br>
Visualize coordinate frame by adding TF display plugin <br><br>
Visualize markers near the pillar by adding Marker plugin <br>
## How to play bag?
The bag file `smb_navigation.bag` which is located in `/src/smb_highlevel_controller/bag` contains sensor measurements from wheel odometry, inertial measurement unit (IMU) and laser scanner <br>
  
You can **play the bag** by roslaunching `ekf_localization.launch`. When playing the bag, you can hit space to toggle paused<br>
  
![image](https://github.com/Ye-Dehuo/ETH_SMB/blob/main/img/recorded%20path%20plot.png) 
<p align="center"> path plot of the bag </p>  

![image](https://github.com/Ye-Dehuo/ETH_SMB/blob/main/img/rviz%20display%20of%20the%20%20bag.png)
<p align="center"> Rviz display of the bag </p>
