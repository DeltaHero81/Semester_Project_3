#ifndef DRONE_INTERFACE_TWO_NANOS_HPP
#define DRONE_INTERFACE_TWO_NANOS_HPP

#include <rclcpp/rclcpp.hpp>
#include <hardware_interface/system_interface.hpp>
#include <libserial/SerialPort.h>
#include <rclcpp_lifecycle/state.hpp>
#include <rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp>

#include <vector>
#include <string>

namespace drone_firmware_two_nanos
{

//using a shorter name
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class DroneInterface : public hardware_interface::SystemInterface
{
public:
  //Constructor (same name as the class)
  DroneInterface();
  virtual ~DroneInterface();

  // Implementing rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface
  CallbackReturn on_activate(const rclcpp_lifecycle::State &previous_state) override;
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previous_state) override;

  // Implementing hardware_interface::SystemInterface
  CallbackReturn on_init(const hardware_interface::HardwareInfo &hardware_info) override;
  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
  hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration &period) override;
  hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration &period) override;

private:
  LibSerial::SerialPort arduino1_;
  LibSerial::SerialPort arduino2_;
  std::string port1_;
  std::string port2_;
  std::vector<double> velocity_commands_;
  std::vector<double> position_states_;
  std::vector<double> velocity_states_;
  rclcpp::Time last_run_;
};
}  // namespace drone_firmware


#endif  // DRONE_INTERFACE_TWO_NANOS_HPP