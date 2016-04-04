#ifndef CONSOLE_COLORS_H
#define CONSOLE_COLORS_H

#include <ostream>

namespace Console_color 
{
	enum Code
	{
		FG_DGRAY   = 30,
		FG_RED     = 31,
		FG_GREEN   = 32,
		FG_BLUE    = 34,
		FG_LGRAY   = 37,
		FG_DEFAULT = 39,
		BG_RED     = 41,
		BG_GREEN   = 42,
		BG_BLUE    = 44,
		BG_DEFAULT = 49
	};
	class Modifier 
	{
		private:
			Code code;
		public:
			Modifier(Code code_p);
			~Modifier();
			friend std::ostream& operator <<(std::ostream& os_p, const Modifier& mod_p);
			
	};

	Modifier::Modifier(Code code_p) : code(code_p) 
	{

	}
	Modifier::~Modifier()
	{

	}
	std::ostream& operator <<(std::ostream& os_p, const Modifier& mod_p) 
	{
		return os_p << "\033[" << mod_p.code << "m";
	};
}

Console_color::Modifier c_red(  Console_color::FG_RED    );
Console_color::Modifier c_blue( Console_color::FG_BLUE   );
Console_color::Modifier c_green(Console_color::FG_GREEN  );
Console_color::Modifier c_lgray(Console_color::FG_LGRAY  );
Console_color::Modifier c_dgray(Console_color::FG_DGRAY  );
Console_color::Modifier c_def(  Console_color::FG_DEFAULT);

#endif