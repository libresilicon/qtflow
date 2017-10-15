#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cassert>
#include <limits>
#include <QDebug>

#include "vcd_data_fwd.hpp"

//The classes defined in this file correspond to the data loaded from the VCD file

namespace vcd {

	enum class LogicValue {
		ONE,
		ZERO,
		UNKOWN,
		HIGHZ
	};
	std::ostream& operator<<(std::ostream& os, LogicValue val);

	class Var {
		public:
			enum class Type {
				WIRE,
				REG,
				PARAMETER,
				VARINTEGER
			};

			typedef int Id;

			Var() = default;
			Var(Type new_type, size_t new_width, Id new_id, std::string new_str_id, std::vector<std::string> new_hierarchical_name);
			Type type() const;
			size_t width() const;
			std::string str_id() const;
			std::string str_type();
			Id id() const;
			std::vector<std::string> hierarchical_name() const;
			std::string name() const;

		private:
			Type type_;
			size_t width_;
			Id id_;
			std::string str_id_;
			std::vector<std::string> hierarchical_name_;
	};
	std::ostream& operator<<(std::ostream& os, Var::Type type);

	class TimeValue {
		public:
			TimeValue(size_t new_time=0, Var::Id new_var_id=-1, LogicValue new_value=LogicValue::UNKOWN);
			size_t time() const;
			Var::Id var_id() const;
			LogicValue value() const;

		private:
			size_t time_;
			Var::Id var_id_;
			LogicValue value_;
	};

	class TimeBusValue {
		public:
			TimeBusValue(size_t new_time, Var::Id new_var_id, std::vector<LogicValue> new_values);
			size_t time() const;
			Var::Id var_id() const;
			std::vector<vcd::LogicValue> values();

		private:
			size_t time_;
			Var::Id var_id_;
			std::vector<vcd::LogicValue> values_;
	};

	class Header {
		public:
			Header() = default;
			const std::string& date() const;
			const std::string& version() const;
			const std::string& timescale() const;

			void set_date(const std::string& new_date);
			void set_version(const std::string& new_version);
			void set_timescale(const std::string& new_timescale);

		private:
			std::string date_;
			std::string version_;
			std::string timescale_;
	};

	typedef std::vector<TimeValue> TimeValues;
	typedef std::vector<TimeBusValue> TimeBusValues;

	class VcdData {
		public:
			VcdData() = default;
			VcdData(const Header new_header, std::vector<Var> new_vars, TimeValues new_time_values, TimeBusValues new_time_bus_values);

			const Header &header() const;
			const std::vector<Var> &vars() const;
			const TimeValues &time_values() const;
			const TimeBusValues &time_bus_values() const;

		private:
			Header header_;
			std::vector<Var> vars_;
			TimeValues time_values_;
			TimeBusValues time_bus_values_;

	};

} //namespace vcd
