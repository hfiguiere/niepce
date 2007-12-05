

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "geometry.h"

namespace utils {

	Rect::Rect()
	{
	}

	Rect::Rect(int x, int y, int w, int h)
	{
		_r[X] = x;
		_r[Y] = y;
		_r[W] = w;
		_r[H] = h;
	}


	Rect::Rect(const std::string & s)
		throw(std::bad_cast)
	{
		std::vector< std::string > v;
		v.reserve(4);
		boost::split(v, s, boost::is_any_of(" "));
		if(v.size() < 4) {
			throw(std::bad_cast());
		}
		int i = 0;
		std::vector< std::string >::iterator iter;
		for(iter = v.begin(); iter != v.end(); ++iter) {
			_r[i] = boost::lexical_cast<int>(*iter);
			i++;
		}
	}

	std::string Rect::to_string() const
	{
		return str(boost::format("%1% %2% %3% %4%")
					  % _r[X] % _r[Y] % _r[W] % _r[H]); 
	}
}
