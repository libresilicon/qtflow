#include "vcd_data.hpp"
#include <iostream>

namespace vcd {

    std::ostream& operator<<(std::ostream& os, LogicValue val) {
        if(val == LogicValue::ONE) os << "1";
        else if(val == LogicValue::ZERO) os << "0";
        else if(val == LogicValue::UNKOWN) os << "x";
        else if(val == LogicValue::HIGHZ) os << "z";
        else assert(false);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, Var::Type type) {
        if(type == Var::Type::WIRE) os << "wire";
        else if(type == Var::Type::REG) os << "reg";
        else if(type == Var::Type::PARAMETER) os << "parameter";
        else assert(false);
        return os;
    }

	Var::Var(Type new_type, size_t new_width, Id new_id, std::string new_str_id, std::vector<std::string> new_hierarchical_name)
		: type_(new_type)
		, width_(new_width)
		, id_(new_id)
		, str_id_(new_str_id)
		, hierarchical_name_(new_hierarchical_name)
	{
	}

	Var::Type Var::type() const
	{
		return type_;
	}

	size_t Var::width() const
	{
		return width_;
	}

	std::string Var::str_id() const
	{
		return str_id_;
	}

	Var::Id Var::id() const
	{
		return id_;
	}

	std::vector<std::string> Var::hierarchical_name() const
	{
		return hierarchical_name_;
	}

	std::string Var::name() const
	{
		return *(--hierarchical_name_.end());
	}

	std::string Var::str_type()
	{
		std::string ret;
		if(type_==Type::WIRE)
			ret = "WIRE";
		if(type_==Type::REG)
			ret = "REG";
		if(type_==Type::PARAMETER)
			ret = "PARAMETER";
		return ret;
	}

	TimeValue::TimeValue(size_t new_time, Var::Id new_var_id, LogicValue new_value) :
		time_(new_time),
		var_id_(new_var_id),
		value_(new_value)
	{}

	size_t TimeValue::time() const
	{
		return time_;
	}

	Var::Id TimeValue::var_id() const
	{
		return var_id_;
	}

	LogicValue TimeValue::value() const
	{
		return value_;
	}

	TimeBusValue::TimeBusValue(size_t new_time, Var::Id new_var_id, std::vector<vcd::LogicValue> new_values) :
		time_(new_time),
		var_id_(new_var_id),
		values_(new_values)
	{
	}

	size_t TimeBusValue::time() const
	{
		return time_;
	}

	Var::Id TimeBusValue::var_id() const
	{
		return var_id_;
	}

	std::vector<vcd::LogicValue> TimeBusValue::values()
	{
		return values_;
	}

	const std::string& Header::date() const
	{
		return date_;
	}

	const std::string& Header::version() const
	{
		return version_;
	}

	const std::string& Header::timescale() const
	{
		return timescale_;
	}

	void Header::set_date(const std::string& new_date)
	{
		date_ = new_date;
	}

	void Header::set_version(const std::string& new_version)
	{
		version_ = new_version;
	}

	void Header::set_timescale(const std::string& new_timescale)
	{
		timescale_ = new_timescale;
	}

	VcdData::VcdData(const Header new_header, std::vector<Var> new_vars, TimeValues new_time_values, TimeBusValues new_time_bus_values) :
		header_(new_header),
		vars_(new_vars),
		time_values_(new_time_values),
		time_bus_values_(new_time_bus_values)
	{}

	const Header &VcdData::header() const
	{
		return header_;
	}

	const std::vector<Var> &VcdData::vars() const
	{
		return vars_;
	}

	const TimeValues &VcdData::time_values() const
	{
		return time_values_;
	}

	const TimeBusValues &VcdData::time_bus_values() const
	{
		return time_bus_values_;
	}

} //vcd
