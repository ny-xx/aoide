//
// C++ Interface: playlist 
//
// Description: 
//
// 
// Author: ny,,, <ny.softwared@gmx.at>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLAYLIST_H
#define PLAYLIST_H

//#include <gtk/gtk.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace Gtk;
using namespace std;

class Playlist : public TreeView
{
public:
    Playlist();

    ~Playlist();

	class Columns : public Gtk::TreeModel::ColumnRecord {
    public:
        Columns() {
			add(col_img);
			add(col_title);
            add(col_path);
        }
   
        ~Columns() {}
   		Gtk::TreeModelColumn<Glib::ustring> col_title;
        Gtk::TreeModelColumn<Glib::ustring> col_path;
		Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_img;
    }; 

	void save(string path);
	void load(string path);
	void appendTitle(string title, string path);
	int getRowNumber(TreeModel::Row &row);
	int getNumberOfTracks(){return numberOfTracks;};
	void deleteTrack(const ListStore::iterator &iter){if(numberOfTracks){listStore->erase(iter);numberOfTracks--;}};
	void deleteAll(){if(numberOfTracks){listStore->clear(); numberOfTracks=0;}};
	string getRowPath(int n);
	
	Columns columns;
private:
	Glib::RefPtr<Gtk::ListStore> listStore; 
	int numberOfTracks;
	
	Image *imMusic;	
};

#endif
