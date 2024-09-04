# ETH_SMB
## Rviz
visualize the 3D point cloud and the laser scan by choosing topics /rslidar_scan and /scan <br><br>
visualize SMB robot by adding Robot Model plugin <br><br>
visualize coordinate frame by adding TF display plugin <br><br>
visualize markers near the pillar by adding Marker plugin <br>
## Play bag
The bag file `smb_navigation.bag` which is located in `/src/smb_highlevel_controller/bag` contains sensor measurements from wheel odometry, inertial measurement unit (IMU) and laser scanner <br>
  
You can **play the bag** by roslaunching `ekf_localization.launch`. When playing the bag, you can hit space to toggle paused<br>
<br>
![image](https://github.com/Ye-Dehuo/ETH_SMB/blob/main/img/recorded%20path%20plot.png) <br>
<p align="center"> path plot of the bag </p> <br>
![image](https://github.com/Ye-Dehuo/ETH_SMB/blob/main/img/rviz%20display%20of%20the%20%20bag.png) <br>
<p align="center"> rviz display of the bag </p> <br>
