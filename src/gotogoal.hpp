// inclusao dos cabeçalhos necessarios. ROS, geometry_msgs, turtlesim, etc*
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseStamped.h>
#include <turtlesim/Pose.h>

// classe geral para utilização pelo script
class Go {
    // variaveis de uso interno do header
    private:
        // node handler para o ROS
        ros::NodeHandle& _Node; 
        // publishers e subscribers para os dados do ROS 
        ros::Subscriber _subPosition;
        ros::Publisher _pubvel;
        // variaveis internas para o script 
        double _posegoal[2];
        double _pose[3]; // posicao em x, y e yheta
        double _accep;
        const float pi = 3.141592;
        int _x;
        int _y;
        // declaraçao do metodo de callbacks
        void subPoseCallback(const turtlesim::Pose::ConstPtr&);
        
    // declaracao do metodo publico para uso no script
    public:
        Go(ros::NodeHandle&, double p_tolerance);
        void gotogoal();
        void getcoords();

};