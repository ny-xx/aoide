/*
  	Filename:  .hh
	Author: Kreil Dominik	
	Description:

	Aoide is a simple musicplayer using the file structure
*/
#ifndef DIRECTORYTREE_HH
#define DIRECTORYTREE_HH

#include "debuglog.hh"

#include <boost/shared_ptr.hpp>
#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string.h>


using namespace Gtk;
using namespace Glib;
using namespace boost;
using namespace std;


class DirectoryTree : public TreeView
{
public:
    DirectoryTree(string *rPath);

    ~DirectoryTree();
	
	string getPath(const Gtk::TreeModel::iterator & iter);
	
	bool validateFileType(string path);
	
private:
	class Columns : public Gtk::TreeModel::ColumnRecord {
		public:
			Columns() {
				//add(col_id);
				add(col_img);
				add(col_name);
				//add(col_price);
				//add(col_stock);
			}
	
			~Columns() {}
	
			//Gtk::TreeModelColumn<unsigned> col_id;
			Gtk::TreeModelColumn<Glib::ustring> col_name;
			Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_img;
			//Gtk::TreeModelColumn<float> col_price;
			//Gtk::TreeModelColumn<bool> col_stock; 
    }; 

	int makeSubtree(TreeModel::Row &parrent, string path);
	void onExpanded(const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path);	
	void onCollapsed(const Gtk::TreeModel::iterator & iter,const Gtk::TreeModel::Path & path);
	RefPtr<TreeStore> treeStore; 
	Columns columns;
	vector<string> fileTypes; 

	Image *imOpen, *imClosed, *imMusic;

	shared_ptr<string> rootPath;
	DebugLog log;
};

#endif
