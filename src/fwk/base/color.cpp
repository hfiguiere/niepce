

#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "fwk/base/color.hpp"

namespace fwk {


  RgbColor::RgbColor(value_type r, value_type g, value_type b)
  {
    at(0) = r;
    at(1) = g;
    at(2) = b;
  }


  RgbColor::RgbColor(const std::string & s)
  {
    std::vector<std::string> components;
    boost::split(components, s, boost::is_any_of(" "));
    if(components.size() >= 3) {
      try {
        for(int i = 0; i < 3; ++i) {
          at(i) = boost::lexical_cast<value_type>(components[i]);
        }
        return;
      }
      catch(...) {

      }
    }
    // fallback in case of failure
    at(0) = 0;
    at(1) = 0;
    at(2) = 0;      
  }


  std::string RgbColor::to_string() const
  {
    return str(boost::format("%1% %2% %3%") % at(0) % at(1) % at(2));
  }


}
