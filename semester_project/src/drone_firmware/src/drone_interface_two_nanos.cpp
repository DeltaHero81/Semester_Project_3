#include "drone_firmware/drone_interface_two_nanos.hpp"
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <pluginlib/class_list_macros.hpp>


namespace drone_firmware_two_nanos
{
DroneInterface::DroneInterface()
{
}


DroneInterface::~DroneInterface()
{
  //Close the serial monitor in case it is still connected
  if (arduino1_.IsOpen())
  {
    try
    {
      arduino1_.Close();
    }
    catch (...)
    {
      RCLCPP_FATAL_STREAM(rclcpp::get_logger("DroneInterface"),
                          "Something went wrong while closing connection with port " << port1_);
    }
  }
  if (arduino2_.IsOpen())
  {
    try
    {
      arduino2_.Close();
    }
    catch (...)
    {
      RCLCPP_FATAL_STREAM(rclcpp::get_logger("DroneInterface"),
                          "Something went wrong while closing connection with port " << port2_);
    }
  }
}

//Define the on_init function in the DroneInterface class
CallbackReturn DroneInterface::on_init(const hardware_interface::HardwareInfo &hardware_info)
{
  //First execute the on init function
  CallbackReturn result = hardware_interface::SystemInterface::on_init(hardware_info);
  //Check if it was successful
  if (result != CallbackReturn::SUCCESS)
  {
    //Abort the execution
    return result;
  }

  //Try to figure out which port the arduino is connected to
  try
  {
    port1_ = info_.hardware_parameters.at("port1");
  }
  //If it was unable to connect, write an error message
  catch (const std::out_of_range &e)
  {
    RCLCPP_FATAL(rclcpp::get_logger("DroneInterface"), "No Serial Port provided! Aborting");
    return CallbackReturn::FAILURE;
  }
  try
  {
    port2_ = info_.hardware_parameters.at("port2");
  }
  //If it was unable to connect, write an error message
  catch (const std::out_of_range &e)
  {
    RCLCPP_FATAL(rclcpp::get_logger("DroneInterface"), "No Serial Port provided! Aborting");
    return CallbackReturn::FAILURE;
  }

  //Reserve space for the variables
  velocity_commands_.reserve(info_.joints.size());
  position_states_.reserve(info_.joints.size());
  velocity_states_.reserve(info_.joints.size());
  last_run_ = rclcpp::Clock().now();

  return CallbackReturn::SUCCESS;
}

//Define function, creates vectors of type StateInterface
std::vector<hardware_interface::StateInterface> DroneInterface::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;

  // Provides position and velocity Interaface for state communication
  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    //New element added to state_interface
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_states_[i]));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &velocity_states_[i]));
  }
  //Return the vector
  return state_interfaces;
}

//Define function, commincates the Interface to be used, so movement commands can be sent
std::vector<hardware_interface::CommandInterface> DroneInterface::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;

  // Provide only a velocity Interafce
  for (size_t i = 0; i < info_.joints.size(); i++)
  {
    //Create new element and fill the element with the velocity commands
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &velocity_commands_[i]));
  }

  return command_interfaces;
}


CallbackReturn DroneInterface::on_activate(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(rclcpp::get_logger("DroneInterface"), "Starting robot hardware ...");

  // Reset commands and states
  velocity_commands_ = { 0.0, 0.0 };
  position_states_ = { 0.0, 0.0 };
  velocity_states_ = { 0.0, 0.0 };

  //Try to open the port and connect to the arduino
  try
  {
    arduino1_.Open(port1_);
    arduino1_.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
  }
  //Catch and write enny errors
  catch (...)
  {
    RCLCPP_FATAL_STREAM(rclcpp::get_logger("DroneInterface"),
                        "Something went wrong while interacting with port " << port1_);
    return CallbackReturn::FAILURE;
  }
  try
  {
    arduino2_.Open(port2_);
    arduino2_.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
  }
  //Catch and write enny errors
  catch (...)
  {
    RCLCPP_FATAL_STREAM(rclcpp::get_logger("DroneInterface"),
                        "Something went wrong while interacting with port " << port2_);
    return CallbackReturn::FAILURE;
  }

  RCLCPP_INFO(rclcpp::get_logger("DroneInterface"),
              "Hardware started, ready to take commands");
  return CallbackReturn::SUCCESS;
}


