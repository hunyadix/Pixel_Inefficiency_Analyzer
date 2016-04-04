#ifndef SMARTHISTO_CUT_H
#define SMARTHISTO_CUT_H

#include <string>
#include <functional>
#include "../Root_color_sets/root_color_sets.hh"

extern bool debug;

namespace Histogram_generation
{

	class Cut
	{
		protected:
			std::function<bool()> cut_function = [] {std::cerr << error_prompt << "Uninitialized cut function!" << std::endl; exit(-1); return true;};
			const char* name;
		public:
			bool operator()()
			{
				return this -> cut_function();
			};
			virtual ~Cut()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut()" << std::endl; 
				}
			}
			const std::function<bool()>& get_cut_function() const {return this -> cut_function;};
			void                         set_cut_function(const std::function<bool()> function_p) {this -> cut_function = function_p;};
			void                         set_name(const char* name_p) {this -> name = name_p;};
			const char* const            get_name() {return this -> name;};
			void                         chain(const std::function<bool()>& condition_p) // Should be avoided
			{
				std::function<bool()> original;
				original = this -> cut_function;
				this -> cut_function = [condition_p, original]()
				{
					bool original_results  = original();
					bool condition_results = condition_p();
					if(original_results && condition_results)
					{
						return true;
					}
					return false;
				};
			}
			void chain(const Cut& other_p)
			{
				this -> chain(other_p.get_cut_function());
			}
			Cut& operator=(const Cut& right_side_p)
			{
				this -> cut_function = right_side_p.cut_function;
				this -> name         = right_side_p.name;
				return *this;
			}
			Cut& operator+=(const Cut& right_side_p)
			{
				this -> chain(right_side_p.cut_function);
				return *this;
			}
			Cut& operator+=(const std::function<bool()>& right_side_p)
			{
				this -> chain(right_side_p);
				return *this;
			}
	};

} // Histogram_generation

#endif