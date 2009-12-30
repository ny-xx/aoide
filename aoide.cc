/*
  	Filename: .cc
	Author: Kreil Dominik	
	Description: 

	Aoide is a simple musicplayer using the file structure
*/

#include <gtkmm/main.h>

#include "main_window.hh"

int main(int argc, char **argv)
{  
   
   Gtk::Main m(&argc, &argv);

   main_window main_window;
   Gtk::Main::run(main_window);
   return 0;
}
