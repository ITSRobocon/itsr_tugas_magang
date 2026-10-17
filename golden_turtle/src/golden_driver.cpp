#include <cmath>
#include <chrono>
#include <functional>
#include <memory>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "std_srvs/srv/empty.hpp"

using namespace std::chrono_literals;

class GoldenDriver : public rclcpp::Node
{
public:
  GoldenDriver()
  : Node("golden_driver"),
  omega_(1.5),
  v_max_(8.0),
  golden_ratio_(1.618),
  pi_(3.14159),
  setup_initiated_(false),
  finished_(false)
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    teleport_client_ = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");
    clear_client_ = this->create_client<std_srvs::srv::Empty>("/clear");
    set_pen_client_ = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");

    b_ = std::log(golden_ratio_) / (pi_ / 2);

    timer_ = this->create_wall_timer(
      100ms, [this]() {
        if (!setup_initiated_) {
          if(teleport_client_->service_is_ready() &&
          clear_client_->service_is_ready() &&
          set_pen_client_->service_is_ready())
          {
            setup_initiated_ = true;
            teleport_turtle();
          } else {
            RCLCPP_INFO(this->get_logger(), "Waiting turtlesim to come online...");
          }
        }
      });
  }
private:
  void teleport_turtle()
  {
    RCLCPP_INFO(this->get_logger(), "Placing Turtle at coordinates (0,0)...");
    auto req = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
    req->x = 4.0;
    req->y = 2.0;
    req->theta = 0.0;

    teleport_client_->async_send_request(req, [this](rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedFuture future) {
      (void)future;
      clear_background();
    });
  }

  void clear_background()
  {
    auto req = std::make_shared<std_srvs::srv::Empty::Request>();
    clear_client_->async_send_request(req, [this](rclcpp::Client<std_srvs::srv::Empty>::SharedFuture future) {
      (void)future;
      set_pen_color();
    });
  }

  void set_pen_color()
  {
    auto req = std::make_shared<turtlesim::srv::SetPen::Request>();
    req->r = 255;
    req->g = 215;
    req->b = 0;
    req->width = 3;
    req->off = false;

    set_pen_client_->async_send_request(req, [this](rclcpp::Client<turtlesim::srv::SetPen>::SharedFuture future) {
      (void)future;
      start_spiral();
    });
  }

  void start_spiral()
  {
    RCLCPP_INFO(this->get_logger(), "Setup complete. Starting the Golden Spiral...");
    start_time_ = this->now();
    timer_ = this->create_wall_timer(
      20ms, [this]() { timer_callback(); });
  }

  void timer_callback()
  {
    if (finished_) {
      return;
    }

    auto current_time = this->now();
    double t = (current_time - start_time_).seconds();

    auto msg = geometry_msgs::msg::Twist();

    if (t > 15.0) {
      msg.linear.x = 0.0;
      msg.angular.z = 0.0;
      publisher_->publish(msg);
      RCLCPP_INFO(this->get_logger(), "Golden Spiral finished");
      finished_ = true;
      return;
    }

    double linear_velocity = v_max_ * std::exp(-b_ * t);
    double angular_velocity = omega_;
    msg.linear.x = linear_velocity;
    msg.angular.z = angular_velocity;
    publisher_->publish(msg);
  }

  rclcpp::TimerBase::SharedPtr init_timer_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client_;
  rclcpp::Client<std_srvs::srv::Empty>::SharedPtr clear_client_;
  rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr set_pen_client_;

  rclcpp::Time start_time_;

  double omega_;
  double v_max_;
  double golden_ratio_;
  double pi_;
  double b_;
  bool setup_initiated_;
  bool finished_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GoldenDriver>());
  rclcpp::shutdown();
  return 0;
}