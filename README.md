# ETH_SMB
## Run
This repository is based on the ETH lectures [Programming for Robotics - ROS](https://rsl.ethz.ch/education-students/lectures/ros.html)  

After `catkin build` the workspace `/smb_ws`, you can run the simulation through `roslaunch smb_highlevel_controller smb_highlevel_controller.launch start_robot:="true"`. Also, in oder to realize the emergency stop function, you can run the simulation through `roslaunch smb_highlevel_controller smb_highlevel_controller.launch start_robot:=true auto_emergenc_stop:=true prior_collision:=true`or `roslaunch smb_highlevel_controller smb_highlevel_controller.launch start_robot:=true auto_emergenc_stop:=true post_collision:=true`. The fomer will stop the SMB robot when the robot is close enough to the obstacle, while the latter will stop the robot after crashing with the obstacle 
 ## Rviz
visualize the 3D point cloud and the laser scan by choosing topics /rslidar_scan and /scan <br><br>
visualize SMB robot by adding Robot Model plugin <br><br>
visualize coordinate frame by adding TF display plugin <br><br>
visualize markers near the pillar by adding Marker plugin <br>
## Play bag
The bag file `smb_navigation.bag` which is located in `/src/smb_highlevel_controller/bag` contains sensor measurements from wheel odometry, inertial measurement unit (IMU) and laser scanner <br>
  
You can **play the bag** by roslaunching `ekf_localization.launch`. When playing the bag, you can hit space to toggle paused<br>
  
![image](https://github.com/Ye-Dehuo/ETH_SMB/blob/main/img/recorded%20path%20plot.png) 
<p align="center"> path plot of the bag </p>  

![image](https://github.com/Ye-Dehuo/ETH_SMB/blob/main/img/rviz%20display%20of%20the%20%20bag.png)
<p align="center"> rviz display of the bag </p>  

