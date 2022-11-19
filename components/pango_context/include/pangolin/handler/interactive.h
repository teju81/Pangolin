#pragma once

#include <Eigen/Core>
#include <pangolin/utils/flag_set.h>
#include <pangolin/maths/conventions.h>
#include <pangolin/maths/min_max.h>

#include <optional>
#include <variant>


namespace pangolin
{

struct Context;

struct WindowPosition
{
  const MinMax<Eigen::Array2i>& region() const
  {
    return region_;
  }

  Eigen::Array2d posInWindow() const
  {
    return pos_window_;
  }

  Eigen::Array2d posInRegion() const
  {
    return pos_window_ - region_.min().cast<double>();
  }

  Eigen::Array2d posInRegionNorm() const
  {
    return (pos_window_ - region_.min().cast<double>()) / region_.range().cast<double>();
  }

  MinMax<Eigen::Array2i> region_;
  Eigen::Array2d pos_window_;
};

enum class PointerAction {
    hover, down, drag, drag_up, click_up, double_click_up
};

enum class PointerButton {
    primary, secondary, tertiary,
    back, forward, _
};

enum class ModifierKey {
    win_cmd_meta, shift, ctrl, fn, alt_option, _
};

struct Interactive {
    using PointerButtonStatus = flag_set<PointerButton>;
    using ModifierKeyStatus = flag_set<ModifierKey>;

    struct PointerEvent {
        // Event and if applicable button that triggered event
        PointerAction action;
        std::optional<PointerButton> button;

        // Current state of input devices
        PointerButtonStatus button_active;
    };

    struct ScrollEvent {
        Eigen::Vector2d pan = {0.0,0.0};
        Eigen::Vector2d tilt = {0.0,0.0};
        double zoom = 0;
    };

    struct KeyboardEvent {
        unsigned char key;
        bool pressed;
    };

    struct Event {
      WindowPosition pointer_pos;
      ModifierKeyStatus modifier_active;
      std::variant<PointerEvent,ScrollEvent,KeyboardEvent> detail;
    };

    virtual bool handleEvent(const Context&, const Event&) = 0;
};

}