CallbackReturn DroneInterface::on_deactivate(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(rclcpp::get_logger("DroneInterface"), "Stopping robot hardware ...");

  //If the communication is open then close it
  if (arduino1_.IsOpen())
  {
    try
    {
      arduino1_.Close();
    }
    catch (...)
    {
      RCLCPP_FATAL_STREAM(rclcpp::get_logger("DroneInterface"),
                          "Something went wrong while closing connection with port " << port1_);
      return CallbackReturn::FAILURE;
    }
  }
  if (arduino2_.IsOpen())
  {
    try
    {
      arduino2_.Close();
    }
    catch (...)
    {
      RCLCPP_FATAL_STREAM(rclcpp::get_logger("DroneInterface"),
                          "Something went wrong while closing connection with port " << port2_);
      return CallbackReturn::FAILURE;
    }
  }

  RCLCPP_INFO(rclcpp::get_logger("DroneInterface"), "Hardware stopped");
  return CallbackReturn::SUCCESS;
}

//Read from the arduino
hardware_interface::return_type DroneInterface::read(const rclcpp::Time &,
                                                          const rclcpp::Duration &)
{
  // Interpret the string
  if(arduino1_.IsDataAvailable())
  {
    auto dt = (rclcpp::Clock().now() - last_run_).seconds();
    std::string message;
    arduino1_.ReadLine(message);
    std::stringstream ss(message);
    std::string res;
    int multiplier = 1;
    while(std::getline(ss, res, ','))
    {
      //If the response it p (positive) return 1 otherwise return -1
      multiplier = res.at(1) == 'p' ? 1 : -1;

      if(res.at(0) == 'r')
      {
        velocity_states_.at(0) = multiplier * std::stod(res.substr(2, res.size()));
        position_states_.at(0) += velocity_states_.at(0) * dt;
      }
      else if(res.at(0) == 'l')
      {
        velocity_states_.at(1) = multiplier * std::stod(res.substr(2, res.size()));
        position_states_.at(1) += velocity_states_.at(1) * dt;
      }
    }
    last_run_ = rclcpp::Clock().now();
  }
  // Interpret the string
  if(arduino2_.IsDataAvailable())
  {
    auto dt = (rclcpp::Clock().now() - last_run_).seconds();
    std::string message;
    arduino2_.ReadLine(message);
    std::stringstream ss(message);
    std::string res;
    int multiplier = 1;
    while(std::getline(ss, res, ','))
    {
      //If the response it p (positive) return 1 otherwise return -1
      multiplier = res.at(1) == 'p' ? 1 : -1;

      if(res.at(0) == 'r')
      {
        velocity_states_.at(0) = multiplier * std::stod(res.substr(2, res.size()));
        position_states_.at(0) += velocity_states_.at(0) * dt;
      }
      else if(res.at(0) == 'l')
      {
        velocity_states_.at(1) = multiplier * std::stod(res.substr(2, res.size()));
        position_states_.at(1) += velocity_states_.at(1) * dt;
      }
    }
    last_run_ = rclcpp::Clock().now();
  }
  return hardware_interface::return_type::OK;
}

//Write to the arduino
hardware_interface::return_type DroneInterface::write(const rclcpp::Time &,
                                                          const rclcpp::Duration &)
{
  // Implement communication protocol with the Arduino
  std::stringstream message_stream;
  char right_wheel_sign = velocity_commands_.at(0) >= 0 ? 'p' : 'n';
  char left_wheel_sign = velocity_commands_.at(1) >= 0 ? 'p' : 'n';
  std::string compensate_zeros_right = "";
  std::string compensate_zeros_left = "";
  //Make the writing strings the right size for the arduino
  if(std::abs(velocity_commands_.at(0)) < 10.0)
  {
    compensate_zeros_right = "0";
  }
  else
  {
    compensate_zeros_right = "";
  }
  if(std::abs(velocity_commands_.at(1)) < 10.0)
  {
    compensate_zeros_left = "0";
  }
  else
  {
    compensate_zeros_left = "";
  }
  //Fix the precision to 2 decimal numbers and load the needed information
  message_stream << std::fixed << std::setprecision(2) << 
    "r" << right_wheel_sign << compensate_zeros_right << std::abs(velocity_commands_.at(0)) << 
    ",l" << left_wheel_sign << compensate_zeros_left << std::abs(velocity_commands_.at(1)) << ",";

  //Try to send the message to the arduino
  try
  {
    arduino1_.Write(message_stream.str());
  }
  catch (...)
  {
    RCLCPP_ERROR_STREAM(rclcpp::get_logger("DroneInterface"),
                        "Something went wrong while sending the message "
                            << message_stream.str() << " to the port " << port1_);
    return hardware_interface::return_type::ERROR;
  }
  try
  {
    arduino2_.Write(message_stream.str());
  }
  catch (...)
  {
    RCLCPP_ERROR_STREAM(rclcpp::get_logger("DroneInterface"),
                        "Something went wrong while sending the message "
                            << message_stream.str() << " to the port " << port2_);
    return hardware_interface::return_type::ERROR;
  }

  return hardware_interface::return_type::OK;
}
}  // namespace drone_firmware

//PLUGINLIB_EXPORT_CLASS(drone_firmware::DroneInterface, hardware_interface::SystemInterface)