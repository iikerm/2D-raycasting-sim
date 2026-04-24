#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class ConfigParser {
private:
    std::vector<unsigned> parameters;   // Contains the parameters in the order defined below

    // Helper function to remove leading and trailing whitespace
    void trim(std::string& str);

public:
    // Constants that represent the index of each configuration parameter in the vector
    const static unsigned short CAM_MOV_SPEED=0,
                                CAM_ROT_SPEED=1,
                                CAM_FOV_DEG=2,
                                CAM_RAY_AMT=3,
                                CAM_RADIUS_PX=4,
                                MAX_FPS=5;

    // Constructor that sets the default parameters for each configuration option
    ConfigParser(const unsigned& cam_mov_speed=0U, const unsigned& cam_rot_speed=0U, 
                 const unsigned& cam_fov_deg=0U, const unsigned& cam_ray_amt=0U, 
                 const unsigned& cam_radius_px=0U, const unsigned& max_fps=0U); 
    
    // Initializes the variables it receives with the values loaded. Should be used after load()
    void startUpValues(unsigned& cam_mov_speed, unsigned& cam_rot_speed, 
                       unsigned& cam_fov_deg, unsigned& cam_ray_amt, 
                       unsigned& cam_radius_px, unsigned& max_fps);

    // Loads and parses the configuration file
    bool load(const std::string& filename);

    // Returns the specified config parameter (0-5), no check is made to verify if the number is out of range
    unsigned get(const unsigned short& what){
        return parameters[what];
    }
};