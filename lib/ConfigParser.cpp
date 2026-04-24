#include "ConfigParser.hpp"

void ConfigParser::trim(std::string& str) {
    // Remove leading spaces
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    // Remove trailing spaces
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

ConfigParser::ConfigParser(const unsigned& cam_mov_speed, const unsigned& cam_rot_speed, 
                           const unsigned& cam_fov_deg, const unsigned& cam_ray_amt, 
                           const unsigned& cam_radius_px, const unsigned& max_fps){
    
    parameters = std::vector<unsigned>({cam_mov_speed, cam_rot_speed, cam_fov_deg, cam_ray_amt, cam_radius_px, max_fps});
}

void ConfigParser::startUpValues(unsigned& cam_mov_speed, unsigned& cam_rot_speed, 
                        unsigned& cam_fov_deg, unsigned& cam_ray_amt, 
                        unsigned& cam_radius_px, unsigned& max_fps){

    cam_mov_speed = parameters[ConfigParser::CAM_MOV_SPEED];
    cam_rot_speed = parameters[ConfigParser::CAM_ROT_SPEED];
    cam_fov_deg = parameters[ConfigParser::CAM_FOV_DEG];
    cam_ray_amt = parameters[ConfigParser::CAM_RAY_AMT];
    cam_radius_px = parameters[ConfigParser::CAM_RADIUS_PX];
    max_fps = parameters[ConfigParser::MAX_FPS];
}

bool ConfigParser::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config file: " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        trim(line);

        // Skip empty lines and comments (lines starting with #)
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Find the position of the '=' delimiter
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            // Extract and trim the key and value
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            
            trim(key);
            trim(value);

            if (key == "camera_mov_speed"){
                parameters[ConfigParser::CAM_MOV_SPEED] = static_cast<unsigned>(std::stoul(value.c_str()));
            }else if (key == "camera_rotation_speed"){
                parameters[ConfigParser::CAM_ROT_SPEED] = static_cast<unsigned>(std::stoul(value.c_str()));
            }else if (key == "camera_fov_deg"){
                parameters[ConfigParser::CAM_FOV_DEG] = static_cast<unsigned>(std::stoul(value.c_str()));
            }else if (key == "camera_ray_amt"){
                parameters[ConfigParser::CAM_RAY_AMT] = static_cast<unsigned>(std::stoul(value.c_str()));
            }else if (key == "camera_radius"){
                parameters[ConfigParser::CAM_RADIUS_PX] = static_cast<unsigned>(std::stoul(value.c_str()));
            }else if (key == "max_fps"){
                parameters[ConfigParser::MAX_FPS] = static_cast<unsigned>(std::stoul(value.c_str()));
            }
            // Ignores any unknown configuration options
        }
    }
    return true;
}